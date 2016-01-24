#include <avr/pgmspace.h>
#include <avrio/lcd_backlight.h>
#include <avrio/delay.h>
#include <avrio/led.h>
#include <avrio/button.h>

/* main ===================================================================== */
int
main (void) {
  int8_t iLevel = 50;
  
  vLedInit();
  vButInit();
  vLcdCtrlBacklightInit ();
  iLcdBacklightSet (iLevel);
  vLedSet (LED_LED1);

  for (;;) {

    if (xButGet(BUTTON_BUTTON1)) {
    
      if (iLevel < 100) {
      
        iLevel += 10;
      }
      else {
      
        iLevel = 0;
      }
      iLcdBacklightSet (iLevel);
      vLedToggle (LED_LED1);
      delay_ms(150);
    }
  }
  return 0;
}
/* ========================================================================== */
