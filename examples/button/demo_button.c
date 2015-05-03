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
 * @file demo_button.c
 * @brief Exemple d'utilisation "Bouton poussoir".
 *
 * Un appui sur le bouton BUTTON1 allume la LED1. Un nouvel appui l'éteind.
 * Si on reste appuyé sur BUTTON1, la LED1 clignote.
 *
 *

   ========================================================================== */
#include <avrio/led.h>
#include <avrio/button.h>
#include <avrio/delay.h>

int
main (void) {

  vLedInit ();
  vButInit ();

  for (;;) {

    if (xButGet (BUTTON_BUTTON1) != 0) {

      // Toggle: Basculer en anglais
      vLedToggle (LED_LED1);
      delay_ms (200);
    }
  }
  return 0;
}
