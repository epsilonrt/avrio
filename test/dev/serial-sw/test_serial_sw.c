/**
 * @file test_serial.c
 * @brief Test unitaire liaison série asynchrone
 * @author Copyright © 2011-2012 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * Revision History ------------------------------------------------------------
 *    20110817 - Initial version by epsilonRT
 */
#include <avr/pgmspace.h>
#include <avrio/serial_sw.h>
#include <avrio/led.h>
#include <avrio/delay.h>
#include "avrio-board-serial-sw.h"

/* internal public functions ================================================ */
void vTestDebug (void);
void vTestAlphabet (void);
void vTestPong (void);
void vTestPongStdIo (void);
void vTestStdio (void);
void vLedAssert (int i);

/* constants ================================================================ */
#define TEST_DELAY 200

/* Pour valider une test -> retirer le commentaire */
//#define TEST_DEBUG
//#define TEST_ALPHABET
//#define TEST_PONG
#define TEST_PONG_STDIO
//#define TEST_STDIO

/* main ===================================================================== */
int
main (void) {

  vLedInit();
  vSerialSwInit ();
  stdout = &xSerialSwPort;
  stdin = &xSerialSwPort;
  sei();

  for (;;) {

    vTestDebug ();
    vTestAlphabet ();
    vTestPong();
    vTestPongStdIo();
    vTestStdio ();
#if TEST_DELAY != 0
    delay_ms (TEST_DELAY);
#endif
  }
  return 0;
}

/* internal public functions ================================================ */

/* -----------------------------------------------------------------------------
 * Test de debug
 * Envoi d'un U
 */
void
vTestDebug (void) {
#ifdef TEST_DEBUG
  char cChar = 0x55;

  vSerialSwPutChar (cChar);
  vLedToggle (LED_LED1);
#endif
}

/* -----------------------------------------------------------------------------
 * Envoi de l'alphabet A -> Z
 */
void
vTestAlphabet (void) {
#ifdef TEST_ALPHABET
  uint8_t ucCount = 32;
  char cChar;

  vSerialSwPutChar ('\n');
  vLedSet (LED_LED1);
  while (ucCount--) {

    cChar = 'A';
    do {

      vSerialSwPutChar (cChar);
    }
    while (cChar++ < 'Z');

    vSerialSwPutChar ('\n');
  }
  vLedClear (LED_LED1);
#endif
}

/* -----------------------------------------------------------------------------
 * Test Pong
 * Boucle infinie d'attente d'un caractère puis renvoi
 */
void
vTestPong (void) {
#ifdef TEST_PONG
  static int c;

  for (;;) {

    c = iSerialSwGetChar ();
    if (c != _FDEV_EOF) {

      vSerialSwPutChar (c);
      vLedToggle (LED_LED1);
    }
  }
#endif
}

/* -----------------------------------------------------------------------------
 * Test Pong, version avec les fonctions stdio getc et putc
 * Boucle infinie d'attente d'un caractère puis renvoi
 */
void
vTestPongStdIo (void) {
#ifdef TEST_PONG_STDIO
  static int c;

  for (;;) {

    c = getc (&xSerialSwPort);
    vLedAssert (ferror (&xSerialSwPort) == 0);
    if ( (c != EOF) && (feof (&xSerialSwPort) == 0)) {

      putc (c, &xSerialSwPort);
      vLedAssert (ferror (&xSerialSwPort) == 0);
      vLedToggle (LED_LED1);
    }
  }
#endif
}

/* -----------------------------------------------------------------------------
 * Test Stdio
 * Attente d'un caractère puis renvoi
 */
void
vTestStdio (void) {
#ifdef TEST_STDIO
  int xChar;


  puts_P (PSTR ("\nTest5 Stdio\n-\tprintf()\n"));
  for (xChar = 0; xChar < 8; xChar++) {

    printf_P (PSTR ("\tStatus 0x%02X\r"), xChar);
  }

  puts_P (PSTR ("-\tgetchar(): Tapez quelque chose au clavier (ENTER pour quitter)\n"));
  do {
    xChar = getchar ();
    putchar (xChar);
    vLedToggle (LED_LED1);
  }
  while (xChar != '\r');    /* Return pour terminer */
  putchar ('\n');
#endif
}

/* -----------------------------------------------------------------------------
 * Vérifie que la condition passée est vraie, sinon fait clignoter rapidement
 * la led 1 à l'infinie
 */
void
vLedAssert (int i) {

  if (!i) {

    for (;;) {

      vLedSet (LED_LED1);
      delay_ms (5);
      vLedClear (LED_LED1);
      delay_ms (25);
    }
  }
}

/* ========================================================================== */
