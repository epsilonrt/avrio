/*
 * Test de la couche port AVR de MBMaster
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

  if (bTurnOn) {
  
    vLedSet (ubIdx);
  }
  else {
  
    vLedClear (ubIdx);
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
