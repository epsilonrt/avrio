#include <avrio/led.h>
#include <avrio/task.h>

/* private variables ======================================================== */
static volatile bool xLedEnabled = true;

/* private functions ======================================================== */
void vAssert (bool bTestIsTrue);
void vBlink (xTaskHandle t);
void vToggle (xTaskHandle t);

/* internal public functions ================================================ */
int
main (void) {
  int i;
  xTaskHandle t1, t2;

  vLedInit ();

  for (;;) {

    t1 = xTaskCreate (xTaskConvertMs (200), vBlink);
    vAssert (t1 == 1);

    vTaskStart (t1);
    for (i = 0; i < 20; i++) {
      /* Bascule l'état de la LED chaque seconde */
      delay_ms (1000);
      xLedEnabled = !xLedEnabled;
    }
    vTaskDelete (t1);
    delay_ms (2000);

    t2 = xTaskCreate (xTaskConvertMs (100), vBlink);
    vAssert (t2 == 1);
    vTaskStart (t2);
    for (i = 0; i < 20; i++) {
      /* Bascule l'état de la LED chaque seconde */
      delay_ms (1000);
      xLedEnabled = !xLedEnabled;
    }
    vTaskDelete (t2);
    delay_ms (2000);

    t1 = xTaskCreate (xTaskConvertMs (1000), vToggle);
    vAssert (t1 == 1);
    t2 = xTaskCreate (xTaskConvertMs (50), vBlink);
    vTaskStart (t1);
    vTaskStart (t2);
    vAssert (t2 == 2);
    delay_ms (20000);
    vTaskDelete (t1);
    vTaskDelete (t2);
  }
  return 0;
}

/*
 * Fonction exécutée sous interruption toutes les 50 ms
 * Génère un signal rectangulaire de période 100 ms si la LED est validée
 */
void
vBlink (xTaskHandle t) {

  if (xLedEnabled) {

    vLedToggle (LED_LED1);
  }
  else {

    vLedClear (LED_LED1);
  }
  vTaskStart (t);
}

// -----------------------------------------------------------------------------
void vToggle (xTaskHandle t) {

  xLedEnabled = !xLedEnabled;
  vTaskStart (t);
}

// -----------------------------------------------------------------------------
void
vAssert (bool bTestIsTrue) {

  if (bTestIsTrue != true) {

    for (;;) {

      vLedSet (LED_LED1);
      delay_ms (25);
      vLedClear (LED_LED1);
      delay_ms (250);
    }
  }
}

/* ========================================================================== */
