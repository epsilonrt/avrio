#include <avrio/led.h>
#include <avrio/switch.h>
#include <avrio/delay.h>

int
main (void) {
  xSwitch  xPrev = 0;
  xSwitch  xCurrent;

  vLedInit ();
  vSwitchInit ();

  for (;;) {

    xCurrent = xSwitchGet();
    if (xCurrent != xPrev) {

      xPrev = xCurrent;
      while (xCurrent--) {
        // La LED clignote le nombre de fois correspondant à la valeur des switchs
        vLedSet (LED_LED1);
        delay_ms (200);
        vLedClear (LED_LED1);
        delay_ms (200);
      }
    }
  }
  return 0;
}
