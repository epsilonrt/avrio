/**
 * @file test_xbee.c
 * @brief Test du module XBee
 *
 * Envoie une trame Ax25/APRS de façon périodique.
 *
 * @author Copyright © 2014 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * @version $Id$
   ========================================================================== */
#include <avrio/xbee.h>
#include <avrio/serial.h>
#include <avrio/led.h>
#include <avrio/delay.h>

/* constants ================================================================ */
// Les constantes ci_dessous peuvent être modifiées au besoin par l'utilisateur
// Baudrate de la liaison série en baud
#define SER_BAUDRATE  38400
// Période d'envoi des trames en ms
#define TX_PERIOD_MS  2000

/* private variables ======================================================== */
static xXBee xbee;


/* internal public functions ================================================ */
int
main(void) {

  vLedInit();
  vSerialInit (SER_BAUDRATE/100, SERIAL_DEFAULT + SERIAL_WR + SERIAL_NOBLOCK);
  sei();
  
  vXBeeInit (&xbee, &xSerialPort);

  for (;;) {
    
    iXBeePoll (&xbee);
  }
  return 0;
}

/* -----------------------------------------------------------------------------
 * Handle an incoming packet
 *
 * the packet will be fully formed and verified
 * for proper construction before being passed off to this function.  This
 * function should dig into the packet & process based on its contents.
 */
int 
iXBeeRecvPktCB (xXBee *xbee, xXBeePkt *pkt, uint8_t len) {

  vLedToggle (LED_LED1);
  return 0;
}
