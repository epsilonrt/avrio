/**
 * @file demo_tsl230.c
 * @brief Démonstration utilisation capteur TSL230
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
#include <util/atomic.h>
#include <avrio/led.h>
#include <avrio/delay.h>
#include <avrio/serial.h>
#include <avrio/tsl230.h>

/* constants ================================================================ */
#define TEST_BAUDRATE     38400
#define TEST_SETUP        (SERIAL_DEFAULT + SERIAL_RW)

/* private variables ======================================================== */
static double dFreq, dIrradiance;

/* main ===================================================================== */
int
main (void) {

  vLedInit ();
  ATOMIC_BLOCK (ATOMIC_FORCEON) {
    vSerialInit (TEST_BAUDRATE / 100, TEST_SETUP);
    stdout = &xSerialPort;
    vTsl230Init();
  }
  printf ("Tsl230 Demo\nfo(Hz)\tEe(uW/cm2)\n");

  for (;;) {

    if (bTsl230IsComplete()) {

      dFreq = dTsl230Freq();
      if (dFreq >= 0) {
        dIrradiance = dTsl230FreqToIrradiance (dFreq);
        printf ("%.1f\t%.1f\n", dFreq, dIrradiance);
        delay_ms (100);
      }
      vTsl230Start();
    }
  }
  return 0;
}
/* ========================================================================== */
