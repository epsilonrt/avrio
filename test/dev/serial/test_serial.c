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
#include <avrio/serial.h>
#include <avrio/led.h>
#include <avrio/delay.h>
#include "avrio-board-serial.h"

/* internal public functions ================================================ */
void vTestDebug (void);
void vTestAlphabet (void);
void vTestPong (void);
void vTestPongStdIo (void);
void vTestPongBlock (void);
void vTestTerminal (void);
void vTestStdio (void);
void vTestReception (void);
void vLedAssert (int i);
size_t fread_l (void *ptr, size_t size, size_t nmemb, FILE *stream);

/* constants ================================================================ */
#define TEST_BAUDRATE 38400
/*
 * WARNING: A baudrate higher than 38400 baud with half-duplex requires
 * 2 stop bits and parity checking ! The link was configured with 2 stop bits
 * and even parity.
 */
// Config avec lecture non bloquante
//#define TEST_SETUP (SERIAL_DEFAULT + SERIAL_RW + SERIAL_RTSCTS + SERIAL_NOBLOCK)
// Config avec lecture  bloquante
#define TEST_SETUP (SERIAL_DEFAULT + SERIAL_RW + SERIAL_RTSCTS)


/* Pour valider une test -> retirer le commentaire */
//#define TEST_DEBUG
//#define TEST_ALPHABET
//#define TEST_TERMINAL
//#define TEST_PONG
#define TEST_PONG_STDIO
//#define TEST_PONG_BLOCK
//#define TEST_STRS
//#define TEST_STRL
//#define TEST_STDIO
//#define TEST_RECEPTION

/* main ===================================================================== */
int
main (void) {

  vLedInit();
  vSerialInit (TEST_BAUDRATE / 100, TEST_SETUP);
  stdout = &xSerialPort;
  stdin = &xSerialPort;
  sei();

  for (;;) {

    vTestDebug ();
    vTestAlphabet ();
    vTestPong();
    vTestPongStdIo();
    vTestPongBlock();
    vTestTerminal ();
    vTestStdio ();
    vTestReception ();
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

  iSerialPutChar (cChar);
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

  vLedSet (LED_LED1);
  while (ucCount--) {

    cChar = 'A';
    do {

      iSerialPutChar (cChar);
    }
    while (cChar++ < 'Z');

    iSerialPutChar ('\n');
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

    c = iSerialGetChar ();
    if (c != _FDEV_EOF) {

      iSerialPutChar (c);
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

    c = getc (&xSerialPort);
    vLedAssert (ferror (&xSerialPort) == 0);
    if ( (c != EOF) && (feof (&xSerialPort) == 0)) {

      putc (c, &xSerialPort);
      vLedAssert (ferror (&xSerialPort) == 0);
      vLedToggle (LED_LED1);
    }
  }
#endif
}

/* -----------------------------------------------------------------------------
 * Test Pong-block avec les fonctions stdio fread et fwrite
 * Boucle infinie d'attente d'un caractère puis renvoi
 *                            <<< WARNING >>>
 * Les fonctions fread et fwrite de avr-libc font appel à getc et putc pour lire
 * le stream ce qui n'améne donc aucune optimisation par rapport à un appel
 * direct. Il y même une perte de performance liée à la recopie des buffers
 * ce qui amène à perdre des informations à fort débit.
 */
void
vTestPongBlock (void) {
#ifdef TEST_PONG_BLOCK
  char buffer[32];
  static size_t ulReadLen, ulWriteLen;

  for (;;) {

    ulReadLen = fread (buffer, 1, sizeof (buffer), &xSerialPort);
    vLedAssert (ferror (&xSerialPort) == 0);
    if (ulReadLen) {

      ulWriteLen = fwrite (buffer, 1, ulReadLen, &xSerialPort);
      vLedAssert (ferror (&xSerialPort) == 0);
      vLedToggle (LED_LED1);
    }
  }
#endif
}

/* -----------------------------------------------------------------------------
 * Test Terminal
 * Invite puis attente d'un caractère puis renvoi
 */
void
vTestTerminal (void) {
#ifdef TEST_TERMINAL
  char cChar;

  vSerialPutString ("\nTest4 Terminal\nTapez quelque chose au clavier (ENTER pour quitter)\n");
  do {

    cChar = iSerialGetChar ();
    iSerialPutChar (cChar);
    vLedToggle (LED_LED1);
  }
  while (cChar != '\r');    /* Return pour terminer */
  iSerialPutChar ('\n');
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
 * Test Réception
 * Stockage de tous les caractères reçus dans un tampon mémoire circulaire
 */
void
vTestReception (void) {
#ifdef TEST_RECEPTION
  static char pcBuffer[80];
  static uint8_t xIndex = 0;

  do {
    pcBuffer[ (xIndex++) % 80] = getchar ();
  }
  while (1);
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

/* -----------------------------------------------------------------------------
 * Copyright (c) 2002, Joerg Wunsch All rights reserved.
 * For debug purposes only
 */
size_t
fread_l (void *ptr, size_t size, size_t nmemb, FILE *stream) {
  size_t i, j;
  uint8_t *cp;
  int c;
  if ( (stream->flags & __SRD) == 0) {
    return 0;
  }
  for (i = 0, cp = (uint8_t *) ptr; i < nmemb; i++)
    for (j = 0; j < size; j++) {
      c = getc (stream);
      if (c == EOF) {
        return i;
      }
      *cp++ = (uint8_t) c;
    }
  return i;
}

/* ========================================================================== */
