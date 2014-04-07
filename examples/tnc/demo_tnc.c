/**
 * @file demo_tnc.c
 * @brief Exemple d'utilisation des modules Tnc et Afsk
 *
 * Envoie une trame Tnc/APRS de façon périodique et affiche les trames reçues
 * sur la liaison série.
 *
 * @author Copyright © 2014 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * @version $Id$
   ========================================================================== */
#include <avrio/led.h>
#include <avrio/delay.h>
#include <avrio/tnc.h>
#include <avrio/serial.h>
#include <avrio/task.h>
#include <avrio/mutex.h>

/* constants ================================================================ */
// Les constantes ci_dessous peuvent être modifiées au besoin par l'utilisateur
// Baudrate de la liaison série en baud
#define SER_BAUDRATE  38400
// Période d'envoi des trames en ms
#define TRANSMIT_PERIOD  10000

/* private variables ======================================================== */
static xTnc tnc;
static uint8_t msg[TNC_RXBUFSIZE];
static xTaskHandle xScheduler;
static xMutex xMutexTx = MUTEX_LOCK;

/* private functions ======================================================== */

// ------------------------------------------------------------------------------
static void
vAssert (bool test) {

  if (test == false) {

    for (;;) {

      vLedSet (LED_LED2);
      delay_ms (50);
      vLedClear (LED_LED2);
      delay_ms (150);
    }
  }
}

// -----------------------------------------------------------------------------
// Cadence l'envoi de trame en dévérouillant le mutex toutes les TX_PERIOD_MS
// millisecondes
static void
vScheduler (xTaskHandle xScheduler) {

  vMutexUnlock (&xMutexTx);
  vTaskStart (xScheduler);
}

/* internal public functions ================================================ */
int
main(void) {
  int i;

  vLedInit ();
  vSerialInit (SER_BAUDRATE/100, SERIAL_DEFAULT + SERIAL_RW);
  vTncInit (&tnc, &xSerialPort, &xSerialPort);
  for (i = 0; i < TNC_RXBUFSIZE; i++)
    msg[i] = i;
  xScheduler = xTaskCreate (xTaskConvertMs (TRANSMIT_PERIOD), vScheduler);
  vTaskStart (xScheduler);

  for (;;) {


    i = iTncPoll (&tnc);
    vAssert (i >= 0);
    if (i == TNC_EOT) {

      for (i = 0; i < tnc.len; i++)
        vAssert (tnc.rxbuf[i] == i);
      vLedToggle (LED_LED1);
    }

    if (xMutexTryLock(&xMutexTx) == 0) {

      i = iTncWrite (&tnc, msg, sizeof(msg));
      vAssert (i == sizeof(msg));
    }
  }
  return 0;
}
