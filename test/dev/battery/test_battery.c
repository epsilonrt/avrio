/**
 * @file test_battery.c
 * @brief Test unitaire Chargeur de batterie
 * @author Copyright _ 2011-2012 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * Revision History ------------------------------------------------------------
 *    20120303 - Initial version by epsilonRT
 */
#include <stdio.h>
#include <avr/pgmspace.h>
#include <avrio/led.h>
#include <avrio/delay.h>
#include <avrio/task.h>
#include <avrio/mutex.h>
#include <avrio/serial.h>
#include <avrio/term.h>
#include <avrio/battery.h>
#include "avrio-board-battery.h"

/* constants ================================================================ */
#define TEST_BAUDRATE 115200

#define MUTEX_LED 0x80
#define MUTEX_MON 0x40

#define MONITOR_INTERVAL     xTaskConvertMs(SECONDS(10))

#define LED_IDLE_INTERVAL    xTaskConvertMs(50)
#define LED_PRE_INTERVAL     xTaskConvertMs(500)
#define LED_TOPOFF_INTERVAL  xTaskConvertMs(250)
#define LED_FAST_INTERVAL    xTaskConvertMs(125)
#define LED_TRICKLE_INTERVAL xTaskConvertMs(50)
#define LED_ERROR_INTERVAL   xTaskConvertMs(250)
#define LED_ERROR_PULSE      (250 / 8)

/* private variables ======================================================== */
static xMutex xSyncMutex;

/* internal public functions ================================================ */
// -----------------------------------------------------------------------------
void
vPrintCurrentCharge (void) {

  switch (eBatChargeGet()) {
  
    case BAT_CHARGE_OFF:
      printf_P (PSTR("Off"));
      break;
    case BAT_CHARGE_TOPOFF:
      printf_P (PSTR("Top-Off"));
      break;
    case BAT_CHARGE_TRICKLE:
      printf_P (PSTR("Trickle"));
      break;
    case BAT_CHARGE_FAST:
      printf_P (PSTR("Fast"));
      break;
  }
}

// -----------------------------------------------------------------------------
void
vPrintState (eBatState eState) {

  switch (eState) {
  
    case BAT_STATE_LOW:
      printf_P (PSTR("Low-Bat"));
      break;
    case BAT_STATE_IDLE:
      printf_P (PSTR("Idle"));
      break;
    case BAT_STATE_PUTON:
      printf_P (PSTR("Put-On"));
      break;
    case BAT_STATE_PRECHARGE:
      printf_P (PSTR("Pre-Charge"));
      break;
    case BAT_STATE_TOPOFF:
      printf_P (PSTR("Top-Off"));
      break;
    case BAT_STATE_TRICKLE:
      printf_P (PSTR("Trickle"));
      break;
    case BAT_STATE_FAST:
      printf_P (PSTR("Fast"));
      break;
    case BAT_STATE_ERROR:
      printf_P (PSTR("Error"));
      break;
  }
}

// -----------------------------------------------------------------------------
void 
vPrintHeader (void) {

  printf_P (PSTR("\nMode\tVBav\tVBmax\n"));
}

// -----------------------------------------------------------------------------
static void 
prvvTaskLed (xTaskHandle xTaskLed) {

  vMutexUnlockBit (&xSyncMutex, MUTEX_LED);
  vTaskStart (xTaskLed);
}

// -----------------------------------------------------------------------------
static void 
prvvTaskMonitor (xTaskHandle xTaskMonitor) {

  vMutexUnlockBit (&xSyncMutex, MUTEX_MON);
  vTaskStart (xTaskMonitor);
}

/* main ===================================================================== */
int
main (void) {
  eBatState eCurrentState, ePreviousState = -1;
  xTaskHandle xTaskMonitor = 0, xTaskLed = 0;

  vLedInit();
  vBatInit();
  vSerialInit (TEST_BAUDRATE / 100, SERIAL_DEFAULT + SERIAL_RW);
  stdout = &xSerialPort;
  printf_P (PSTR("\nTest unitaire Chargeur batterie\n"));
  
  xTaskMonitor = xTaskCreate (MONITOR_INTERVAL , prvvTaskMonitor);
  vTaskStart (xTaskMonitor);
  xTaskLed     = xTaskCreate (LED_IDLE_INTERVAL, prvvTaskLed);
  vTaskStart (xTaskLed);
  
  vPrintHeader();
  
  for (;;) {

    eCurrentState = eBatProcess();
    if (ePreviousState != eCurrentState) {
    
      ePreviousState = eCurrentState;
      printf_P (PSTR("------- Process State Changed ----> "));
      vPrintState (eCurrentState);
      putchar ('\n');
    }

    if (xMutexTryLockBit(&xSyncMutex, MUTEX_LED) == 0) {
      uint8_t ucCount;
      
      switch (eCurrentState) {
      
        case BAT_STATE_LOW:
        case BAT_STATE_IDLE:
          vLedClear (LED_LED1);
          vTaskSetInterval (xTaskLed, LED_IDLE_INTERVAL);
          break;
          
        case BAT_STATE_PUTON:
        case BAT_STATE_PRECHARGE:
          vLedToggle (LED_LED1);
          vTaskSetInterval (xTaskLed, LED_PRE_INTERVAL);
          break;
          
        case BAT_STATE_FAST:
          vLedToggle (LED_LED1);
          vTaskSetInterval (xTaskLed, LED_FAST_INTERVAL);
          break;
          
        case BAT_STATE_TOPOFF:
          vLedToggle (LED_LED1);
          vTaskSetInterval (xTaskLed, LED_TOPOFF_INTERVAL);
          break;
          
        case BAT_STATE_TRICKLE:
          vLedSet (LED_LED1);
          vTaskSetInterval (xTaskLed, LED_TRICKLE_INTERVAL);
          break;
          
        case BAT_STATE_ERROR:
          for (ucCount = 0; ucCount < 4; ucCount++) {
          
            vLedToggle (LED_LED1);
            delay_ms (LED_ERROR_PULSE);            
          }
          vTaskSetInterval (xTaskLed, LED_ERROR_INTERVAL);
          break;
      }
    }
    
    if (xMutexTryLockBit(&xSyncMutex, MUTEX_MON) == 0) {

      vPrintCurrentCharge();
      printf_P (PSTR("\t%.3f\t%.3f\n"), 
                ADC_MEASUREMENT(usBatVoltageAverage(),  BAT_SCALE),
                ADC_MEASUREMENT(usBatVoltageMax(),  BAT_SCALE));
    }
  }
  return 0;
}
/* ========================================================================== */
