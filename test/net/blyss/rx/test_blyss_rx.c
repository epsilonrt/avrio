/*
 * Test module blyss en réception
 */
#include <stdio.h>
#include <avr/pgmspace.h>
#include <avrio/delay.h>
#include <avrio/blyss.h>
#include <avrio/tc.h>

/* constants ================================================================ */
#define TERMINAL_BAUDRATE 115200
#define TERMINAL_PORT     "tty0"

/* main ===================================================================== */
int
main (void) {
  xBlyssFrame f;
  xSerialIos xTermIos = SERIAL_SETTINGS (TERMINAL_BAUDRATE);

  FILE * tc = xFileOpen (TERMINAL_PORT, O_WRONLY, &xTermIos);
  stdout = tc;
  stderr = tc;

  vBlyssInit ();
  sei();
  printf_P (PSTR ("** Blyss Receiver Test **\n"));

  for (;;) {

    if (bBlyssReceive(&f)) {

      vBlyssPrintFrame (&f);
    }
  }
  return 0;
}
/* ========================================================================== */
