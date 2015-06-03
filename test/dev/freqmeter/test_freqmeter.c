/**
 * @file test_freqmeter.c
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
#include <avrio/freqmeter.h>

/* constants ================================================================ */
#define TEST_BAUDRATE     38400
#define TEST_SETUP        (SERIAL_DEFAULT + SERIAL_RW)

/* private variables ======================================================== */
static double dFreq;

/* main ===================================================================== */
int
main (void) {
  xFreqMeter xMyMeter;
  xFreqMeter * fm = &xMyMeter;

  vLedInit ();
  ATOMIC_BLOCK (ATOMIC_FORCEON) {
    vSerialInit (TEST_BAUDRATE / 100, TEST_SETUP);
    stdout = &xSerialPort;
    vFreqMeterInit (fm, INT1);
    vFreqMeterSetWindow (fm, 1000);
  }
  printf ("Frequency Meter Test\nWindow=%u ms\n", fm->usWindow);

  for (;;) {

    if (bFreqMeterIsComplete (fm)) {
      dFreq = dFreqMeterRead (fm);
      printf ("%.1f\n", dFreq);
      delay_ms (50);
      vFreqMeterStart (fm);
    }
  }
  return 0;
}
/* ========================================================================== */
