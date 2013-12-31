/**
 * @file demo_serial.c
 * @brief Exemple liaison série asynchrone
 * @author Copyright © 2011-2012 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * @version $Id$
 * Revision History ---
 *    20110817 - Initial version by epsilonRT
 */
#include <avrio/led.h>
#include <avrio/serial.h>


/* constants ================================================================ */
#define TEST_BAUDRATE 38400

/* main ===================================================================== */
int
main (void) {
  char c;

  vLedInit();
  vSerialInit (TEST_BAUDRATE / 100, SERIAL_DEFAULT + SERIAL_RW);
  // Les fonctions de stdio.h utiliseront le port série en entrée et sortie
  stdout = &xSerialPort;
  stdin = &xSerialPort;
  sei();

  printf ("Exemple d'utilisation du module serial\n");
  for (;;) {

    // Renvoie chaque caractère reçu
    c = getchar();
    putchar(c);
    // Inverse l'état de la led à chaque réception
    vLedToggle (LED_LED1);
  }
  return 0;
}
/* ========================================================================== */
