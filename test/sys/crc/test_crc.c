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
 * @file test_crc.c
 * @brief Tests unitaires du module \ref crc_module "CRC".
 */
#include <avrio/led.h>
#include <avrio/delay.h>
#include <avrio/crc.h>
#include <string.h>

#define TEST_STR "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define TEST_CRC 0xC452

/* private functions ======================================================== */
// ------------------------------------------------------------------------------
static void
vAssert (bool test) {

  if (test == false) {

    for (;;) {

      vLedSet (LED_LED1);
      delay_ms (50);
      vLedClear (LED_LED1);
      delay_ms (150);
    }
  }
}

/* internal public functions ================================================ */
int
main (void) {
  uint16_t usCrc = CRC_CCITT_INIT_VAL;

  vLedInit ();

  for (;;) {

    usCrc = usCrcCcitt (CRC_CCITT_INIT_VAL, TEST_STR, strlen (TEST_STR));
    vAssert (usCrc == TEST_CRC);
    vLedToggle (LED_LED1);
    delay_ms (1000);
  }
  return 0;
}

/* ========================================================================== */
