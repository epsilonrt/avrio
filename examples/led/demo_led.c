/*
 * Exemple LED: Clignotement LED1
 */
#include <avrio/led.h>
#include <avrio/delay.h>

int
main (void) {

  vLedInit ();

  for (;;) {

    vLedToggle (LED_LED1);
    delay_ms (1000);
  }
  return 0;
}
