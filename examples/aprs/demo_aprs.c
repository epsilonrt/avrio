/**
 * @file demo_aprs.c
 * @brief Arduino APRS radio demo.
 *
 * This example shows how to read and decode APRS radio packets.
 * It uses the following modules:
 * afsk
 * ax25
 * serial
 *
 * You will see how to use a serial port to output messages, init the afsk demodulator and
 * how to parse input messages using ax25 module.
 *          @copyright 2009 GNU General Public License version 2
 *          See the notice below.
 * @author Francesco Sacchi <batt@develer.com>
 * @author Luca Ottaviano <lottaviano@develer.com>
 * @author Daniele Basile <asterix@develer.com>
 *
 * @author Pascal JEAN <pjean@btssn.net>
 *          @copyright 2014 GNU Lesser General Public License version 3
 *          <http://www.gnu.org/licenses/lgpl.html>
 * @version $Id$
 * Revision History ---
 *    20120519 - Initial version from BeRTOS
 * -----------------------------------------------------------------------------
 * Bertos is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * As a special exception, you may use this file as part of a free software
 * library without restriction.  Specifically, if other files instantiate
 * templates or use macros or inline functions from this file, or you compile
 * this file and link it with other files to produce an executable, this
 * file does not by itself cause the resulting executable to be covered by
 * the GNU General Public License.  This exception does not however
 * invalidate any other reasons why the executable file might be covered by
 * the GNU General Public License.
 *
 * Copyright 2009 Develer S.r.l. (http://www.develer.com/)
 */
#include <avrio/ax25.h>
#include <avrio/afsk.h>
#include <avrio/serial.h>
#include <avrio/task.h>
#include <avrio/mutex.h>
#include <avrio/led.h>
#include <stdio.h>
#include <string.h>

#define ADC_CH 3
#define BAUDRATE 1200
#define APRS_MSG ">Test SolarPi APRS http://www.btssn.net"

static xAx25Context ax25;
static xTaskHandle xScheduler;
static xMutex xMutexTx = MUTEX_LOCK;
static xAx25Call path[] = AX25_PATH(AX25_CALL("tlm100", 0), AX25_CALL("nocall", 0));

/* private functions ======================================================== */
// ------------------------------------------------------------------------------
static void
vMsgCallBack (struct xAx25Msg *msg) {

  vLedToggle (LED_RX);
}

// ------------------------------------------------------------------------------
static void
vScheduler (xTaskHandle xScheduler) {

  vMutexUnlock (&xMutexTx);
  vTaskStart (xScheduler);
}

// ------------------------------------------------------------------------------
static void
init (void) {

  vLedInit();

  /*
   * Init afsk demodulator. We need to implement the macros defined in hw_afsk.h, which
   * is the hardware abstraction layer.
   * We do not need transmission for now, so we set transmission DAC channel to 0.
   */
  vAfskInit (AFSK_MODE_NOBLOCK);

  /*
   * Here we initialize AX25 context, the channel we are going to read messages
   * from and the callback that will be called on incoming messages.
   */
  vAx25Init (&ax25, &xAfskPort, vMsgCallBack);

  xScheduler = xTaskCreate (xTaskConvertMs (5000), vScheduler);
  vTaskStart (xScheduler);
}

/* internal public functions ================================================ */
int
main(void) {

  init();

  for (;;) {

    /*
     * This function will look for new messages from the AFSK channel.
     * It will call the vMsgCallBack() function when a new message is received.
     * If there's nothing to do, this function will call cpu_relax()
     */
    vAx25Poll (&ax25);

    if (xMutexTryLock(&xMutexTx) == 0) {

      vAx25SendVia (&ax25, path, countof(path), APRS_MSG, sizeof(APRS_MSG));
    }
  }
  return 0;
}
