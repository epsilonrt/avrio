/* Copyright _ 2009-2012 epsilonRT. All rights reserved.
 *  $Id$ */
/* ========================================================================== */
#include <ctype.h>
#include <avrio/serial.h>
#include <avrio/keyb.h>

/* constants ================================================================ */
#define TEST_BAUDRATE 115200

/* main ===================================================================== */
int
main (void) {

  vSerialInit (TEST_BAUDRATE / 100, SERIAL_DEFAULT + SERIAL_RW);
  stdout = &xSerialPort;
  puts ("Press key...");

  /* Initialisation Clavier */
  vKeybInit ();

  for (;;) {

    if (usKeybHit () != 0) {
      char c;

      c = cKeybGetChar ();
      if (isprint (c)) {

        putchar (c);
      }
      else {

        if (c == KEY_HOME)
          putchar('\n');
        else
          printf ("0x%02X",c);
       }
    }
  }

  return 0;
}
