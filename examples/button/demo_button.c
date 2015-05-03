/**
 * @file demo_button.c
 * @brief Exemple d'utilisation "Bouton poussoir".
 *
 * Un appui sur le bouton BUTTON1 allume la LED1. Un nouvel appui l'éteind.
 * Si on reste appuyé sur BUTTON1, la LED1 clignote.
 *
 * @author Copyright © 2011-2012 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>

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
