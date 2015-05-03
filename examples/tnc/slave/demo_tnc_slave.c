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
 * @file demo_tnc.c
 * @brief Exemple d'utilisation du module Tnc
 *
 * Attends la réception d'un message TNC et renvoie un accusé réception
 *
 *

   ========================================================================== */
#include <avrio/led.h>
#include <avrio/delay.h>
#include <avrio/tnc.h>
#include <avrio/serial.h>

/* constants ================================================================ */
// Les constantes ci_dessous peuvent être modifiées au besoin par l'utilisateur
// Baudrate de la liaison série en baud
#define SER_BAUDRATE  38400

/* private variables ======================================================== */
static xTnc tnc;
static int i;

/* private functions ======================================================== */

// ------------------------------------------------------------------------------
static void
vAssert (bool test) {

  if (test == false) {

    for (;;) {

      vLedSet (LED_LED2);
      delay_ms (25);
      vLedClear (LED_LED2);
      delay_ms (75);
    }
  }
}

/* internal public functions ================================================ */
int
main(void) {

  vLedInit ();
  vSerialInit (SER_BAUDRATE/100, SERIAL_DEFAULT + SERIAL_RW + SERIAL_NOBLOCK);
  vTncInit (&tnc, &xSerialPort, &xSerialPort);
  sei();

  for (;;) {

    i = iTncPoll (&tnc);
    vAssert (i >= 0);
    if (i == TNC_EOT) {
      uint16_t usLen = tnc.len;

      i = iTncWrite (&tnc, &usLen, sizeof(usLen));
    }
  }
  return 0;
}
