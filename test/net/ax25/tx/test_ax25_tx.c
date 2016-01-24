/*
 * Test du module Ax25 en transmission
 *
 * Envoie une trame Ax25/APRS de façon périodique.
 *
 */
#include <avrio/ax25.h>
#include <avrio/serial.h>
#include <avrio/led.h>
#include <avrio/delay.h>
#include <avrio/afsk.h>

/* constants ================================================================ */
// Les constantes ci_dessous peuvent être modifiées au besoin par l'utilisateur
// Baudrate de la liaison série en baud
#define SER_BAUDRATE  38400
// Période d'envoi des trames en ms
#define TX_PERIOD_MS  2000
// Station destination de la trame envoyée
#define APRS_DST "tlm100"
// Station source de la trame envoyée
#define APRS_SRC "nocall"
// Information APRS envoyé
#define APRS_MSG ">Test %05u SolarPi APRS http://www.btssn.net"

/* private variables ======================================================== */
static xAx25 ax25;
static xAx25Node path[] = AX25_PATH(AX25_CALL(APRS_DST, 0), AX25_CALL(APRS_SRC, 0));

/* internal public functions ================================================ */
int
main(void) {

  vLedInit();
  vSerialInit (SER_BAUDRATE/100, SERIAL_DEFAULT + SERIAL_WR);
  vAfskInit (AFSK_MODE_NOBLOCK);
  vAx25Init (&ax25, &xAfskPort, &xAfskPort, NULL);
  sei();

  for (;;) {
    static uint16_t usCount;
    char msg[80];
    // Numérotation de la trame afin de suivre la perte éventuelle à la
    // réception
    snprintf (msg, sizeof(msg), APRS_MSG, usCount++);

    vAx25SendVia (&ax25, path, countof(path), msg, strlen(msg));
    delay_ms (TX_PERIOD_MS);
  }
  return 0;
}
