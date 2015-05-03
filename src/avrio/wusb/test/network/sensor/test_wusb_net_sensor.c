/**
 * @file test_wusb_net_sensor.c
 * @brief Test unitaire du réseau Nto1 WirelessUSB
 * @author Copyright © 2011-2012 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
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
