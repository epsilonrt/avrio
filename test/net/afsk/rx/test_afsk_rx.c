/*
 * Test Afsk en réception
 *
 * Affichage des octets reçus sur le terminal série
 *
 */
#include <string.h>
#include <ctype.h>
#include <avr/interrupt.h>
#include <avrio/led.h>
#include <avrio/delay.h>
#include <avrio/serial.h>
#include <avrio/afsk.h>

/* constants ================================================================ */
// Baudrate de la liaison série en baud
#define SER_BAUDRATE  38400

/* internal public functions ================================================ */
int
main(void) {
  int c;

  vLedInit();
  vAfskInit (AFSK_MODE_NOBLOCK);
  vSerialInit (SER_BAUDRATE/100, SERIAL_DEFAULT + SERIAL_WR);
  stdout = &xSerialPort;
  sei();

  for (;;) {

    c = fgetc (&xAfskPort);
    if (c != EOF) {

      if (c == HDLC_FLAG) {

        putchar('\n');
      }
      else if (isprint(c))
        putchar(c);
      else
        printf("\\%02X",c);
    }
    else  {

      delay_ms (10);
    }
  }
  return 0;
}
/* ========================================================================== */
