#include <avrio/irq.h>
#include <avrio/led.h>
#include <avrio/delay.h>

/* private variables ======================================================== */
static int iIrqCount[2];

/* private functions ======================================================== */
void
vIsr (int8_t i) {
  int * iCounter = (int *) pvIrqGetUserData (i);

  if (i == INT1) {

    switch (eIrqGetMode (i)) {
      case eIrqRising:
        if (bIrqReadPin (i)) {

          vLedToggle (LED_LED1);
        }
        break;
      case eIrqFalling:
        if (bIrqReadPin (i) == 0) {

          vLedToggle (LED_LED1);
        }
        break;
      case eIrqEdge:
        if (bIrqReadPin (i)) {

          vLedSet (LED_LED1);
        }
        else {
          vLedClear (LED_LED1);
        }
        break;
      case eIrqLowLevel:
        break;
    }
  }
  (*iCounter) ++;
}

/* main ===================================================================== */
int
main (void) {

  vLedInit ();
  vIrqSetUserData (INT0, &iIrqCount[INT0]);
  vIrqAttach (INT0, vIsr, eIrqRising);
  vIrqDisable (INT0);
  vIrqSetUserData (INT1, &iIrqCount[INT1]);
  vIrqAttach (INT1, vIsr, eIrqRising);
  sei();

  for (;;) {
    // Rien à faire
  }
  return 0;
}
/* ========================================================================== */
