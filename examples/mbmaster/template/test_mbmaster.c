/**
 * @file test_mbmaster.c
 * @brief Test de la couche port AVR de MBMaster
 * @author Copyright _ 2007 Christian Walter <wolti@sil.at> All rights reserved.
 * @author Copyright _ 2012 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * Revision History ------------------------------------------------------------
 *    20120604 - Initial version by epsilonRT from MBmaster 2.7.0
 */
#include <avr/interrupt.h>
#include <avrio/led.h>
#include "mbport.h"
#include "mbmporttest.h"

/* macros =================================================================== */
/* constants ================================================================ */
/* private variables ======================================================== */

/* private functions ======================================================== */

// -----------------------------------------------------------------------------
void
vMBPTestDebugLED ( UBYTE ubIdx, BOOL bTurnOn ) {
  uint8_t ucLed = xLedGetMask (ubIdx);

  if (bTurnOn) {
  
    vLedSet (ucLed);
  }
  else {
  
    vLedClear (ucLed);
  }
}

/* internal public functions ================================================ */
int
main (void) {

  vLedInit();
  sei();
  
  for (;;) {
  
    vMBPTestRun ();
  }

  return 0;
}
/* ========================================================================== */
