/**
 * @file demo_tnc.c
 * @brief Exemple d'utilisation du module Tnc
 *
 * Attends la réception d'un message TNC et renvoie un accusé réception
 *
 * @author Copyright © 2014 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>

   ========================================================================== */
#include <avrio/led.h>
#include <avrio/delay.h>
#include <avrio/tnc.h>
#include <avrio/serial.h>

/* constants ================================================================ */
// Les constantes ci_dessous peuvent être modifiées au besoin par l'utilisateur
// Baudrate de la liaison série en baud
#define SER_BAUDRATE  38400

/* private variables ======================================================== */
static xTnc tnc;
static int i;

/* private functions ======================================================== */

// ------------------------------------------------------------------------------
static void
vAssert (bool test) {

  if (test == false) {

    for (;;) {

      vLedSet (LED_LED2);
      delay_ms (25);
      vLedClear (LED_LED2);
      delay_ms (75);
    }
  }
}

/* internal public functions ================================================ */
int
main(void) {

  vLedInit ();
  vSerialInit (SER_BAUDRATE/100, SERIAL_DEFAULT + SERIAL_RW + SERIAL_NOBLOCK);
  vTncInit (&tnc, &xSerialPort, &xSerialPort);
  sei();

  for (;;) {

    i = iTncPoll (&tnc);
    vAssert (i >= 0);
    if (i == TNC_EOT) {
      uint16_t usLen = tnc.len;

      i = iTncWrite (&tnc, &usLen, sizeof(usLen));
    }
  }
  return 0;
}
