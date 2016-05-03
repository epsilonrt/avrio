/**
 * Copyright © 2011-2015 epsilonRT. All rights reserved.
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
 * @file test_wusb_sensor_database.c
 * @brief Test unitaire capteur WirelessUSB
 *
 * Revision History ------------------------------------------------------------
 *    20111121 - Initial version by epsilonRT
 */
#include <avrio/led.h>
#include <avrio/delay.h>
#include <avrio/wusb/hub/sensor_database.h>

/* constants ================================================================ */
#define TEST_LED LED_LED1
#define DBASE_SIZE  16
#define BUFFER_SIZE WSDBASE_SENSOR_FILE_SIZE(DBASE_SIZE)
#define FIRST_MID 0x12345678
#define FIRST_DATA  0x41
#define DATA_SIZE WUSB_CONFIG_HUB_BACKDATA_SIZE
#define FLAGS _BV(WSDBASE_VALID_DID)

/* private variables ======================================================== */
static uint8_t ucSensorBuffer[BUFFER_SIZE];
static uint8_t ucDataIn[DATA_SIZE];
static uint8_t ucDataOut[DATA_SIZE];

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
  for (ucCount = 0; ucCount < DATA_SIZE; ucCount++) {
  
    ucDataIn[ucCount] = FIRST_DATA + ucCount;
  }
  
  
  vAssert(iWSdBaseOpen(BUFFER_SIZE, ucSensorBuffer) == 0);

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
