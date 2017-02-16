/**
 * @file
 * Exemple d'utilisation du module Tnc
 *
 * Envoie une trame Tnc de façon périodique et affiche les trames reçues
 * sur la liaison série.
 */
#include <avrio/led.h>
#include <avrio/delay.h>
#include <avrio/tnc.h>
#include <avrio/tc.h>
#include <avrio/task.h>
#include <avrio/mutex.h>

/* constants ================================================================ */
#define BAUDRATE  115200
#define PORT      "tty0"
// Période d'envoi des trames en ms
#define TRANSMIT_PERIOD  300

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

      vLedSet (LED_LED1);
      delay_ms (50);
      vLedClear (LED_LED1);
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
  // Configuration du port série par défaut (8N1, sans RTS/CTS)
  xSerialIos settings = SERIAL_SETTINGS (BAUDRATE);
  // Ouverture du port série en entrée et en sortie
  FILE * serial_port = xFileOpen (PORT, O_RDWR | O_NONBLOCK, &settings);
  vTncInit (&tnc, serial_port, serial_port);
  sei();

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
