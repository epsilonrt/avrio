/*
 * Test XBee Routeur/End Device
 * - Affiche le contenu des paquets de données reçus sur la liaison tty série
 * - Transmet un paquet de données de test à chaque appui sur le bouton poussoir
 */
#define __ASSERT_USE_STDERR
#include <avrio/assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <avr/pgmspace.h>
#include <avrio/xbee.h>
#include <avrio/led.h>
#include <avrio/delay.h>
#include <avrio/button.h>
#include <avrio/tc.h>
#include <avrio/mutex.h>

/* constants ================================================================ */
#define XBEE_BAUDRATE   38400
#define XBEE_PORT       "tty1"
#define XBEE_RESET_PORT PORTB
#define XBEE_RESET_PIN  7

#define TERMINAL_BAUDRATE 115200
#define TERMINAL_PORT     "tty0"

#define LED_INIT    LED_LED0
#define LED_STATUS  LED_LED1
#define LED_ATCMD   LED_LED2
#define LED_TX      LED_LED3
#define LED_RX      LED_LED4
#define LED_ERROR   LED_LED7

/* private variables ======================================================== */
static xXBee * xbee;
volatile uint8_t ucModemStatus;
// Numéro de trame Data
volatile int iDataFrameId = 0;
// Numéro de trame Data commande AT
volatile int iAtLocalFrameId = 0;
volatile int iAtLocalStatus = XBEE_PKT_STATUS_UNKNOWN;
static xXBeePkt * xAtLocalPkt;
static char sMyNID[21];
static uint64_t ullPanID;
static FILE * tc;
static xDPin xResetPin = { .port = &XBEE_RESET_PORT, .pin = XBEE_RESET_PIN };
static xMutex xButIrq = MUTEX_INITIALIZER;

/* private functions ======================================================== */
int iDataCB (xXBee *xbee, xXBeePkt *pkt, uint8_t len);
int iTxStatusCB (xXBee *xbee, xXBeePkt *pkt, uint8_t len);
int iAtLocalCB (xXBee *xbee, xXBeePkt *pkt, uint8_t len);
int iModemStatusCB (xXBee * xbee, xXBeePkt * pkt, uint8_t len);
void vLedAssert (int i);
int iInit (xSerialIos * xXBeeIos);
int iAtLocalCmd (const char * sCmd, uint8_t * pParams, uint8_t ucParamsLen);
void vWaitToJoinNetwork (void);

/* internal public functions ================================================ */
int
main (void) {
  static int ret;
  xSerialIos xXBeeIos = {
    .baud = XBEE_BAUDRATE, .dbits = SERIAL_DATABIT_8,
    .parity = SERIAL_PARITY_NONE, .sbits = SERIAL_STOPBIT_ONE,
    .flow = SERIAL_FLOW_RTSCTS, .eol = SERIAL_BINARY
  };

  /*
   * Initialisation
   */
  if (iInit (&xXBeeIos) != 0) {

    return -1;
  }

  vWaitToJoinNetwork();

  for (;;) {

    // Scrute la réception des paquets
    ret = iXBeePoll (xbee, 0);
    assert (ret == 0);

  if (xMutexTryLock (&xButIrq) == 0) {
      char message[33];
      static int iCount = 1;

      vLedSet (LED_TX);
      snprintf (message, 32, "%s #%d", sMyNID, iCount++);

      iDataFrameId = iXBeeZbSendToCoordinator (xbee, message, strlen (message));
      assert (iDataFrameId >= 0);

      printf ("Tx%d>%s\n", iDataFrameId, message);
      EIFR  |= _BV (INTF4);
      EIMSK |= _BV (INT4);
    }
  }

  ret = iXBeeClose (xbee);
  return 0;
}

// -----------------------------------------------------------------------------
// Gestionnaire de status de fin de transmission
int
iTxStatusCB (xXBee *xbee, xXBeePkt *pkt, uint8_t len) {

  if (iXBeePktFrameId (pkt) == iDataFrameId) {
    int status = iXBeePktStatus (pkt);

    if (status) {

      printf ("Tx%d Err. %d\n", iDataFrameId, status);
    }
    else {

      printf ("Tx%d Ok\n", iDataFrameId);
      vLedClear (LED_TX);
    }
    iDataFrameId = 0;
  }
  vXBeeFreePkt (xbee, pkt);
  return 0;
}

// -----------------------------------------------------------------------------
// Gestionnaire de réception des paquets de données
int
iDataCB (xXBee *xbee, xXBeePkt *pkt, uint8_t len) {
  int size;

  vLedSet (LED_RX);
  size = iXBeePktDataLen (pkt);
  if (size > 0) {
    volatile char * p;
    uint8_t * src64 = pucXBeePktAddrSrc64 (pkt);

    printf ("...");

    // Affiche les 4 octets LSB de l'adresse 64-bits de la source
    for (int i = 4; i < 8; i++) {

      printf ("%02X", src64[i]);
    }

    putchar ('>');
    // Indique si le paquet est en broadcast
    if (iXBeePktIsBroadcast (pkt)) {
      putchar ('*');
    }

    // puis le contenu du paquet
    p = (char *) pucXBeePktData (pkt);
    p[size] = 0; // Ajout d'un caractère de fin de chaine
    printf ("\n%s\n", p);
  }

  vXBeeFreePkt (xbee, pkt);
  vLedClear (LED_RX);
  return 0;
}

// -----------------------------------------------------------------------------
int
iModemStatusCB (xXBee * xbee, xXBeePkt * pkt, uint8_t len) {
  int i = iXBeePktStatus (pkt);

  if (i != -1) {

    ucModemStatus = (unsigned) i;
  }
  vXBeeFreePkt (xbee, pkt);
  return 0;
}

// -----------------------------------------------------------------------------
// Gestionnaire de réception des paquets de réponse à commande AT locale
int
iAtLocalCB (xXBee *xbee, xXBeePkt *pkt, uint8_t len) {

  if (iXBeePktFrameId (pkt) == iAtLocalFrameId) {

    iAtLocalStatus = iXBeePktStatus (pkt);
    iAtLocalFrameId = 0;
    if (iAtLocalStatus == XBEE_PKT_STATUS_OK) {

      xAtLocalPkt = pkt;
    }
    else {

      vXBeeFreePkt (xbee, pkt);
    }
    vLedClear (LED_ATCMD);
  }
  return 0;
}

// -----------------------------------------------------------------------------
void
vWaitToJoinNetwork (void) {
  int ret, i;
  /*
   * Lecture du PAN ID demandé par le module XBee
   */
  if (iAtLocalCmd (XBEE_CMD_PAN_ID, NULL, 0) == XBEE_PKT_STATUS_OK) {

    ret = iXBeePktParamLen (xAtLocalPkt);

    if (ret == 8) {

      if (iXBeePktParamGetULongLong (&ullPanID, xAtLocalPkt, 0) == 0) {

        printf_P (PSTR ("Requested PAN Id> 0x%lx\n"), ullPanID);
      }
    }
    vXBeeFreePkt (xbee, xAtLocalPkt);
  }

  i = printf_P (PSTR ("Waiting to join"));

  /*
   * Attente connexion réseau ZigBee
   */
  while (ucModemStatus != XBEE_PKT_MODEM_JOINED_NETWORK) {

    if (i++ < 80) {

      putchar ('.');
    }
    else {

      putchar ('\n');
      i = 0;
    }

    // Scrute la réception des paquets du XBee
    iXBeePoll (xbee, 0);
    delay_ms (1000);
  }

  // Connecté au réseau
  /*
   * Lecture du PAN ID effectif du module XBee
   */
  if (iAtLocalCmd (XBEE_CMD_OPERATING_PAN_ID, NULL, 0) == XBEE_PKT_STATUS_OK) {

    ret = iXBeePktParamLen (xAtLocalPkt);

    if (ret == 8) {

      if (iXBeePktParamGetULongLong (&ullPanID, xAtLocalPkt, 0) == 0) {

        printf_P (PSTR ("\nCurrent PAN Id> 0x%lx, Network joined\n"), ullPanID);
      }
    }
    vXBeeFreePkt (xbee, xAtLocalPkt);
  }
  delay_ms (2000);

  /*
   * Lecture du Node ID dans le module XBee
   */
  if (iAtLocalCmd (XBEE_CMD_NODE_ID, NULL, 0) == XBEE_PKT_STATUS_OK) {

    ret = iXBeePktParamGetStr (sMyNID, xAtLocalPkt, sizeof (sMyNID));

    if (ret > 0) {

      printf ("NID>%s\n", sMyNID);
      vLedClear (LED_INIT);
    }
    vXBeeFreePkt (xbee, xAtLocalPkt);
  }

  vXBeeSetCB (xbee, XBEE_CB_TX_STATUS, iTxStatusCB);
  vXBeeSetCB (xbee, XBEE_CB_DATA, iDataCB);
}

// -----------------------------------------------------------------------------
int
iInit (xSerialIos * xXBeeIos) {

  /*
   * Init LED, utilisée pour signaler la réception d'un paquet ou pour signaler
   * une erreur d'ouverture TC
   */
  vLedInit();
  vLedSet (LED_INIT);

  /*
   * Init bouton poussoir NORD, un appui permet d'envoyer un message
   */
  vMutexLock (&xButIrq);
  PORTE |= _BV (4);
  EICRB |= _BV (ISC41); // front descendant
  EIMSK |= _BV (INT4);

  /*
   * Init du terminal d'affichage des messages
   */
  xSerialIos xTermIos = SERIAL_SETTINGS (TERMINAL_BAUDRATE);
  tc = xFileOpen (TERMINAL_PORT, O_RDWR | O_NONBLOCK, &xTermIos);
  vLedAssert (tc != NULL);
  stdout = tc;
  stdin = tc;
  sei();

  printf_P (PSTR ("\n**  XBee Test **\nInit... "));
  xbee = xXBeeNew (XBEE_SERIES_S2, &xResetPin);
  assert (xbee);

  /*
   * Mise en place des gestionnaires sauf réception et émission
   */
  vXBeeSetCB (xbee, XBEE_CB_AT_LOCAL, iAtLocalCB);
  vXBeeSetCB (xbee, XBEE_CB_MODEM_STATUS, iModemStatusCB);

  /*
   * Init liaison série vers module XBee
   * Mode lecture/écriture, non bloquant, avec contrôle de flux matériel
   */
  if (iXBeeOpen (xbee, XBEE_PORT, xXBeeIos) != 0) {

    fprintf_P (stderr, PSTR ("iXBeeOpen() failed !\n"));
    return -1;
  }
  // sei(); // La réception UART utilise les interruptions....


  printf_P (PSTR ("Success\n"));
  delay_ms (500);
  return 0;
}

// -----------------------------------------------------------------------------
int
iAtLocalCmd (const char * sCmd, uint8_t * pParams, uint8_t ucParamsLen) {

  vLedSet (LED_ATCMD);
  iAtLocalStatus = XBEE_PKT_STATUS_UNKNOWN;
  iAtLocalFrameId = iXBeeSendAt (xbee, sCmd, pParams, ucParamsLen);
  assert (iAtLocalFrameId > 0);

  while (iAtLocalStatus == XBEE_PKT_STATUS_UNKNOWN) {

    iXBeePoll (xbee, 0);
  }
  return iAtLocalStatus;
}


/* -----------------------------------------------------------------------------
 * Vérifie que la condition passée est vraie, sinon fait clignoter rapidement
 * la led 1 à l'infinie
 */
void
vLedAssert (int i) {

  if (!i) {

    for (;;) {

      vLedSet (LED_ERROR);
      delay_ms (5);
      vLedClear (LED_ERROR);
      delay_ms (25);
    }
  }
}

//------------------------------------------------------------------------------
ISR (INT4_vect) {

  vMutexUnlock (&xButIrq);
  EIMSK &= ~_BV (INT4);
}

/* ========================================================================== */
