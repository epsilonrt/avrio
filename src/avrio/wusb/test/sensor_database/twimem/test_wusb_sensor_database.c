/**
 * @file test_wusb_sensor_database.c
 * @brief Test unitaire capteur WirelessUSB
 * @author Copyright _ 2011-2012 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * Revision History ------------------------------------------------------------
 *    20111121 - Initial version by epsilonRT
 */
#include <avrio/led.h>
#include <avrio/delay.h>
#include <avrio/twi.h>
#include <avrio/wusb/hub/sensor_database.h>

/* constants ================================================================ */
#define SLAVE_ADDR  0xA0
#define MEM_SIZE    8192
#define DBASE_SIZE WSDBASE_SENSOR_DBASE_SIZE(MEM_SIZE)
#define TEST_LED LED_LED1
#define FIRST_MID 0x12345678
#define FIRST_DATA  0x41
#define DATA_SIZE WUSB_CONFIG_HUB_BACKDATA_SIZE
#define FLAGS _BV(WSDBASE_VALID_DID)

/* private variables ======================================================== */
static uint8_t ucDataIn[DATA_SIZE];
static uint8_t ucDataOut[DATA_SIZE];
static xTwiFrame xFrame;

/* private functions ======================================================== */
// ------------------------------------------------------------------------------
static void
vAssert (bool test) {

  if (test == false) {

     for (;;) {

      vLedSet (TEST_LED);
      delay_ms (50);
      vLedClear (TEST_LED);
      delay_ms (500);
    }
  }
}

/* main ===================================================================== */
int
main (void) {
  uint32_t ulMid;
  uint8_t ucCount;
  uint16_t usDid, usCount;
  
  vLedInit ();
  vTwiInit ();
  vAssert(eTwiSetSpeed (400) == TWI_SUCCESS);
  
  for (ucCount = 0; ucCount < DATA_SIZE; ucCount++) {
  
    ucDataIn[ucCount] = FIRST_DATA + ucCount;
  }
  
  
  xFrame.xAddr = SLAVE_ADDR;
  vAssert(iWSdBaseOpen(MEM_SIZE, &xFrame) == 0);

  for (;;) {

    vWSdBaseClear();
    vAssert (usWSdBaseSize() == DBASE_SIZE);
    vAssert (usWSdBaseLength() == 0);
    for ( ulMid = FIRST_MID, usCount = 0; 
          ulMid < (DBASE_SIZE + FIRST_MID); 
          ulMid++, usCount++) {

      vLedClear (TEST_LED);
      usDid = usWSdBaseAddSensor (ulMid);
      vLedSet (TEST_LED);
      vAssert (usDid == usCount);
      vAssert (ulWSdBaseSensorMid(usDid) == ulMid);
      vWSdBaseSensorSetFlag (usDid, FLAGS);
      vAssert (ucWSdBaseSensorFlag(usDid) == FLAGS);
      vWSdBaseSensorSetBackData (usDid, ucDataIn, DATA_SIZE);
      vAssert (ucWSdBaseSensorBackData(usDid, ucDataOut) == DATA_SIZE);
      vAssert (ucWSdBaseSensorBackDataLength(usDid) == DATA_SIZE);
      vAssert (memcmp(ucDataIn, ucDataOut, DATA_SIZE) == 0);
      vWSdBaseFindNextFreeDeviceId();
    }
    vAssert (usWSdBaseSize() == DBASE_SIZE);
    vAssert (usWSdBaseLength() == DBASE_SIZE);
  }
  return 0;
}
/* ========================================================================== */
