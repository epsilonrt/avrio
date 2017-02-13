#include <avrio/led.h>
#include <avrio/delay.h>

/* constants ================================================================ */
/* macros =================================================================== */
/* structures =============================================================== */
/* types ==================================================================== */
/* private variables ======================================================== */
/* public variables ========================================================= */
/* private functions ======================================================== */
/* internal public functions ================================================ */

/* main ===================================================================== */
int
main (void) {

  vLedInit ();
  // Votre code d'initialisation devrait être ici
  // Your initialization code should be here

  for (;;) {

    // Your main code should be here,
    // you can delete the two lines below
    // Votre code principal devrait être ici,
    // Vous pouvrez supprimer les deux lignes ci-dessous
    vLedToggle (LED_LED1);
    delay_ms (1000);
  }
  return 0;
}

/* ========================================================================== */
