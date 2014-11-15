/**
 * @file test_xbee.c
 * @brief Test du module XBee
 *
 * Envoie une trame Ax25/APRS de façon périodique.
 *
 * @author Copyright © 2014 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * @version $Id$
   ========================================================================== */
#define __ASSERT_USE_STDERR
#include <assert.h>
#include <avrio/xbee.h>
#include <avrio/serial.h>
#include <avrio/led.h>
#include <avrio/delay.h>
#include <avrio/lcd.h>
#include <avrio/button.h>
#include <avrio/hih6130.h>

/* constants ================================================================ */
// Les constantes ci_dessous peuvent être modifiées au besoin par l'utilisateur
// Baudrate de la liaison série en baud
#define SER_BAUDRATE  38400

/* private variables ======================================================== */
static xXBee xbee;

int iDataCB (xXBee *xbee, xXBeePkt *pkt, uint8_t len);
void vLedAssert (int i);

/* internal public functions ================================================ */
int
main(void) {
  static eHih6130Error e;
  static xHih6130Data xData;

  vLedInit();
  vLedAssert (iLcdInit() == 0);
  stdout = &xLcd;
  stderr = &xLcd;
  // ucLcdBacklightSet (32);
  
  vButInit();
  
  e = eHih6130Init (0);
  assert (e == 0);
  
  vSerialInit (SER_BAUDRATE/100, SERIAL_DEFAULT | SERIAL_RW | SERIAL_NOBLOCK | SERIAL_RTSCTS);
  sei();
  
  vXBeeInit (&xbee, &xSerialPort);
  vXBeeSetCB (&xbee, XBEE_CB_DATA, iDataCB);

  for (;;) {
    
    iXBeePoll (&xbee);
    if (xButGet (BUTTON_BUTTON1)) {
      
      e = eHih6130Start();
      assert (e == 0);
    }
    if (eHih6130Read (&xData) == 0) {
      char message[16];
      
      sprintf (message, "DT%04XH%04X", )
      int i = iXBeeZbSendToCoordinator (&xbee, message, strlen (message));
      assert (i >= 0);
    }
  }
  return 0;
}

/* -----------------------------------------------------------------------------
 */
int 
iDataCB (xXBee *xbee, xXBeePkt *pkt, uint8_t len) {
  int size;

  size = iXBeePktDataLen (pkt);
  if (size > 0) {
    volatile char * p;

    vLcdClear();
    printf ("%08lX%08lX", ulXBeePktAddressSH(pkt), ulXBeePktAddressSL(pkt));
    p = (char *)pucXBeePktData (pkt);
    p[size] = 0;
    printf ("%s\n", p);
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
