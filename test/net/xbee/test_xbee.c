/**
 * Copyright © 2011-2015 Pascal JEAN aka epsilonRT. All rights reserved.
 *
 * This file is part of AvrIO.
 *
 * AvrIO is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * AvrIO is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with AvrIO.  If not, see <http://www.gnu.org/licenses/lgpl.html>
 *
 * @file test_xbee.c
 * @brief Test XBee Routeur/End Device
 * - Affiche le contenu des paquets de données reçus sur le LCD
 * - Transmet un paquet de données de test à chaque appui sur le bouton poussoir
 * .
 */
#define __ASSERT_USE_STDERR
#include <avrio/assert.h>
#include <avrio/xbee.h>
#include <avrio/serial.h>
#include <avrio/led.h>
#include <avrio/delay.h>
#include <avrio/lcd.h>
#include <avrio/twi.h>
#include <avrio/button.h>
#include <avrio/hih6130.h>

/* constants ================================================================ */
#define DEFAULT_BAUDRATE  38400

/* build options ============================================================ */
#define CONFIG_XB_RESET      PD7
#define CONFIG_XB_RESET_DDR  DDRD
#define CONFIG_XB_RESET_PORT PORTD

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

/* private functions ======================================================== */
int iDataCB (xXBee *xbee, xXBeePkt *pkt, uint8_t len);
int iTxStatusCB (xXBee *xbee, xXBeePkt *pkt, uint8_t len);
int iAtLocalCB (xXBee *xbee, xXBeePkt *pkt, uint8_t len);
int iModemStatusCB (xXBee * xbee, xXBeePkt * pkt, uint8_t len);
void vLedAssert (int i);
int iInit (xSerialIos * xIosSet);
int iAtLocalCmd (const char * sCmd, uint8_t * pParams, uint8_t ucParamsLen);

/* internal public functions ================================================ */
int
main (void) {
  int i;
  static int ret;
  xSerialIos xIosSet = {
    .baud = DEFAULT_BAUDRATE, .dbits = SERIAL_DATABIT_8,
    .parity = SERIAL_PARITY_NONE, .sbits = SERIAL_STOPBIT_ONE,
    .flow = SERIAL_FLOW_RTSCTS, .flag = 0
  };

  /*
   * Initialisation
   */
  if (iInit (&xIosSet) != 0) {

    return -1;
  }

  /*
   * Lecture du PAN ID demandé par le module XBee
   */
  if (iAtLocalCmd (XBEE_CMD_PAN_ID, NULL, 0) == XBEE_PKT_STATUS_OK) {

    ret = iXBeePktParamLen (xAtLocalPkt);

    if (ret == 8) {

      if (iXBeePktParamGetULongLong (&ullPanID, xAtLocalPkt, 0) == 0) {

        printf ("PANID>%lX%lX\nWaiting to join", (uint32_t) (ullPanID << 32),
                (uint32_t) ullPanID);
      }
    }
    vXBeeFreePkt (xbee, xAtLocalPkt);
  }
  

  /*
   * Attente connexion réseau ZigBee
   */
  i = xLcdGetX();
  while (ucModemStatus != XBEE_PKT_MODEM_JOINED_NETWORK) {

    if (i++ < xLcdWidth()) {

      putchar ('.');
    }
    else {

      vLcdClearCurrentLine();
      i = 0;
    }
    
    // Scrute la réception des paquets du XBee
    iXBeePoll (xbee, 0);
    delay_ms (1000);
  }

  // Connecté au réseau
  vLcdClear();

  /*
   * Lecture du PAN ID effectif du module XBee
   */
  if (iAtLocalCmd (XBEE_CMD_OPERATING_PAN_ID, NULL, 0) == XBEE_PKT_STATUS_OK) {

    ret = iXBeePktParamLen (xAtLocalPkt);

    if (ret == 8) {

      vLcdClear();
      if (iXBeePktParamGetULongLong (&ullPanID, xAtLocalPkt, 0) == 0) {

        printf ("PANID>%lX%lX\nNetwork joined\n", (uint32_t) (ullPanID << 32),
                (uint32_t) ullPanID);
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

      vLcdGotoXY(0, 1);
      vLcdClearCurrentLine();
      printf ("NID>%s\n", sMyNID);
      vLedClear (LED_LED1);
    }
    vXBeeFreePkt (xbee, xAtLocalPkt);
  }

  for (;;) {

    // Scrute la réception des paquets
    ret = iXBeePoll (xbee, 0);
    assert (ret == 0);

    if (xButGet (BUTTON_BUTTON1)) {
      char message[33];
      static int iCount = 1;

      vLedSet (LED_LED1);
      snprintf (message, 32, "%s #%d", sMyNID, iCount++);

      iDataFrameId = iXBeeZbSendToCoordinator (xbee, message, strlen (message));
      assert (iDataFrameId >= 0);
      vLcdClear();
      printf ("Tx%d>%s\n", iDataFrameId, message);
      while (xButGet (BUTTON_BUTTON1))
        ; // Attente relachement bouton pour éviter des envois multiples
    }
  }

  ret = iXBeeClose (xbee);
  return 0;
}

// -----------------------------------------------------------------------------
int
iInit (xSerialIos * xIosSet) {

  /*
   * Active le RESET du module XBee
   */
  CONFIG_XB_RESET_DDR  |=  _BV (CONFIG_XB_RESET);
  CONFIG_XB_RESET_PORT &= ~_BV (CONFIG_XB_RESET);

  /*
   * Init LED, utilisée pour signaler la réception d'un paquet ou pour signaler
   * une erreur LCD
   */
  vLedInit();

  /*
   * Init du bus I2C en mode maître à 400 kHz utilisé par le LCD
   */
  vTwiInit ();
  eTwiSetSpeed (400);

  /*
   * Init LCD, c'est la sortie standard et d'erreur
   * Dans le cas de la carte XNODE, l'afficheur est connecté par I2C, le bus
   * I2C est donc initialisé à la vitesse par défaut (100 kHz)
   */
  vLedAssert (iLcdInit() == 0);
  ucLcdBacklightSet (32);
  stdout = &xLcd;
  stderr = &xLcd;

  printf ("**  XBee Test **\nInit... ");

  /*
   * Init bouton poussoir, un appui permet de déclencher une mesure
   */
  vButInit();

  /*
   * Init du capteur HIH6130 (thermomètre, humidimètre I2C)
   */
  e = eHih6130Init (0);
  /*
   * assert() vérifie l'absence d'erreur, si erreur, affiche sur la sortie
   * d'erreur et stop le programme
   */
  assert (e == 0);

  /*
   * Init liaison série vers module XBee
   * Mode lecture/écriture, non bloquant, avec contrôle de flux matériel
   */
  if ( (xbee = xXBeeOpen (NULL, xIosSet, XBEE_SERIES_S2)) == NULL) {

    fprintf (stderr, "Failed !");
    return -1;
  }

  /*
   * Init XBee, mise en place des gestionnaires de réception
   */
  vXBeeSetCB (xbee, XBEE_CB_DATA, iDataCB);
  vXBeeSetCB (xbee, XBEE_CB_TX_STATUS, iTxStatusCB);
  vXBeeSetCB (xbee, XBEE_CB_AT_LOCAL, iAtLocalCB);
  vXBeeSetCB (xbee, XBEE_CB_MODEM_STATUS, iModemStatusCB);
  sei(); // La réception UART utilise les interruptions....

  /*
   * Désactive le RESET du module XBee puis remet la broche en entrée avec
   * résistance de pull-up
   */
  CONFIG_XB_RESET_PORT |=   _BV (CONFIG_XB_RESET);
  CONFIG_XB_RESET_DDR  &=  ~_BV (CONFIG_XB_RESET);

  printf ("Success");
  delay_ms (500);
  vLcdClear();
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
  }
  return 0;
}

// -----------------------------------------------------------------------------
int
iAtLocalCmd (const char * sCmd, uint8_t * pParams, uint8_t ucParamsLen) {

  iAtLocalStatus = XBEE_PKT_STATUS_UNKNOWN;
  iAtLocalFrameId = iXBeeSendAt (xbee, sCmd, pParams, ucParamsLen);
  assert (iAtLocalFrameId > 0);

  while (iAtLocalStatus == XBEE_PKT_STATUS_UNKNOWN) {

    iXBeePoll (xbee, 0);
  }
  return iAtLocalStatus;
}

// -----------------------------------------------------------------------------
// Gestionnaire de réception des paquets de données
int
iDataCB (xXBee *xbee, xXBeePkt *pkt, uint8_t len) {
  int size;

  vLedSet (LED_LED1);
  size = iXBeePktDataLen (pkt);
  if (size > 0) {
    volatile char * p;
    uint8_t * addr64 = pucXBeePktAddrSrc64 (pkt);
    
    // Affiche l'adresse 64-bits de la source et le contenu du paquet
    vLcdClear();
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
    p = (char *) pucXBeePktData (pkt);
    p[size] = 0;
    printf ("%s\n", p);
  }
  vXBeeFreePkt (xbee, pkt);
  vLedClear (LED_LED1);
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
      vLedClear (LED_LED1);
    }
    iDataFrameId = 0;
  }
  vXBeeFreePkt (xbee, pkt);
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
  vLedToggle (LED_LED1);
  return 0;
}

/* -----------------------------------------------------------------------------
 * Vérifie que la condition passée est vraie, sinon fait clignoter rapidement
 * la led 1 à l'infinie
 */
void
vLedAssert (int i) {

  if (!i) {

    for (;;) {

      vLedSet (LED_LED1);
      delay_ms (5);
      vLedClear (LED_LED1);
      delay_ms (25);
    }
  }
}
/* ========================================================================== */
