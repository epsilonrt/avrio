/**
 * @file test_wusb_packet.c
 * @brief Test unitaire d'un paquet WirelessUSB CYWUSB6935
 * @author Copyright _ 2011-2012 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * @version $Id$
 * Revision History ---
 *    20111113 - Initial version by epsilonRT
 */
#include <avrio/led.h>
#include <avrio/delay.h>
#include <avrio/wusb/net/packet.h>

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

  for (;;) {
  
    vAssert (iWPktTest () == 0);
    vLedToggle (LED_LED1);
    delay_ms (1000);
  }

  return 0;
}

/* ========================================================================== */
