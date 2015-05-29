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
vIsr (xIrqContext * ctx) {
  int * iCounter = (int *)ctx->udata;

  if (ctx->chan == 1) {
    vLedToggle (LED_LED1);
  }
  (*iCounter)++;
}

/* main ===================================================================== */
int
main (void) {

  vLedInit ();
  //vIrqSetUserData (0, &iIrqCount[0]);
  //vIrqAttach (0, vIsr, eIrqRising);
  vIrqSetUserData (1, &iIrqCount[1]);
  vIrqAttach (1, vIsr, eIrqRising);
  sei();

  for (;;) {

  }
  return 0;
}
/* ========================================================================== */
