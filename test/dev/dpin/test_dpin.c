#include <avrio/dpin.h>
#include <avrio/delay.h>

/* constants ================================================================ */
static xDPin xPa[8];
static xDPin xPe[4] = {
  { .port = &PORTE, .pin = 4 },
  { .port = &PORTE, .pin = 5 },
  { .port = &PORTE, .pin = 6 },
  { .port = &PORTE, .pin = 7 },
};

/* main ===================================================================== */
int
main (void) {
  bool bValue = true;

  for (uint8_t i = 0; i < 8; i++) {

    vDpInit (&xPa[i], &PORTA, i, eModeOutput);
  }

  for (uint8_t i = 0; i < 4; i++) {

    vDpSetMode (&xPe[i], eModeInputPullUp);
  }

  for (;;) {
    
    for (uint8_t j = 0; j < 8; j++) {
      
      for (uint8_t i = 0; i < 8; i++) {

        vDpWrite (&xPa[i], bValue);
      }
      bValue = !bValue;
    }
    
    for (uint8_t j = 0; j < 8; j++) {
      
      for (uint8_t i = 0; i < 4; i++) {

        bValue = bDpRead (&xPe[i]);
        vDpWrite (&xPa[i], bValue);
      }
    }    
  }
  return 0;
}
/* ========================================================================== */
