/**
 * @file test_wusb_sensor.c
 * @brief Test unitaire capteur WirelessUSB
 * @author Copyright _ 2011-2012 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * Revision History ------------------------------------------------------------
 *    20111121 - Initial version by epsilonRT
 */
#include <avrio/led.h>
#include <avrio/delay.h>
#include <avrio/wusb/sensor.h>

/* constants ================================================================ */
#define TEST_BAUDRATE 38400
#define TEST_LED LED_LED1

/* private variables ======================================================== */
static xWNetTestResult xTestResult;

/* private functions ======================================================== */
#if defined(AVRIO_DEBUG_STREAM)
#include <avrio/serial.h>
#include <avrio/debug.h>

// -----------------------------------------------------------------------------
static void
prvvDbgInit (void) {

  /* Init terminal */
  vSerialInit (TEST_BAUDRATE / 100, SERIAL_DEFAULT + SERIAL_WR);
  AVRIO_DEBUG_STREAM = &xSerialPort;
  vDbgNewLine();
}

// -----------------------------------------------------------------------------
static void
prvvPrintResult (const xWNetTestResult * pxResult) {

  vDbgPuts_P(PSTR("\rSend: "));
  vDbgPutDec (pxResult->usSended);
  vDbgPuts_P(PSTR( " Recv: "));
  vDbgPutDec (pxResult->usReceived);
  vDbgPuts_P(PSTR( " Lost: "));
  vDbgPutDec (pxResult->usSended - pxResult->usReceived);
  vDbgPuts_P(PSTR( " Bad : "));
  vDbgPutDec (pxResult->usCorrupted);
  vDbgNewLine();
}
#else
# define prvvDbgInit()
# define prvvPrintResult(r)
#endif

/* main ===================================================================== */
int
main (void) {
  int iError;

  vLedInit ();
  prvvDbgInit ();
  vWSensorInit (WDEV_RATE_16KBPS);

  for (;;) {

    vLedSet (TEST_LED);
    vWSensorBind (false);

    while (xWSensorBound () == true) {

      iError = iSensorTestPing (100, 500, &xTestResult);
      prvvPrintResult (&xTestResult);
      vLedToggle (TEST_LED);
    } /* Fin boucle Node Bound */
  } /* Fin boucle infinie */
  return 0;
}

/* internal public functions ================================================ */
void 
vWSensorSleepCB (eWSensorError eError) {
  
  vWSensorSleep (true);
  delay_ms (5000);
  vWSensorSleep (false);
}
/* ========================================================================== */
