/*
 * Démonstration du module Task
 */
#include <avrio/led.h>
#include <avrio/task.h>

/* private variables ======================================================== */
static volatile bool xLedEnabled = true;

/* private functions ======================================================== */
/*
 * Fonction exécutée sous interruption
 */
static void
vTaskLed (xTaskHandle xTaskLed) {

  if (xLedEnabled) {

    vLedToggle (LED_LED1);  /* bascule l'état de la LED */
  } else {

    vLedClear (LED_LED1); /* éteint la LED */
  }
  vTaskStart (xTaskLed);  /* redémarre le compteur pour 50 ms */
}

/* internal public functions ================================================ */
int
main (void) {
  xTaskHandle xTaskLed;

  vLedInit ();
  xTaskLed = xTaskCreate (xTaskConvertMs (50), vTaskLed);
  vTaskStart (xTaskLed);

  for (;;) {

    /* Bascule l'état de la LED chaque seconde */
    delay_ms (1000);
    xLedEnabled = !xLedEnabled;
  }
  return 0;
}

/* ========================================================================== */
