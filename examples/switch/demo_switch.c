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
 * @file test_switch.c
 * @brief Exemple du module \ref switch_module "Micro interrupteurs".
 * Copyright © 2011-2014 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>

   ========================================================================== */
#include <avrio/led.h>
#include <avrio/switch.h>
#include <avrio/delay.h>

int
main (void) {
  xSwitch  xPrev = 0;
  xSwitch  xCurrent;

  vLedInit ();
  vSwitchInit ();

  for (;;) {

    xCurrent = xSwitchGet();
    if (xCurrent != xPrev) {

      xPrev = xCurrent;
      while (xCurrent--) {
        // La LED clignote le nombre de fois correspondant à la valeur des switchs
        vLedSet (LED_LED1);
        delay_ms (200);
        vLedClear (LED_LED1);
        delay_ms (200);
      }
    }
  }
  return 0;
}
