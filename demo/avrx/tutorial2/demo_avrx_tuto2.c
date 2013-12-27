/**
 * @file demo_avrx_tuto2.c
 * @brief Utilisation de sémaphores pour synchroniser 2 tâches
 *
 * Une tâche est utilisée pour faire clignoter une LED, l'autre pour scruter 
 * une entrée. La LED change d'état à chaque appui sur le poussoir.
 * @author Copyright © 2011-2012 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * @version $Id$
 * Revision History ---
 *    20110918 - Initial version by epsilonRT
 */
#include <avrio/led.h>
#include <avrio/button.h>
#include <avrio/delay.h>
#include <avrio/avrx.h>

/* private variables ======================================================== */
/*
 * Les objets partagés par le noyau et les tâches ne doivent pas être des
 * variables automatiques mais des variables globales (static).
 */
static xSem   xButtonSem;

/* internal public functions ================================================ */

/* -----------------------------------------------------------------------------
 * La tâche 1 fait clignoter la LED1 avec une période de 400 ms
 */
AVRX_TASK (task1, 8, 2) {
  static xTimer xLed1Timer;

  for (;;) {
  
    vAvrXDelay (&xLed1Timer, xDelayMsToTicks(200));
    vLedToggle (LED_LED1);
  }
}

/* -----------------------------------------------------------------------------
 * La tâche 2 bascule la LED2 à chaque appui sur le bouton
 */
AVRX_TASK (task2, 8, 3) {

  for (;;) {
  
    vAvrXWaitSemaphore (&xButtonSem);
    vLedToggle (LED_LED2);
  }
}

/* -----------------------------------------------------------------------------
 * La tâche 3 surveille le bouton poussoir
 */
AVRX_TASK (task3, 8, 4) {

  for (;;) {

    if (xButGet(BUTTON_BUTTON1)) {
    
      vAvrXSetSemaphore (&xButtonSem);
      while(xButGet(BUTTON_BUTTON1)) {
      
        vLedSet (LED_LED3);
      }
      vLedClear (LED_LED3);
    }
  }
}

// -----------------------------------------------------------------------------
int
main (void){

  vLedInit();
  vButInit();

  vAvrXRunTask (task1);
  vAvrXRunTask (task2);
  vAvrXRunTask (task3);

  vAvrXStart ();
}

/* ========================================================================== */
