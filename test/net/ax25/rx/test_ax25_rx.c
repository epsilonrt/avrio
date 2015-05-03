/**
 * @file test_ax25_rx.c
 * @brief Test unitaire du module Ax25 en r�ception
 *
 * Re�oit les trames AX25 et les affiche sur le terminal s�rie.
 *
 * @author Copyright © 2014 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>

   ========================================================================== */
#include <avrio/ax25.h>
#include <avrio/serial.h>
#include <avrio/led.h>
#include <avrio/afsk.h>

/* constants ================================================================ */
// Les constantes ci_dessous peuvent être modifiées au besoin par l'utilisateur
// Baudrate de la liaison série en baud
#define SER_BAUDRATE  38400

/* private variables ======================================================== */
static xAx25 ax25;

/* private functions ======================================================== */

// -----------------------------------------------------------------------------
// Gestion des trames reçues
static void
vMsgCallBack (struct xAx25Frame *msg) {

  vAx25Print (&xSerialPort, msg);
}

/* internal public functions ================================================ */
int
main(void) {

  vLedInit();
  vSerialInit (SER_BAUDRATE/100, SERIAL_DEFAULT + SERIAL_WR);
  vAfskInit (AFSK_MODE_NOBLOCK);
  vAx25Init (&ax25, &xAfskPort, &xAfskPort, vMsgCallBack);
  sei();

  for (;;) {

    vAx25Poll (&ax25);
  }
  return 0;
}
