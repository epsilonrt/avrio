/**
 * @file demo_mbmaster.c
 * @brief MBMaster Serial RTU demonstration
 * @author Copyright _ 2009 Christian Walter <cwalter@embedded-solutions.at>
 *         All rights reserved.
 * @author Copyright _ 2012 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * Revision History ------------------------------------------------------------
 *    20120604 - Initial version by epsilonRT
 */
#include <avr/interrupt.h>
#include <avrio/delay.h>
#include <avrio/led.h>
#include <avrio/button.h>
#include "mbport.h"
#include "mbm.h"
#include "common/mbportlayer.h"

/* constants ================================================================ */
#define DEMO_PORT           (0) 
#define DEMO_BAUDRATE       (38400) 
#define DEMO_PARITY         (MB_PAR_NONE) 
#define DEMO_SLAVE_ADDRESS (32)
#define DEMO_WAIT_BUTTON

/* private variables ======================================================== */
static eMBErrorCode    eStatus;

/* private functions ======================================================== */

// -----------------------------------------------------------------------------
static void
vAssert (bool bTestIsTrue) {

  if (bTestIsTrue != true) {

    for (;;) {

      vLedSet (LED_LED1);
      delay_ms (50);
      vLedClear (LED_LED1);
      delay_ms (150);
    }
  }
}

// -----------------------------------------------------------------------------
static void 
vWait (void) {

  vAssert (eStatus == MB_ENOERR);
  vLedToggle (LED_LED1);
#ifdef DEMO_WAIT_BUTTON
  while (xButGet (BUTTON_BUTTON1) == 0) {

  }
  delay_ms(200);
#else
  delay_ms (5000) ;
#endif
}

/* internal public functions ================================================ */
int
main (void) {
  xMBHandle       xMBMMaster;
  USHORT          usNRegs[5];
  USHORT          usRegCnt = 0;
  USHORT          usReg;
  UBYTE           ubIdx;

  vLedInit();
#ifdef DEMO_WAIT_BUTTON
  vButInit();
#endif

  eStatus = eMBMSerialInit (&xMBMMaster, MB_RTU, DEMO_PORT, 
                                         DEMO_BAUDRATE, DEMO_PARITY);
  vAssert (eStatus == MB_ENOERR);
  
  sei () ;
  
  for(;;) {

    eStatus = MB_ENOERR;
    
    /* Write an incrementing counter to register address 0. */
    eStatus = eMBMWriteSingleRegister (xMBMMaster, DEMO_SLAVE_ADDRESS, 
                                        0, usRegCnt);
    vWait ();
    
    /* Read holding register from adress 0 and check ... */
    eStatus = eMBMReadHoldingRegisters (xMBMMaster, DEMO_SLAVE_ADDRESS, 
                                         0, 1, &usReg);
    vAssert (usRegCnt == usReg);
    vWait ();

    usRegCnt++;

    /* Read holding register from adress 5 - 10 ... */
    eStatus = eMBMReadHoldingRegisters (xMBMMaster, DEMO_SLAVE_ADDRESS, 
                                         5, 5, usNRegs);
    vWait ();

    /* increment them by one ... */
    for (ubIdx = 0; ubIdx < 5; ubIdx++) {
    
        usNRegs[ubIdx]++;
    }
    
    /* and store them at address 10. */
    eStatus = eMBMWriteMultipleRegisters (xMBMMaster, DEMO_SLAVE_ADDRESS, 
                                           10, 5, usNRegs);
    vWait ();

    /* Read the input registers from address 2 - 5 and write them to the holding
     * registers at address 1 - 4.
     */
    eStatus = eMBMReadInputRegisters (xMBMMaster, DEMO_SLAVE_ADDRESS,
                                       1, 4, usNRegs);
    vWait ();
    
    eStatus = eMBMWriteMultipleRegisters (xMBMMaster, DEMO_SLAVE_ADDRESS,
                                           1, 4, usNRegs);
    vWait ();
  }

  return 0;
}
/* ========================================================================== */
