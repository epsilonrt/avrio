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
 * @file demo_avrx_message.c
 *
 * Revision History ------------------------------------------------------------
 *    20110907 - Initial version by epsilonRT
 * ---
 * @brief Utilisation des messages AvrX
 */
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
AVRX_TASK (vTask1, 20, 3) {
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
  volatile xButMask xCurrent, xPrevious;
/* -----------------------------------------------------------------------------
 * La tâche 2 envoie les messages en fonction de l'état du bouton poussoir
 */
AVRX_TASK (vTask2, 20, 4) {
  xPrevious = 0;
  
  for (;;) {
  
    xCurrent = xButGet(BUTTON_BUTTON1);

    if (xCurrent == xPrevious) {
    }
    else {

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
  vButInit();
  vDebugInit (SERIAL_BAUD_X1(BAUDRATE / 100L));

  vAvrXRunTask (vTask1);
  vAvrXRunTask (vTask2);
  vAvrXRunTask (vTask3);
  vAvrXRunTask (vDebugMonitor);

  vAvrXStart ();
}

/* ========================================================================== */
