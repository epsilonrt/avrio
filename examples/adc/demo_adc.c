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
 * @file demo_adc.c
 * @brief Exemple ADC
 *
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
