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
 * @file demo_bisrelay.c
 * @brief Exemple Relais Bistable
   ========================================================================== */
#include <avrio/led.h>
#include <avrio/delay.h>
#include <avrio/button.h>
#include <avrio/bisrelay.h>

static void vWaitButton (void);
static void vLedBlink (uint8_t ucCount);

/* main ===================================================================== */
int
main (void) {

  vLedInit();
  vButInit();
  vBisRelayInit ();

  for (;;) {
    uint8_t ucRelay = 1;

    // Mets à l'état On les relais un par un à chaque appui sur le BP
    // La led clignote à chaque appui du nombre de fois correspondant au
    // numéro du relais basculé
    for (uint8_t i = 0; i < BISRELAY_QUANTITY; i++) {

      vWaitButton();
      vBisRelaySet (ucRelay);
      vLedBlink (i);
      ucRelay <<= 1;
    }

    // Mets à l'état Off les relais un par un à chaque appui sur le BP
    // La led clignote à chaque appui du nombre de fois correspondant au
    // numéro du relais basculé
    ucRelay = 1;
    for (uint8_t i = 0; i < BISRELAY_QUANTITY; i++) {

      vWaitButton();
      vBisRelayClear (ucRelay);
      vLedBlink (i);
      ucRelay <<= 1;
    }

    // Inverse l'état des relais 8 fois à chaque appui sur le BP
    vWaitButton();
    vBisRelaySet (0x55);
    for (uint8_t i = 0; i < 7; i++) {

      vWaitButton();
      vBisRelayToggle (BISRELAY_ALL);
    }

    // Tous à On, puis tous à Off 8 fois à une seconde d'écart
    vWaitButton();
    for (uint8_t i = 0; i < 7; i++) {

      vBisRelaySet (BISRELAY_ALL);
      delay_ms (1000);
      vBisRelayClear (BISRELAY_ALL);
      delay_ms (1000);
    }
  }
  return 0;
}

// -----------------------------------------------------------------------------
static void
vWaitButton (void) {

  while (!xButGet(BUTTON_BUTTON1))
    delay_ms(20);
  while (xButGet(BUTTON_BUTTON1))
    delay_ms(20);
}

// -----------------------------------------------------------------------------
static void
vLedBlink (uint8_t ucCount) {

  ucCount++;
  while (ucCount--) {

    vLedSet (LED_LED1);
    delay_ms (100);
    vLedClear (LED_LED1);
    delay_ms (200);
  }
}
/* ========================================================================== */
