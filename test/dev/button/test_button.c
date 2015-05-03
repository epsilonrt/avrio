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
 * @file test_button.c
 * @brief Tests unitaires du module \ref button_module "Bouton poussoir".
 *

   ========================================================================== */
#include <avrio/led.h>
#include <avrio/button.h>
#include <avrio/delay.h>

/* ========================================================================== */
/**
 * @addtogroup test_group
 * @{
 */

 /* 
  * Validation des tests à effectuer
  */

/* internal public functions ================================================ */
int
main (void) {
  uint8_t ucBit;

  vLedInit ();
  vButInit ();

  for (;;) {

    for (ucBit = 0; ucBit < BUTTON_QUANTITY; ucBit++) {

      if (xButGet (xButGetMask (ucBit))) {
        uint8_t ucCount = (ucBit + 1) * 2;
        
        while (ucCount--) {
          // La LED clignote une fois pour le bouton 1, 2 fois pour le 2 ....
          vLedToggle (LED_LED1);
          delay_ms (200);
        }
      }
    }
  }
  return 0;
}

/** @} */
/* ========================================================================== */
