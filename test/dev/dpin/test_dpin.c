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
 * @file test_tc.c
 * @brief Test liaison série
 */
#include <avrio/dpin.h>
#include <avrio/delay.h>

/* constants ================================================================ */
static xDPin xPa[8];
static xDPin xPe[4] = {
  { .port = &PORTE, .pin = 4 },
  { .port = &PORTE, .pin = 5 },
  { .port = &PORTE, .pin = 6 },
  { .port = &PORTE, .pin = 7 },
};

/* main ===================================================================== */
int
main (void) {
  bool bValue = true;

  for (uint8_t i = 0; i < 8; i++) {

    vDpInit (&xPa[i], &PORTA, i, eModeOutput);
  }

  for (uint8_t i = 0; i < 4; i++) {

    vDpSetMode (&xPe[i], eModeInputPullUp);
  }

  for (;;) {
    
    for (uint8_t j = 0; j < 8; j++) {
      
      for (uint8_t i = 0; i < 8; i++) {

        vDpWrite (&xPa[i], bValue);
      }
      bValue = !bValue;
    }
    
    for (uint8_t j = 0; j < 8; j++) {
      
      for (uint8_t i = 0; i < 4; i++) {

        bValue = bDpRead (&xPe[i]);
        vDpWrite (&xPa[i], bValue);
      }
    }    
  }
  return 0;
}
/* ========================================================================== */
