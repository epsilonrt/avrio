/**
 * Copyright © 2011-2015 epsilonRT. All rights reserved.
 *
 * This file is part of AvrIO.
 *
 * This software is governed by the CeCILL license under French law and
 * abiding by the rules of distribution of free software.  You can  use, 
 * modify and/ or redistribute the software under the terms of the CeCILL
 * license as circulated by CEA, CNRS and INRIA at the following URL
 * <http://www.cecill.info>. 
 * 
 * The fact that you are presently reading this means that you have had
 * knowledge of the CeCILL license and that you accept its terms.
 *
 * @file test_wusb_hub.c
 * @brief Test unitaire Hub WirelessUSB
 *
 * Revision History ------------------------------------------------------------
 *    20111211 - Initial version by epsilonRT
 */
#include <avrio/led.h>
#include <avrio/delay.h>
#include <avrio/twi.h>
#include <avrio/serial.h>
#include <avrio/wusb/hub.h>

/* constants ================================================================ */
#define FRAM_SLAVE_ADDR  0xA0
#define FRAM_SIZE        8192
#define LOOP_LED    LED_LED1
#define ERROR_LED   LED_LED2
#define TEST_BAUDRATE 115200

/* private variables ======================================================== */
// static uint8_t pSensorDbase[WSDBASE_SENSOR_FILE_SIZE(3)];
static xTwiFrame xFRAM = { .xAddr = FRAM_SLAVE_ADDR };
static const xWHubMessage * pxMsg;

/* private functions ======================================================== */
// ------------------------------------------------------------------------------
static void
vAssert (bool test) {

  if (test == false) {

     for (;;) {

      vLedSet (ERROR_LED);
      delay_ms (50);
      vLedClear (ERROR_LED);
      delay_ms (500);
    }
  }
}

/* main ===================================================================== */
int
main (void) {

  vLedInit ();
#if defined(AVRIO_DEBUG_STREAM)
  /* Init terminal */
  vSerialInit (TEST_BAUDRATE / 100, SERIAL_DEFAULT + SERIAL_WR);
  stderr = &xSerialPort;
  fputc('\r', stderr);
#endif
  vTwiInit ();
  vAssert(eTwiSetSpeed (400) == TWI_SUCCESS);
  vAssert(iWHubInit (WDEV_RATE_16KBPS, FRAM_SIZE, &xFRAM) == 0);
  vWHubSetStatusFlag (WHUB_AUTOBIND, false);
  // vWSdBaseClear ();

  for (;;) {

    vLedSet (LOOP_LED);
    pxMsg = pxWHubLoop ();
    vLedClear (LOOP_LED);
  }

  return 0;
}

/* ========================================================================== */
