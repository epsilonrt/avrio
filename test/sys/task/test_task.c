/**
 * Copyright © 2011-2015 Pascal JEAN aka epsilonRT. All rights reserved.
 *
 * This file is part of AvrIO.
 *
 * AvrIO is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * AvrIO is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with AvrIO.  If not, see <http://www.gnu.org/licenses/lgpl.html>
 *
 * @file test_task.c
 * @brief Test unitaire du module \ref task_module
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
