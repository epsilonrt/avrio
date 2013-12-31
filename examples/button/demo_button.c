/**
 * @file demo_button.c
 * @brief Tests unitaires du module \ref button_module "Bouton poussoir".
 * @author Copyright © 2011-2012 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * @version $Id$
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
