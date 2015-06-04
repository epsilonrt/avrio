/**
 * @file test_icounter.c
 * @brief Test unitaire mesure de fréquence d'un signal d'interruption
 *
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
 */
#include <avrio/led.h>
#include <avrio/delay.h>
#include <avrio/serial.h>
#include <avrio/tcapture.h>

/* constants ================================================================ */
#define TIM TCAPTURE1
#define TEST_TSL230
#define TEST_BAUDRATE     38400
#define TEST_SETUP        (SERIAL_DEFAULT + SERIAL_RW)

#ifdef TEST_TSL230
#include <avrio/tsl230.h>
#include "avrio-board-tsl230.h"
#endif

/* private variables ======================================================== */
static double dFreq;
static uint16_t usValue;

/* main ===================================================================== */
int
main (void) {

  vLedInit ();
  vSerialInit (TEST_BAUDRATE / 100, TEST_SETUP);
  stdout = &xSerialPort;

#ifdef TEST_TSL230
  vTsl230PinInit();
  vTsl230PinSetSensitivity (eTsl230Sensitivity10);
  vTsl230PinSetScale (eTsl230Scale1);
#endif

  vTCaptureInit();
  vTCaptureSetNoiseFilter (TIM, true);
  vTCaptureSetPrescaler (TIM, eTCaptureDiv64);
  sei();
  printf ("Timer Capture Test\nValue\tFreq\n");

  for (;;) {

    if (bTCaptureIsComplete (TIM)) {

      usValue = usTCaptureValue (TIM);
      dFreq = dTCaptureValueToFreq (TIM, usValue);
      printf ("%u\t%.3f\n", usValue, dFreq);
      delay_ms (100);
      vTCaptureStart (TIM);
    }
    vLedToggle (LED_LED1);
  }
  return 0;
}
/* ========================================================================== */
