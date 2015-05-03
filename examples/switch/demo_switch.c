/**
 * @file test_switch.c
 * @brief Exemple du module \ref switch_module "Micro interrupteurs".
 * @author Copyright © 2011-2014 epsilonRT. All rights reserved.
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
