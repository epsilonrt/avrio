/**
 * @file
 * Exemple d'utilisation du module Tnc
 *
 * Attends la réception d'un message TNC et renvoie un accusé réception
 */
#include <avrio/led.h>
#include <avrio/delay.h>
#include <avrio/tnc.h>
#include <avrio/tc.h>

/* constants ================================================================ */
#define BAUDRATE  115200
#define PORT      "tty0"

/* private variables ======================================================== */
static xTnc tnc;
static int i;

/* private functions ======================================================== */

// ------------------------------------------------------------------------------
static void
vAssert (bool test) {

  if (test == false) {

    for (;;) {

      vLedSet (LED_LED1);
      delay_ms (25);
      vLedClear (LED_LED1);
      delay_ms (75);
    }
  }
}

/* internal public functions ================================================ */
int
main(void) {

  vLedInit ();
  // Configuration du port série par défaut (8N1, sans RTS/CTS)
  xSerialIos settings = SERIAL_SETTINGS (BAUDRATE);
  // Ouverture du port série en entrée et en sortie
  FILE * serial_port = xFileOpen (PORT, O_RDWR | O_NONBLOCK, &settings);
  sei();
  vTncInit (&tnc, serial_port, serial_port);
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
