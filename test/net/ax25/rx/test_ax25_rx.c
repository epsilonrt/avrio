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
 * @file test_ax25_rx.c
 * @brief Test unitaire du module Ax25 en r�ception
 *
 * Re�oit les trames AX25 et les affiche sur le terminal s�rie.
 *
 *

   ========================================================================== */
#include <avrio/ax25.h>
#include <avrio/serial.h>
#include <avrio/led.h>
#include <avrio/afsk.h>

/* constants ================================================================ */
// Les constantes ci_dessous peuvent être modifiées au besoin par l'utilisateur
// Baudrate de la liaison série en baud
#define SER_BAUDRATE  38400

/* private variables ======================================================== */
static xAx25 ax25;

/* private functions ======================================================== */

// -----------------------------------------------------------------------------
// Gestion des trames reçues
static void
vMsgCallBack (struct xAx25Frame *msg) {

  vAx25Print (&xSerialPort, msg);
}

/* internal public functions ================================================ */
int
main(void) {

  vLedInit();
  vSerialInit (SER_BAUDRATE/100, SERIAL_DEFAULT + SERIAL_WR);
  vAfskInit (AFSK_MODE_NOBLOCK);
  vAx25Init (&ax25, &xAfskPort, &xAfskPort, vMsgCallBack);
  sei();

  for (;;) {

    vAx25Poll (&ax25);
  }
  return 0;
}
