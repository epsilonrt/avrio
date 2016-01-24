#include <avr/pgmspace.h>
#include <avrio/serial.h>
#include <avrio/led.h>
#include <avrio/delay.h>
#include "avrio-board-serial.h"

/* internal public functions ================================================ */
void vTestDebug (void);
void vTestAlphabet (void);
void vTestTerminal (void);
void vTestPrintf (void);
void vTestStdio (void);
void vTestPong (void);
void vTestPongStdIo (void);
void vTestPongStdIoFwrite (void);
void vTestReception (void);
void vTestTxOverflow (void);
void vLedAssert (int i);
size_t fread_l (void *ptr, size_t size, size_t nmemb, FILE *stream);

/* constants ================================================================ */
#define TEST_BAUDRATE     38400
#define TEST_DATABIT      SERIAL_8BIT // 5 à 9 bits
#define TEST_PARITY       SERIAL_NONE // NONE, EVEN, ODD
#define TEST_STOPBIT      SERIAL_1STP // 1 ou 2
#define TEST_OPT_READ     1
#define TEST_OPT_WRITE    1
#define TEST_OPT_ECHO     0
#define TEST_OPT_NOBLOCK  1
#define TEST_OPT_RTSCTS   0
#define TEST_DELAY        500 // Valeur en ms

/* Pour valider une test -> retirer le commentaire */
//#define TEST_DEBUG
//#define TEST_ALPHABET
//#define TEST_TXOVERFLOW
//#define TEST_TERMINAL
//#define TEST_STDIO
#define TEST_PRINTF
//#define TEST_PONG
//#define TEST_PONG_STDIO
//#define TEST_PONG_STDIO_FWRITE

#define TEST_SETUP (TEST_DATABIT + TEST_PARITY + TEST_STOPBIT + \
                    (TEST_OPT_READ ? SERIAL_RD : 0) + \
                    (TEST_OPT_WRITE ? SERIAL_WR : 0) + \
                    (TEST_OPT_ECHO ? SERIAL_ECHO : 0) + \
                    (TEST_OPT_NOBLOCK ? SERIAL_NOBLOCK : 0) + \
                    (TEST_OPT_RTSCTS ? SERIAL_RTSCTS : 0))

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
    vTestTxOverflow();
    vTestTerminal ();
    vTestPrintf ();
    vTestStdio ();
    vTestPong();
    vTestPongStdIo();
    vTestPongStdIoFwrite();
#if TEST_DELAY > 0
    delay_ms (TEST_DELAY);
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
#ifdef TEST_DEBUG

  c = 0x55;
  iErr = iSerialPutChar (c);
  vLedAssert (iErr == 0);
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

  vLedSet (LED_LED1);
  while (ucCount--) {

    c = 'A';
    do {

      iErr = iSerialPutChar (c);
      vLedAssert (iErr == 0);
    }
    while (c++ < 'Z');

    iErr = iSerialPutChar ('\n');
    vLedAssert (iErr == 0);
  }
  vLedClear (LED_LED1);
#endif
}

/* -----------------------------------------------------------------------------
 * Envoi d'une longue chaine de caractères (2 fois plus longue que le buffer tx)
 */
void
vTestTxOverflow (void) {
#if defined(TEST_TXOVERFLOW) && defined(SERIAL_TXBUFSIZE)
  #define BUFSIZE (SERIAL_TXBUFSIZE * 2)
  char s[BUFSIZE];
  for (int i = 0; i < BUFSIZE - 1; i++) {
    
    s[i] = (i % 10) + 0x30;
  }
  s[BUFSIZE - 1] = 0;
  vSerialPutString (s);
#endif
}

/* -----------------------------------------------------------------------------
 * Test Terminal
 * Invite puis attente d'un caractère puis renvoi
 */
void
vTestTerminal (void) {
#ifdef TEST_TERMINAL
  uint16_t usCount = 0;
  bool isWait = true;

  vSerialPutString ("\nTerminal Test\nPress any key (ENTER to quit)...");
  do {

    c = iSerialGetChar ();
    if (c != _FDEV_EOF) {

      if (isWait) {
        iErr = iSerialPutChar ('\n');
        vLedAssert (iErr == 0);
        isWait = false;
      }
      iErr = iSerialPutChar (c);
      vLedAssert (iErr == 0);
      vLedToggle (LED_LED1);
    }
    else {
      if ( (isWait) && ( (usCount++ % 50000) == 0)) {

        iErr = iSerialPutChar ('.');
        vLedAssert (iErr == 0);
      }
    }
  }
  while (c != '\r');       /* Return pour terminer */
  iErr = iSerialPutChar ('\n');
  vLedAssert (iErr == 0);
#endif
}

/* -----------------------------------------------------------------------------
 * Test Printf
 * Affichage de chaîne de caractère courte (gestion de TXEN)
 */
void
vTestPrintf (void) {
#ifdef TEST_PRINTF

  for (c = 0; c < 10; c++) {

    iErr = printf ("%d", c);
    //delay_ms(100);
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
  uint16_t usCount = 0;
  bool isWait = true;

  puts_P (PSTR ("\nStdio Test\n-printf() test"));
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
 * Test Pong
 * Boucle infinie d'attente d'un caractère puis renvoi
 */
void
vTestPong (void) {
#ifdef TEST_PONG

  // vSerialPutString ("\nPong Test\nPress any key...\n");
  for (;;) {

    c = iSerialGetChar ();
    if (c != _FDEV_EOF) {

      iErr = iSerialPutChar (c);
      vLedAssert (iErr == 0);
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

  // iErr = puts ("\nPong Stdio Test\nPress any key...");
  // vLedAssert (iErr >= 0);
  for (;;) {

    c = getc (&xSerialPort);
    iErr = ferror (&xSerialPort);
    vLedAssert (iErr == 0);
    if ( (c != EOF) && (feof (&xSerialPort) == 0)) {

      iErr = putc (c, &xSerialPort);
      vLedAssert (iErr == c);
      iErr = ferror (&xSerialPort);
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
vTestPongStdIoFwrite (void) {
#ifdef TEST_PONG_STDIO_FWRITE
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
