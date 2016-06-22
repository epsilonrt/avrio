#include <avrio/gpio.h>
#include <avrio/delay.h>

// -----------------------------------------------------------------------------
void 
vButtonCB (uGpioMask uPinValues, uint8_t ucPin, eDpEdge edge, void *udata) {
  
  vGpioWrite (ucPin - BUTC, (edge == eEdgeRising) ? true : false);
}

/* main ===================================================================== */
int
main (void) {
  uint8_t i;

  vGpioSetCallback(vButtonCB);
  for (i = BUTC; i <= BUTS; i++) {

    vGpioSetPinEdge (i, eEdgeBoth);
  }

  for (i = 0; i < 8; i++) {

    vGpioToggleAll(-1);
    delay_ms (200);
  }

  for (;;) {
    
    vGpioPoll();
  }
  return 0;
}
/* ========================================================================== */
