/**
 * @file test_twi_hih6130.c
 * @brief Test unitaire HIH6130
 * @author Copyright © 2011-2012 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * Revision History ------------------------------------------------------------
 *    20120303 - Initial version by epsilonRT
 */
#include <avrio/led.h>
#include <avrio/delay.h>
#include <avrio/hih6130.h>
#include <avrio/serial.h>
#include <stdio.h>
#include <avr/pgmspace.h>

/* ========================================================================== */

/* constants ================================================================ */
#define TEST_BAUDRATE 115200

/* private functions ======================================================== */
// ------------------------------------------------------------------------------
static void
vAssert (bool test) {

  if (test == false) {

    for (;;) {

      vLedSet (LED_LED1);
      delay_ms (25);
      vLedClear (LED_LED1);
      delay_ms (100);
    }
  }
}

/* main ===================================================================== */
int
main (void) {
  eTwiStatus eTwiError;
  eHih6130Error eError;
  xHih6130Data xData;
  uint16_t usCount = 0;
  
  vLedInit ();
  vSerialInit (TEST_BAUDRATE / 100, SERIAL_DEFAULT + SERIAL_RW);
  stdout = &xSerialPort;
  printf_P (PSTR("\nTest unitaire HIH6130\n"));
  
  vTwiInit ();
  eTwiError = eTwiSetSpeed (100);
  vAssert (eTwiError == TWI_SUCCESS);

  eError = eHih6130Init (0);
  vAssert (eError == HIH6130_SUCCESS);
  
  for (;;) {

    
    // Test 1 - Mesure 
    // Le temps d'allumage LED_LED1 correspond au temps de mesure
    vLedSet (LED_LED1);
    eError = eHih6130Start();
    vAssert (eError == HIH6130_SUCCESS);
    
    do {
    
      // Lecture
      eError = eHih6130Read (&xData);
      // Vérif absence d'erreur
      vAssert (eError >= HIH6130_SUCCESS);
    } while (eError == HIH6130_BUSY);
    vLedClear (LED_LED1);
    
    printf_P (PSTR("%05d - Read Temp. = %.1f oC - Read Hum. = %.1f %% \n"), 
              usCount++, xData.iTemp / 10.0,  
              xData.iHum  / 10.0);
    delay_ms (1000);
  }
  return 0;
}

/* ========================================================================== */
