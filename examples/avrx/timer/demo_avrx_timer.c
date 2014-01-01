/**
 * @file demo_avrx_timer.c
 * @author Copyright © 2011-2012 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * @version $Id$
 * Revision History ---
 *    20110907 - Initial version by epsilonRT
 * ---
 * @brief Utilisation des timers AvrX
 *
 * Utilisation des timers pour faire clignoter plusieurs LED.
 */
#include <avrio/led.h>
#include <avrio/delay.h>
#include <avrio/serial.h>
#include <avrio/avrx.h>
#include <avrx/debug.h>

/* constants ================================================================ */
#define BAUDRATE 9600

/* private variables ======================================================== */
/*
 * Les objets partagés par le noyau et les tâches ne doivent pas être des
 * variables automatiques mais des variables globales (static).
 */

/* internal public functions ================================================ */

/* -----------------------------------------------------------------------------
 * La tâche 1 fait clicnoter avec une période de 100 ms et un rapport 
 * cyclique de 20%
 */
AVRX_TASK (vTask1, 8, 2) {
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
AVRX_TASK (vTask2, 8, 2) {
  static xTimer xLed2Timer;

  for (;;) {
  
    vAvrXDelay (&xLed2Timer, xDelayMsToTicks(200));
    vLedToggle (LED_LED2);
  }
}

/* -----------------------------------------------------------------------------
 * La dernière tâche est le moniteur de debugage
 */
AVRX_DECLARE_TASK (vDebugMonitor, 20, 1);

// -----------------------------------------------------------------------------
int
main (void){

  vLedInit();
  vDebugInit (SERIAL_BAUD_X1(BAUDRATE / 100L));

  vAvrXRunTask (vTask1);
  vAvrXRunTask (vTask2);
  vAvrXRunTask (vDebugMonitor);

  vAvrXStart ();
}

/* ========================================================================== */
