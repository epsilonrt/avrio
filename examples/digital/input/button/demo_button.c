/**
 * @brief Demo port GPIO
 *
 * Ce programme fait clignoter la LED1 16 fois puis lit l'état du bouton
 * poussoir et bascule l'état de la LED1 à chaque appui.
 *
 * Il est prévu pour une carte Arduino UNO, le fichier avrio-board-button.h dans
 * le répertoire courant devra être adapté pour une autre carte.
 */
#include <avrio/led.h>
#include <avrio/button.h>
#include <avrio/delay.h>

/* main ===================================================================== */
int
main (void) {

  vLedInit ();
  vButInit ();

  for (int i = 0; i < 16; i++) {

    vLedSet (LED_LED1); // on allume la led
    delay_ms (50); // pendant 50ms
    vLedClear (LED_LED1); // puis on l'éteint
    delay_ms (150); // pendant 150ms
  }

  for (;;) {

    if (xButGet (BUTTON_BUTTON1)) {

      // Toggle: Basculer en anglais
      vLedToggle (LED_LED1);
      delay_ms (200);
    }
  }
  return 0;
}
/* ========================================================================== */
