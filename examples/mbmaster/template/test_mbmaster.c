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
