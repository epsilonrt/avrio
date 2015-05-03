/**
 * @file demo_adc.c
 * @brief Exemple ADC
 * @author Copyright © 2011-2012 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * Revision History ------------------------------------------------------------
 *    20120303 - Initial version by epsilonRT
 */
#include <avrio/led.h>
#include <avrio/delay.h>
#include <avrio/serial.h>
#include <avrio/adc.h>
#include <stdio.h>
#include "avrio-board-adc.h"

/* ATTENTION
 * avrio-board-adc.h doit être adapté en fonction de la fréquence d'horloge
 * choisie et en fonction de la tension de référence choisie.
 */

/* constants ================================================================ */
#define TEST_BAUDRATE 38400

/* internal public functions ================================================ */
void vStdOutInit (void);
void vPrintValue (uint8_t ucIndex, uint16_t usAdc);

static const uint8_t ucAdcChan[ADC_CHAN_QUANTITY] = ADC_CHAN_LIST;
static const double dAdcFullScale [ADC_CHAN_QUANTITY] = ADC_FULLSCALE_LIST;

/* main ===================================================================== */
int
main (void) {
  uint16_t usAdc;
  uint8_t ucIndex;

  vStdOutInit();
  vAdcInit();

  for (;;) {

    for (ucIndex = 0; ucIndex < ADC_CHAN_QUANTITY; ucIndex++) {

      usAdc  = usAdcReadAverage (ucAdcChan[ucIndex], 8);
      vPrintValue (ucIndex, usAdc);
    }
    vLedToggle (LED_LED1);
    delay_ms (1000);
  }
  return 0;
}


// -----------------------------------------------------------------------------
void
vStdOutInit (void) {

  vSerialInit (TEST_BAUDRATE / 100, SERIAL_DEFAULT + SERIAL_RW);
  stdout = &xSerialPort;
  printf ("\nExemple ADC\n");
}

// -----------------------------------------------------------------------------
void
vPrintValue (uint8_t ucIndex, uint16_t usAdc) {

  if ((ucIndex == 0) || ((ucIndex % 4) == 0)) {

    putchar('\n');
  }
  else {

    putchar(' ');
  }
  printf ("A%01d=%.2f (%04d)", ucIndex,
                        ADC_MEASUREMENT(usAdc, dAdcFullScale[ucIndex]), usAdc);
}
/* ========================================================================== */
