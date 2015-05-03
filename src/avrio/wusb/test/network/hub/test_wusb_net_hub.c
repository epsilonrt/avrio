/**
 * @file test_wusb_ne_hub.c
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
#include <avrio/wusb/net/network.h>

/* constants ================================================================ */
#define TEST_BAUDRATE 115200

/* private functions ======================================================== */
// ------------------------------------------------------------------------------
static void
vAssert (bool test) {

  if (test == false) {

     for (;;) {

      vLedSet (LED_LED1);
      delay_ms (50);
      vLedClear (LED_LED1);
      delay_ms (500);
    }
  }
}


/* internal public functions ================================================ */
int
main (void) {

  vLedInit ();
#if defined(WNET_DEBUG_ONUART)
  /* Init terminal */
  vSerialInit (TEST_BAUDRATE / 100, SERIAL_DEFAULT + SERIAL_WR);
  stderr = &xSerialPort;
  fputc('\r', stderr);
#endif
  vAssert (iWNetTestSetup () == 0);
  (void) xWNetSetChannel (WNET_BIND_CHANNEL);
  (void) xWNetSetPnCode (WNET_BIND_PNCODE);
  vWNetSetCrcSeed (WPKT_BIND_CRC_SEED);
  vWNetSetChecksumSeed (WPKT_BIND_CHECKSUM_SEED);
  

  for (;;) {
    
    vAssert (eWNetTestHub(-1) == WNET_SUCCESS);
    vLedToggle (LED_LED1);
  }

  return 0;
}

/* ========================================================================== */
