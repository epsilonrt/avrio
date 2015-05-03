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
 * @file test_adc.c
 * @brief Test unitaire ADC
 *
 * Revision History ------------------------------------------------------------
 *    20120303 - Initial version by epsilonRT
 */
#include <avrio/led.h>
#include <avrio/delay.h>
#include <avrio/serial.h>
#include <avrio/adc.h>
#include <stdio.h>

/* ATTENTION
 * avrio-board-adc.h doit être adapté en fonction de la fréquence d'horloge
 * choisie et en fonction de la tension de référence choisie.
 */

/* constants ================================================================ */
#define TEST_BAUDRATE 38400
#define AVERAGE_TERMS 16

/* private variables ======================================================== */
static const uint8_t ucAdcChan[] = ADC_CHAN_LIST;
static const double dAdcFullScale[] = ADC_FULLSCALE_LIST;

/* internal public functions ================================================ */
void vStdOutInit (void);
void vPrintValue (uint8_t ucIndex, uint16_t usAdc);

/* main ===================================================================== */
int
main (void) {
  uint16_t usAdc;
  uint8_t ucIndex;

  vStdOutInit();
  vAdcInit();

#ifdef ADC_SCALE_ENABLE
  // Test mise à l'échelle
  for (uint8_t i = 0; i <= ucAdcGetScaleMax (0); i ++) {

    vAdcSetScale (0, i);
  }
  vAdcSetScale (0, 0);
#endif
  vAdcSetAutoscale (0);

  for (;;) {

    for (ucIndex = 0; ucIndex < ADC_CHAN_QUANTITY; ucIndex++) {

      if (ucAdcChan[ucIndex] > 5) {
        eAdcRef eRef = eAdcGetRef();

        vAdcSetRef (eAdcInternal);
        usAdc  = usAdcReadAverage (ucAdcChan[ucIndex], AVERAGE_TERMS);
        vAdcSetRef (eRef);
      }
      else {

        usAdc  = usAdcReadAverage (ucAdcChan[ucIndex], AVERAGE_TERMS);
      }
      vPrintValue (ucIndex, usAdc);
    }
    vLedToggle (LED_LED1);
    delay_ms (500);
  }
  return 0;
}

// -----------------------------------------------------------------------------
void
vStdOutInit (void) {

  vSerialInit (TEST_BAUDRATE / 100, SERIAL_DEFAULT + SERIAL_RW);
  stdout = &xSerialPort;
  printf ("\nTest unitaire ADC\n");
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
#ifdef ADC_SCALE_ENABLE
  printf ("A%01d(%01d) %.03f V (%04d)",
    ucAdcChan[ucIndex],
    ucAdcGetScale(ucIndex),
    ADC_MEASUREMENT(usAdc, dAdcFullScale[ucIndex]), usAdc);
#else
  printf ("A%01d %.03f V (%04d)",  ucAdcChan[ucIndex],
                      ADC_MEASUREMENT(usAdc, dAdcFullScale[ucIndex]), usAdc);
#endif
}
/* ========================================================================== */
