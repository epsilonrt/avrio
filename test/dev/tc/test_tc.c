/**
 * @file test_serial.c
 * @brief Test unitaire liaison série asynchrone
 *
 * Copyright © 2011-2015 Pascal JEAN aka epsilonRT. All rights reserved.
 *
 * This file is part of AvrIO.
 *
 * AvrIO is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * AvrIO is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with AvrIO.  If not, see <http://www.gnu.org/licenses/lgpl.html>
 */
#include <avrio/tc.h>
#include <avrio/led.h>
#include <avrio/delay.h>
#include <avr/pgmspace.h>
#include <avr/version.h>
// Pour TC_TXBUFSIZE
#include "avrio-board-tc.h"

/* constants ================================================================ */
#define PORT         "tty0"
#define BAUDRATE     115200
#define DATABIT      TC_DATABIT_8 // 5 à 9 bits
#define PARITY       TC_PARITY_NONE // NONE, EVEN, ODD
#define STOPBIT      TC_STOPBIT_ONE // 1 ou 2
#define FLOWCTL      TC_FLOWCTL_RTSCTS
#define OPT_READ     1
#define OPT_WRITE    1
#define OPT_ECHO     0
#define OPT_NOBLOCK  1
#define LOOP_DELAY   0 // Valeur en ms

/* Pour valider une test -> retirer le commentaire */
#define TEST_DEBUG        1
#define TEST_ALPHABET     0
#define TEST_TXOVERFLOW   0
#define TEST_TERMINAL     0
#define TEST_PRINTF       0
#define TEST_PONG         0
#define TEST_PONG_FWRITE  0

/* constants ================================================================ */
#define FLAGS ((OPT_READ ? O_RD : 0) + \
               (OPT_WRITE ? O_WR : 0) + \
               (OPT_ECHO ? O_ECHO : 0) + \
               (OPT_NOBLOCK ? O_NONBLOCK : 0))

/* private variables ======================================================== */
static xTcIos settings = {
  .baud = BAUDRATE, .dbits = DATABIT, .parity = PARITY,
  .sbits = STOPBIT, .flowctl = FLOWCTL
};

/* internal public functions ================================================ */
void vTestDebug (void);
void vTestAlphabet (void);
void vTestTerminal (void);
void vTestPrintf (void);
void vTestPong (void);
void vTestPongFwrite (void);
void vTestReception (void);
void vTestTxOverflow (void);
void vLedAssert (int i);

/* main ===================================================================== */
int
main (void) {

  vLedInit();
  FILE * tc = xFileOpen (PORT, FLAGS, &settings);
  stdout = tc;
  stdin = tc;
  sei();

  for (;;) {

    vTestDebug ();
    vTestAlphabet ();
    vTestTxOverflow();
    vTestTerminal ();
    vTestPrintf ();
    vTestPong();
    vTestPongFwrite();
#if LOOP_DELAY > 0
    delay_ms (LOOP_DELAY);
#endif
  }
  return 0;
}

/* internal public functions ================================================ */
static int iErr;
static int c;

/* -----------------------------------------------------------------------------
 * Test de debug
 * Envoi d'un U
 */
void
vTestDebug (void) {
#if TEST_DEBUG

  c = 0x55;
  iErr = putchar (c);
  vLedAssert (iErr == c);
  vLedToggle (LED_LED1);
#endif
}

/* -----------------------------------------------------------------------------
 * Envoi de l'alphabet A -> Z
 */
void
vTestAlphabet (void) {
#if TEST_ALPHABET
  uint8_t ucCount = 32;

  vLedSet (LED_LED1);
  while (ucCount--) {

    c = 'A';
    do {

      iErr = putchar (c);
      vLedAssert (iErr == c);
    }
    while (c++ < 'Z');

    c = '\n';
    iErr = putchar (c);
    vLedAssert (iErr == c);
  }
  vLedClear (LED_LED1);
#endif
}

/* -----------------------------------------------------------------------------
 * Envoi d'une longue chaine de caractères (2 fois plus longue que le buffer tx)
 */
void
vTestTxOverflow (void) {
#if TEST_TXOVERFLOW != 0 && defined(TC_TXBUFSIZE)
#define BUFSIZE (TC_TXBUFSIZE * 2)
  char s[BUFSIZE];
  for (int i = 0; i < BUFSIZE - 1; i++) {

    s[i] = (i % 10) + 0x30;
  }
  s[BUFSIZE - 1] = 0;
  fputs (s, stdout);
#endif
}

/* -----------------------------------------------------------------------------
 * Test Terminal
 * Invite puis attente d'un caractère puis renvoi
 */
void
vTestTerminal (void) {
#if TEST_TERMINAL
  uint16_t usCount = 0;
  bool isWait = true;

  fputs ("\nTerminal Test\nPress any key (ENTER to quit)...", stdout);
  do {

    c = getchar ();
    if (c != EOF) {

      if (isWait) {
        iErr = putchar ('\n');
        vLedAssert (iErr == '\n');
        isWait = false;
      }
      iErr = putchar (c);
      vLedAssert (iErr == c);
      vLedToggle (LED_LED1);
    }
    else {
      if ( (isWait) && ( (usCount++ % 50000) == 0)) {

        iErr = putchar ('.');
        vLedAssert (iErr == '.');
      }
    }
  }
  while (c != '\r');       /* Return pour terminer */
  iErr = putchar ('\n');
  vLedAssert (iErr == '\n');
#endif
}

/* -----------------------------------------------------------------------------
 * Test Stdio
 * Attente d'un caractère puis renvoi
 */
void
vTestPrintf (void) {
#if TEST_PRINTF
  uint16_t usCount = 0;
  bool isWait = true;

  printf_P (PSTR ("\nPrintf Test\nLibc version: %s\n-printf() test\n"), __AVR_LIBC_VERSION_STRING__);
  for (c = 0; c < 8; c++) {

    printf_P (PSTR ("\tTest #0x%02X\r"), c);
  }

  printf_P (PSTR ("\n-getchar() test: Press any key (ENTER to quit)..."));
  do {

    c =  getchar ();;
    iErr = ferror (stdin);
    vLedAssert (iErr == 0);
    if ( (c != EOF) && (feof (stdin) == 0)) {

      if (isWait) {
        iErr = putchar ('\n');
        vLedAssert (iErr != EOF);
        iErr = ferror (stdout);
        vLedAssert (iErr == 0);
        isWait = false;
      }

      iErr = putchar (c);
      vLedAssert (iErr == c);
      iErr = ferror (stdout);
      vLedAssert (iErr == 0);
      vLedToggle (LED_LED1);
    }
    else {
      if ( (isWait) && ( (usCount++ % 10000) == 0)) {

        iErr = putchar ('.');
        vLedAssert (iErr != EOF);
        iErr = ferror (stdout);
        vLedAssert (iErr == 0);
      }
    }

  }
  while (c != '\r');    /* Return pour terminer */
  putchar ('\n');
#endif
}

/* -----------------------------------------------------------------------------
 * Test Pong, version avec les fonctions stdio getc et putc
 * Boucle infinie d'attente d'un caractère puis renvoi
 */
void
vTestPong (void) {
#if TEST_PONG

  // iErr = puts ("\nPong Stdio Test\nPress any key...");
  // vLedAssert (iErr >= 0);
  for (;;) {

    c = getchar ();
    iErr = ferror (stdin);
    vLedAssert (iErr == 0);
    if ( (c != EOF) && (feof (stdin) == 0)) {

      iErr = putc (c, stdin);
      vLedAssert (iErr == c);
      iErr = ferror (stdout);
      vLedAssert (iErr == 0);
      vLedToggle (LED_LED1);
    }
  }
#endif
}

/* -----------------------------------------------------------------------------
 * Test avec les fonctions stdio fread et fwrite
 * Boucle infinie d'attente d'un caractère puis renvoi
 *                            <<< WARNING >>>
 * Les fonctions fread et fwrite de avr-libc font appel à getc et putc pour lire
 * le stream ce qui n'améne donc aucune optimisation par rapport à un appel
 * direct.
 */
void
vTestPongFwrite (void) {
#if TEST_PONG_FWRITE
  char buffer[32];
  static size_t ulReadLen, ulWriteLen;

  for (;;) {

    ulReadLen = fread (buffer, 1, sizeof (buffer), stdin);
    vLedAssert (ferror (stdin) == 0);
    if (ulReadLen) {

      ulWriteLen = fwrite (buffer, 1, ulReadLen, stdout);
      vLedAssert (ferror (stdout) == 0);
      vLedToggle (LED_LED1);
    }
  }
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
      delay_ms (50);
      vLedClear (LED_LED1);
      delay_ms (100);
    }
  }
}

/* ========================================================================== */
