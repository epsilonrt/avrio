#include <avrio/led.h>
#include <avrio/button.h>
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
xMessage xSwitchUpMsg, xSwitchDownMsg;
xMessageQueue xMsgQueue;

/* internal public functions ================================================ */

/* -----------------------------------------------------------------------------
 * La tâche 1 allume la LED1 sur réception du message xSwitchDownMsg et
 * l'éteind sur réception du message xSwitchUpMsg.
 */
AVRX_TASK (vTask1, 10, 3) {
  xMessage * pxMsg;

  for (;;) {
  
    pxMsg = pxAvrXWaitMessage (&xMsgQueue);
    if (pxMsg == &xSwitchUpMsg) {
    
      vLedClear (LED_LED1);
    }
    else if (pxMsg == &xSwitchDownMsg) {
    
      vLedSet (LED_LED1);
    }
    else {
      
      vAvrXHalt ();
    }
    vAvrXAckMessage (pxMsg);
  }
}

/* -----------------------------------------------------------------------------
 * La tâche 2 envoie les messages en fonction de l'état du bouton poussoir
 */
AVRX_TASK (vTask2, 10, 3) {
  xButMask xCurrent, xPrevious;

  xPrevious = xButGet(BUTTON_BUTTON1);
  
  for (;;) {
  
    xCurrent = xButGet(BUTTON_BUTTON1);
    if (xCurrent != xPrevious) {
      if (xCurrent & BUTTON_BUTTON1) {
      
        vAvrXSendMessage (&xMsgQueue, &xSwitchDownMsg);
        vAvrXWaitMessageAck (&xSwitchDownMsg);
      } 
      else {
      
        vAvrXSendMessage (&xMsgQueue, &xSwitchUpMsg);
        vAvrXWaitMessageAck (&xSwitchUpMsg);
      }
      xPrevious = xCurrent;
    }
  }
}

/* -----------------------------------------------------------------------------
 * La tâche 2 fait clignoter avec une période de 400 ms
 */
AVRX_TASK (vTask3, 8, 2) {
  static xTimer xLed3Timer;

  for (;;) {
  
    vAvrXDelay (&xLed3Timer, xDelayMsToTicks(200));
    vLedToggle (LED_LED3);
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
  vAvrXRunTask (vTask3);
  vAvrXRunTask (vDebugMonitor);

  vAvrXStart ();
}

/* ========================================================================== */
