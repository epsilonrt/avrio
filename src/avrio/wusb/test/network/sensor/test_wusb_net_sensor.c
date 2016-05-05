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
 * @file test_wusb_net_sensor.c
 * @brief Test unitaire du réseau Nto1 WirelessUSB
 *
 * Revision History ------------------------------------------------------------
 *    20111113 - Initial version by epsilonRT
 */
#include <avrio/led.h>
#include <avrio/delay.h>
#include <avrio/serial.h>
#include <avrio/term.h>
#include <avrio/wusb/net/network.h>

/* constants ================================================================ */
#define TEST_BAUDRATE 115200
#define TEST_LED LED_LED1

/* private variables ======================================================== */
static xWNetTestResult xTestResult;

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

#if defined(WNET_DEBUG_ONUART)
static void
prvvDbgInit (void) {

  /* Init terminal */
  vSerialInit (TEST_BAUDRATE / 100, SERIAL_DEFAULT + SERIAL_WR);
  stderr = &xSerialPort;
  fputc('\r', stderr);
}

static void
prvvPrintResult (const xWNetTestResult * pxResult) {

  fputs(  "\rSend: ", stderr);
  vTermPutDec (pxResult->usSended, stderr);
  fputs( " Recv: ", stderr);
  vTermPutDec (pxResult->usReceived, stderr);
  fputs( " Lost: ", stderr);
  vTermPutDec (pxResult->usSended - pxResult->usReceived, stderr);
  fputs( " Bad : ", stderr);
  vTermPutDec (pxResult->usCorrupted, stderr);
  fputc('\r', stderr);
}
#else
# define prvvDbgInit()
# define prvvPrintResult(r)
#endif

/* internal public functions ================================================ */
int
main (void) {
  eWNetErrorCode eError;
  
  vLedInit ();
  prvvDbgInit ();

  vAssert (iWNetTestSetup () == 0);
  
  (void) xWNetSetChannel (WNET_BIND_CHANNEL);
  (void) xWNetSetPnCode (WNET_BIND_PNCODE);
  vWNetSetCrcSeed (WPKT_BIND_CRC_SEED);
  vWNetSetChecksumSeed (WPKT_BIND_CHECKSUM_SEED);

  do {
  
    eError = eWNetTestSensorBind (10, 100, &xTestResult);
    prvvPrintResult (&xTestResult);
  } while (eError < 0);
    
  for (;;) {
    
    eError = eWNetTestSensorPing (1000, 10, &xTestResult);
    prvvPrintResult (&xTestResult);
    vLedToggle (TEST_LED);
  }

  return 0;
}

/* ========================================================================== */
