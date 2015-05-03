/**
 * @file demo_serial.c
 * @brief Exemple liaison série asynchrone
 * @author Copyright © 2011-2012 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * Revision History ------------------------------------------------------------
 *    20110817 - Initial version by epsilonRT
 */
#include <avrio/led.h>
#include <avrio/serial.h>


/* constants ================================================================ */
#define TEST_BAUDRATE 38400

/* main ===================================================================== */
int
main (void) {
  int c;

  vLedInit();
  vSerialInit (TEST_BAUDRATE / 100, SERIAL_DEFAULT + SERIAL_RW + SERIAL_NOBLOCK);
  // Les fonctions de stdio.h utiliseront le port série en entrée et sortie
  stdout = &xSerialPort;
  stdin = &xSerialPort;
  sei();

  printf ("Exemple d'utilisation du module serial\n");
  for (;;) {

    // Renvoie chaque caractère reçu
    c = getchar();

    if (c != EOF) {
      // Inverse l'état de la led à chaque réception
      vLedToggle (LED_LED1);
      putchar(c);
    }
  }
  return 0;
}
/* ========================================================================== */
