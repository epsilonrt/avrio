/**
 * @file test_serial.c
 * @brief Test unitaire liaison série asynchrone
 * @author Copyright © 2011-2012 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * @version $Id$
 * Revision History ---
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
void vTestPutShortString (void);
void vTestPutLongString (void);
void vTestTerminal (void);
void vTestStdio (void);
void vTestReception (void);

/* constants ================================================================ */
#define TEST_DELAY 200

/* Pour valider une test -> retirer le commentaire */
//#define TEST_DEBUG
#define TEST_STRS
#define TEST_STRL
#define TEST_ALPHABET
//#define TEST_TERMINAL
#define TEST_STDIO
//#define TEST_RECEPTION

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
    vTestPutShortString ();
    vTestPutLongString ();
    vTestAlphabet ();
    vTestTerminal ();
    vTestStdio ();
    vTestReception ();
#if TEST_DELAY != 0
    delay_ms(TEST_DELAY);
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
 * Test affichage chaîne de caractères courte
 */
void
vTestPutShortString (void) {
#ifdef TEST_STRS

  vLedSet (LED_LED1);
  vSerialSwPutString ("\nTest1\n");
  vLedClear (LED_LED1);
#endif
}

/* -----------------------------------------------------------------------------
 * Test affichage chaîne de caractères longue
 */
void
vTestPutLongString (void) {
#ifdef TEST_STRL

  vLedSet (LED_LED1);
  vSerialSwPutString ("\nTest2: Affichage chaine de caracteres longue Ok\n");
  vLedClear (LED_LED1);
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
    } while (cChar++ < 'Z');

    vSerialSwPutChar ('\n');
  }
  vLedClear (LED_LED1);
#endif
}

/* -----------------------------------------------------------------------------
 * Test Terminal
 * Attente d'un caractère puis renvoi
 */
void
vTestTerminal (void) {
#ifdef TEST_TERMINAL
  char cChar;

  vSerialSwPutString ("\nTest4 Terminal\nTapez quelque chose au clavier (ENTER pour quitter)\n");
  do {

    cChar = cSerialSwGetChar ();
    vSerialSwPutChar (cChar);
    vLedToggle (LED_LED1);
  } while (cChar != '\r');  /* Return pour terminer */
  vSerialSwPutChar ('\n');
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


  puts_P (PSTR("\nTest5 Stdio\n-\tprintf()\n"));
  for (xChar = 0; xChar < 8; xChar++) {

    printf_P (PSTR("\tStatus 0x%02X\r"), xChar);
  }

  puts_P (PSTR("-\tgetchar(): Tapez quelque chose au clavier (ENTER pour quitter)\n"));
  do {
    xChar = getchar ();
    putchar (xChar);
    vLedToggle (LED_LED1);
  } while (xChar != '\r');  /* Return pour terminer */
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
    pcBuffer[(xIndex++) % 80] = getchar ();
  }
  while (1);
#endif
}

/* ========================================================================== */
