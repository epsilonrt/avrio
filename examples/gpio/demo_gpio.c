/**
 * Demo port GPIO
 *
 * Ce programme fait clignoter la LED1 16 fois puis lit l'état du bouton 
 * poussoir et bascule l'état de la LED1 à chaque appui.
 *
 * Il est prévu pour une carte Arduino UNO, le fichier avrio-board-gpio.h dans
 * le répertoire board devra être adapté pour une autre carte.
 */
#include <avrio/gpio.h>
#include <avrio/delay.h>

/* constants ================================================================ */
#define LED1    13
#define BUTTON  2

/* main ===================================================================== */
int
main (void) {

  vGpioSetPinMode (LED1, eModeOutput); // broche led en sortie
  vGpioSetPinMode (BUTTON, eModeInputPullUp); // broche bouton en entrée avec pull-up
  // avec le mode eModeInputPullUp, bGpioRead() retourne true si le signal est à 0

  // fait clignoter la LED1 16 fois
  for (int i = 0; i < 16; i++) {

    vGpioWrite (LED1, 1); // on allume la led
    delay_ms (50); // pendant 50ms
    vGpioWrite (LED1, 0); // puis on l'éteint
    delay_ms (150); // pendant 150ms
  }

  for (;;) {

    if (bGpioRead (BUTTON)) { // si bouton appuyé
      
      vGpioToggle (LED1); // on bascule la led
      delay_ms (200);
    }
  }
  return 0;
}
/* ========================================================================== */
