/* Copyright © 2009-2012 epsilonRT. All rights reserved.
 *  $Id$ */
/* ========================================================================== */
#include <stdio.h>
#include <avr/pgmspace.h>
#include <avrio/serial.h>
#include <util/atomic.h>
#include <avrio/melody.h>
#include <avrio/mt8870.h>

/* private variables ======================================================== */

// pxHello = mélodie d'acceuil
static const xNote pxHello[] PROGMEM = {  // surprise ....
  {PITCH_PAUSE, 16}, {PITCH_PAUSE, 16},
  {29, 12}, {PITCH_PAUSE, 4}, {29, 12}, {PITCH_PAUSE, 4}, {29, 12},
     {PITCH_PAUSE, 4},
  {25, 8}, {PITCH_PAUSE, 4}, {32, 4}, {29, 12}, {PITCH_PAUSE, 4}, {25, 8},
  {PITCH_PAUSE, 4}, {32, 4}, {29, 25}, {PITCH_END, 0}
};

/* main ===================================================================== */
int
main (void) {
  char c;

  vSerialInit (96, SERIAL_DEFAULT + SERIAL_WR);
  stdout = &xSerialPort;
  printf ("\nTest avrio-phone\n");
  vMelodyInit (30, 0);
  vPhoneInit ();

  for (;;) {

    if (ucPhoneMsgLength ()) {

      c = cPhoneMsgGetChar ();
      putchar (c);
      switch (c) {

        case 'H':  // décrocher
          vPhoneHookoff ();
          vMelodyPlayFlash (pxHello); // son de bienvenue
          break;
        case 'G':  // raccrocher
          vPhoneHangup ();
          break;
        case 'K':  // Appui touche
          c = cPhoneMsgGetChar ();
          putchar (c);
          break;
        case 'R':  // Sonnerie
          c = cPhoneMsgGetChar ();
          printf ("%d", c);
          break;
        default:
          break;
      }
      putchar ('\n');
    }
  }
}

/* ========================================================================== */
