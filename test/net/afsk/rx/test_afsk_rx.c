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
 * @file test_afsk_rx.c
 * @brief Test Afsk en réception
 *
 * Affichage des octets reçus sur le terminal série
 *
 *

   ========================================================================== */
#include <string.h>
#include <ctype.h>
#include <avr/interrupt.h>
#include <avrio/led.h>
#include <avrio/delay.h>
#include <avrio/serial.h>
#include <avrio/afsk.h>

/* constants ================================================================ */
// Baudrate de la liaison série en baud
#define SER_BAUDRATE  38400

/* internal public functions ================================================ */
int
main(void) {
  int c;

  vLedInit();
  vAfskInit (AFSK_MODE_NOBLOCK);
  vSerialInit (SER_BAUDRATE/100, SERIAL_DEFAULT + SERIAL_WR);
  stdout = &xSerialPort;
  sei();

  for (;;) {

    c = fgetc (&xAfskPort);
    if (c != EOF) {

      if (c == HDLC_FLAG) {

        putchar('\n');
      }
      else if (isprint(c))
        putchar(c);
      else
        printf("\\%02X",c);
    }
    else  {

      delay_ms (10);
    }
  }
  return 0;
}
/* ========================================================================== */
