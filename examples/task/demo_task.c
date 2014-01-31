/**
 * @file demo_task.c
 * @brief Démonstration du module Task
 * @author Copyright © 2011-2012 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * @version $Id$
 */
#include <avrio/led.h>
#include <avrio/task.h>

/* private variables ======================================================== */
static volatile bool xLedEnabled = true;

/* private functions ======================================================== */
/* 
 * Fonction exécutée sous interruption toutes les 50 ms
 * Génère un signal rectangulaire de période 100 ms si la LED est validée
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
