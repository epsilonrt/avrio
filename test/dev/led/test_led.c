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
 * @file test_led.c
 * @brief Tests unitaires du module \ref led_module "LED".
 *

   ========================================================================== */
#include <avrio/led.h>
#include <avrio/delay.h>

/* ========================================================================== */
/**
 * @addtogroup test_group
 * @{
 */

 /*
  * Validation des tests à effectuer
  */
#define TEST_SETCLEAR
#define TEST_TOGGLE
#define TEST_MASK_ARRAY

#ifdef TEST_SETCLEAR
  // ----------------------------------------------------------------------------
static void
prvvTestSetClear (void) {
  uint8_t ucCount = 10;

  while (ucCount--) {

    vLedSet (LED_ALL_LEDS);
    delay_ms (50);
    vLedClear (LED_ALL_LEDS);
    delay_ms (550);
  }
  delay_ms (SECONDS (1));
}
#else
#  define prvvTestSetClear()
#endif

#ifdef TEST_TOGGLE
  // ----------------------------------------------------------------------------
static void
prvvTestToggle (void) {
  uint8_t ucCount = 20;

  while (ucCount--) {

    vLedToggle (LED_ALL_LEDS);
    delay_ms (250);
  }
  delay_ms (SECONDS (1));
}
#else
#  define prvvTestToggle()
#endif

#ifdef TEST_MASK_ARRAY
  // ----------------------------------------------------------------------------
static void
prvvTestMaskArray (void) {
  uint8_t ucLed, ucCount = 10;

  while (ucCount--) {

    for (ucLed = 0; ucLed < LED_QUANTITY; ucLed++) {

      vLedSet (xLedGetMask (ucLed));
      delay_ms (300);
      vLedClear (xLedGetMask (ucLed));
      delay_ms (300);
    }
    delay_ms (SECONDS (1));
  }
}
#else
#  define prvvTestMaskArray()
#endif

/* main ===================================================================== */
int
main (void) {

  vLedInit ();
  for (;;) {

    prvvTestSetClear ();
    prvvTestToggle ();
    prvvTestMaskArray ();
  }
  return 0;
}

/** @} */
/* ========================================================================== */
