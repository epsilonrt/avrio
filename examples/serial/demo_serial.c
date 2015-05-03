/**
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
 *
 * @file demo_serial.c
 * @brief Exemple liaison série asynchrone
 *
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
