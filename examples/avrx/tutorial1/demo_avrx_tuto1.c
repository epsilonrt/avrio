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
 * @file demo_avrx_tuto1.c
 * @brief Utilisation des timers pour faire clignoter plusieurs LED
 *
 * Inspiré par le fichier original timers.c
 *
 * Revision History ------------------------------------------------------------
 *    20110918 - Initial version by epsilonRT
 */
#include <avrio/led.h>
#include <avrio/delay.h>
#include <avrio/avrx.h>

/* private variables ======================================================== */
/*
 * Les objets partagés par le noyau et les tâches ne doivent pas être des
 * variables automatiques mais des variables globales (static).
 */

/* internal public functions ================================================ */

/* -----------------------------------------------------------------------------
 * La tâche 1 fait clicnoter avec une période de 100 ms et un rapport 
 * cyclique de 20%
 * Paramètres de définition de la tâche :
 * nom : task 1
 * taille du stack : 8 bytes
 * priorité : 2
 */
AVRX_TASK (task1, 8, 2) {
  static xTimer xLed1Timer;

  for (;;) {
  
    vAvrXStartTimer (&xLed1Timer, xDelayMsToTicks(80));
    vAvrXWaitTimer (&xLed1Timer);
    vLedToggle (LED_LED1);
    
    vAvrXStartTimer (&xLed1Timer, xDelayMsToTicks(20));
    vAvrXWaitTimer (&xLed1Timer);
    vLedToggle (LED_LED1);
  }
}

/* -----------------------------------------------------------------------------
 * La tâche 2 fait clignoter avec une période de 400 ms
 */
AVRX_TASK (task2, 8, 3) {
  static xTimer xLed2Timer;

  for (;;) {
  
    vAvrXDelay (&xLed2Timer, xDelayMsToTicks(200));
    vLedToggle (LED_LED2);
  }
}

/* -----------------------------------------------------------------------------
 * La tâche 3 fait clignoter avec une période de 1000 ms
 */
AVRX_TASK (task3, 8, 4) {
  static xTimer xLed3Timer;

  for (;;) {
  
    vAvrXDelay (&xLed3Timer, xDelayMsToTicks(500));
    vLedToggle (LED_LED3);
  }
}

// -----------------------------------------------------------------------------
int
main (void){

  vLedInit();

  vAvrXRunTask (task1);
  vAvrXRunTask (task2);
  vAvrXRunTask (task3);

  vAvrXStart ();
}

/* ========================================================================== */
