/**
 * @file test_avrx_monitor.c
 * @author Copyright _ 2011-2012 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * Revision History ------------------------------------------------------------
 *    20110907 - Initial version by epsilonRT
 * ---
 * @brief Test du moniteur pour le debug
 */
#include <avrio/led.h>
#include <avrio/avrx.h>
#include <avrio/serial.h>
#include <avrx/debug.h>

/* constants ================================================================ */
#define BAUDRATE 9600

/* private functions ======================================================== */
static void
prvvFillRamBlock (uint8_t * pucBlock, size_t xSize) {
  uint8_t ucValue = 0;
  
  while (xSize--) {
  
    *pucBlock++ = ucValue++;
  }
}

/* internal public functions ================================================ */
AVRX_TASK (task1, 20, 2) {
  
  for (;;) {
    
    vDebugMonitor ();
    vLedToggle (LED_LED1);
  }
}

// -----------------------------------------------------------------------------
int
main (void){

  vLedInit();
  vDebugInit (SERIAL_BAUD_X1(BAUDRATE / 100L));
  prvvFillRamBlock ((uint8_t *)0x200, 0x100);

  vAvrXRunTask (task1);
  vAvrXStart ();
}

/* ========================================================================== */
