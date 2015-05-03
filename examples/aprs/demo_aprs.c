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
 * @file demo_aprs.c
 * @brief Exemple d'utilisation des modules Ax25 et Afsk
 *
 * Envoie une trame Ax25/APRS de façon périodique et affiche les trames reçues
 * sur la liaison série.
 *
 *

   ========================================================================== */
#include <avrio/ax25.h>
#include <avrio/afsk.h>
#include <avrio/serial.h>
#include <avrio/task.h>
#include <avrio/mutex.h>

/* constants ================================================================ */
// Les constantes ci_dessous peuvent être modifiées au besoin par l'utilisateur
// Baudrate de la liaison série en baud
#define SER_BAUDRATE  38400
// Période d'envoi des trames en ms
#define TX_PERIOD_MS  5000
// Station destination de la trame envoyée
#define APRS_DST "tlm100"
// Station source de la trame envoyée
#define APRS_SRC "nocall"
// Information APRS envoyé
#define APRS_MSG ">Test %05u SolarPi APRS http://www.btssn.net"

/* private variables ======================================================== */
static xAx25 ax25;
static xAx25Node path[] = AX25_PATH(AX25_CALL(APRS_DST, 0), AX25_CALL(APRS_SRC, 0));
static xTaskHandle xScheduler;
static xMutex xMutexTx = MUTEX_LOCK;

/* private functions ======================================================== */

// -----------------------------------------------------------------------------
// Gestion des trames reçues
static void
vMsgCallBack (struct xAx25Frame *msg) {

  vAx25Print (&xSerialPort, msg);
}

// -----------------------------------------------------------------------------
// Cadence l'envoi de trame en dévérouillant le mutex toutes les TX_PERIOD_MS
// millisecondes
static void
vScheduler (xTaskHandle xScheduler) {

  vMutexUnlock (&xMutexTx);
  vTaskStart (xScheduler);
}

/* internal public functions ================================================ */
int
main(void) {

  vSerialInit (SER_BAUDRATE/100, SERIAL_DEFAULT + SERIAL_WR);
  vAfskInit (AFSK_MODE_NOBLOCK);
  vAx25Init (&ax25, &xAfskPort, &xAfskPort, vMsgCallBack);
  sei();

  xScheduler = xTaskCreate (xTaskConvertMs (TX_PERIOD_MS), vScheduler);
  vTaskStart (xScheduler);

  for (;;) {

    vAx25Poll (&ax25);

    if (xMutexTryLock(&xMutexTx) == 0) {
      static uint16_t usCount;
      char msg[80];
      // Numérotation de la trame afin de suivre la perte éventuelle à la
      // réception
      snprintf (msg, sizeof(msg), APRS_MSG, usCount++);

      vAx25SendVia (&ax25, path, countof(path), msg, strlen(msg));
    }
  }
  return 0;
}
