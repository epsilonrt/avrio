/**
 * @file test_irq.c
 * @brief Tests unitaires du module irq
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
#include <avrio/irq.h>
#include <avrio/led.h>
#include <avrio/delay.h>

/* private variables ======================================================== */
static int iIrqCount[2];

/* private functions ======================================================== */
void
vIsr (int8_t i) {
  int * iCounter = (int *) pvIrqGetUserData (i);

  if (i == INT1) {

    switch (eIrqGetMode (i)) {
      case eIrqRising:
        if (bIrqReadPin (i)) {

          vLedToggle (LED_LED1);
        }
        break;
      case eIrqFalling:
        if (bIrqReadPin (i) == 0) {

          vLedToggle (LED_LED1);
        }
        break;
      case eIrqEdge:
        if (bIrqReadPin (i)) {

          vLedSet (LED_LED1);
        }
        else {
          vLedClear (LED_LED1);
        }
        break;
      case eIrqLowLevel:
        break;
    }
  }
  (*iCounter) ++;
}

/* main ===================================================================== */
int
main (void) {

  vLedInit ();
  vIrqSetUserData (INT0, &iIrqCount[INT0]);
  vIrqAttach (INT0, vIsr, eIrqRising);
  vIrqDisable (INT0);
  vIrqSetUserData (INT1, &iIrqCount[INT1]);
  vIrqAttach (INT1, vIsr, eIrqRising);
  sei();

  for (;;) {
    // Rien à faire
  }
  return 0;
}
/* ========================================================================== */
