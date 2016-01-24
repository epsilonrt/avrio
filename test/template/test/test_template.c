#include <avrio/led.h>
#include <avrio/delay.h>

/* constants ================================================================ */
/* macros =================================================================== */
/* structures =============================================================== */
/* types ==================================================================== */
/* private variables ======================================================== */
/* public variables ========================================================= */

/* private functions ======================================================== */
// -----------------------------------------------------------------------------
static void
vAssert (bool test) {

  if (test == false) {

    for (;;) {

      vLedSet (LED_LED1);
      delay_ms (50);
      vLedClear (LED_LED1);
      delay_ms (150);
    }
  }
}


/* internal public functions ================================================ */

/* main ===================================================================== */
int
main (void) {

  vLedInit ();

  for (;;) {

    // Test 1: ...
    
    // Fin des tests
    vLedToggle (LED_LED1);
    delay_ms (1000);
  }
  return 0;
}

/* ========================================================================== */
