/**
 * @brief Demo LED
 *
 * Ce programme fait clignoter la LED1 (L sur Arduino).
 *
 * Il est prévu pour une carte Arduino UNO, le fichier avrio-board-led.h dans
 * le répertoire courant devra être adapté pour une autre carte.
 */
#include <avrio/led.h>
#include <avrio/delay.h>

int
main (void) {

  vLedInit ();

  for (;;) {

    
    // 1ère solution ...
    for (int i = 0; i < 16; i++) {
      
      vLedSet (LED_LED1); // on allume la led
      delay_ms (250); // pendant 250ms
      vLedClear (LED_LED1); // puis on l'éteint
      delay_ms (750); // pendant 750ms
    }
    
    delay_ms (1000); // pause

    // 2ème solution ...
    for (int i = 0; i < 16; i++) {
      
      vLedToggle (LED_LED1); // on bascule l'état de la led
      delay_ms (500); // toutes les 500ms
    }
  }
  return 0;
}
