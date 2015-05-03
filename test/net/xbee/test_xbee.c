/**
 * @file test_xbee.c
 * @brief Test du module XBee
 *
 * Envoie une trame Ax25/APRS de façon périodique.
 *
 * @author Copyright © 2014 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>

   ========================================================================== */
#define __ASSERT_USE_STDERR
#include <avrio/assert.h>
#include <avrio/xbee.h>
#include <avrio/serial.h>
#include <avrio/led.h>
#include <avrio/delay.h>
#include <avrio/lcd.h>
#include <avrio/button.h>
#include <avrio/hih6130.h>

/* constants ================================================================ */
// Baudrate de la liaison série en baud
#define SER_BAUDRATE  38400

/* private variables ======================================================== */
static xXBee xbee;
volatile int frame_id = 0;

/* private functions ======================================================== */
int iDataCB (xXBee *xbee, xXBeePkt *pkt, uint8_t len);
int iTxStatusCB (xXBee *xbee, xXBeePkt *pkt, uint8_t len);
void vLedAssert (int i);

/* internal public functions ================================================ */
int
main (void) {
  static eHih6130Error e;
  static xHih6130Data xData;
  static bool isStarted = false;

  /*
   * Init LED, utilisée pour signaler la réception d'un paquet ou pour signaler
   * une erreur LCD
   */
  vLedInit();

  /*
   * Init LCD, c'est la sortie standard et d'erreur
   * Dans le cas de la carte XNODE, l'afficheur est connecté par I2C, le bus
   * I2C est donc initialisé à la vitesse par défaut (100 kHz)
   */
  vLedAssert (iLcdInit() == 0);
  ucLcdBacklightSet (32);
  stdout = &xLcd;
  stderr = &xLcd;

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
  vSerialInit (SER_BAUDRATE / 100, SERIAL_DEFAULT | SERIAL_RW | SERIAL_NOBLOCK | SERIAL_RTSCTS);

  /*
   * Init XBee, mise en place des gestionnaires de réception
   */
  vXBeeInit (&xbee, &xSerialPort);
  vXBeeSetCB (&xbee, XBEE_CB_DATA, iDataCB);
  vXBeeSetCB (&xbee, XBEE_CB_TX_STATUS, iTxStatusCB);
  sei(); // La réception UART utilise les interruptions....

  for (;;) {

    // Scrute la réception des paquets
    iXBeePoll (&xbee);
    
    // Si bouton appuyé, lancer mesure
    if (xButGet (BUTTON_BUTTON1)) {

      e = eHih6130Start();
      assert (e == 0);
      isStarted = true;
      while (xButGet (BUTTON_BUTTON1))
        ; // Attente relachement bouton pour éviter des mesures multiples
    }
    
    // Si fin de mesure, transmettre le résultat
    if ( (isStarted) && (eHih6130Read (&xData) == 0)) {
      char message[16];

      isStarted = false;
      sprintf (message, "T%04dH%04d", xData.iTemp, xData.iHum);
      vLcdClear();
      printf (">%s\n", message);
      frame_id = iXBeeZbSendToCoordinator (&xbee, message, strlen (message));
      assert (frame_id >= 0);
    }
  }
  return 0;
}

// -----------------------------------------------------------------------------
// Gestionnaire de réception des paquets de données
int
iDataCB (xXBee *xbee, xXBeePkt *pkt, uint8_t len) {
  int size;

  size = iXBeePktDataLen (pkt);
  if (size > 0) {
    volatile char * p;
    uint8_t * addr64 = pucXBeePktAddrSrc64 (pkt);
    
    // Affiche l'adresse 64-bits de la source et le contenu du paquet
    vLcdClear();
    for (int i = 0; i < 8; i++) {
      
      printf ("%02X", addr64[i]);
    }
    p = (char *) pucXBeePktData (pkt);
    p[size] = 0;
    printf ("%s\n", p);
  }
  vXBeeFreePkt (xbee, pkt);
  vLedToggle (LED_LED1);
  return 0;
}

// -----------------------------------------------------------------------------
// Gestionnaire de status de fin de transmission
int 
iTxStatusCB (xXBee *xbee, xXBeePkt *pkt, uint8_t len) {
  
  if (iXBeePktFrameId (pkt) == frame_id) {
    int status = iXBeePktStatus (pkt);
    
    if (status) {
      
      printf ("Tx%d Err. %d", frame_id, status);
    }
    else {
      
      printf ("Tx%d Ok", frame_id);
    }
    frame_id = 0;
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
