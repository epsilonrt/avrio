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
 * Envoie une trame Tnc de façon périodique et affiche les trames reçues
 * sur la liaison série.
 *
 *

   ========================================================================== */
#include <avrio/led.h>
#include <avrio/delay.h>
#include <avrio/tnc.h>
#include <avrio/serial.h>
#include <avrio/task.h>
#include <avrio/mutex.h>

/* constants ================================================================ */
// Les constantes ci_dessous peuvent être modifiées au besoin par l'utilisateur
// Baudrate de la liaison série en baud
#define SER_BAUDRATE  38400
// Période d'envoi des trames en ms
#define TRANSMIT_PERIOD  300

/* private variables ======================================================== */
static xTnc tnc;
static uint8_t msg[TNC_RXBUFSIZE];
static xTaskHandle xScheduler;
static xMutex xMutexTx = MUTEX_LOCK;

/* private functions ======================================================== */

// ------------------------------------------------------------------------------
static void
vAssert (bool test) {

  if (test == false) {

    for (;;) {

      vLedSet (LED_LED2);
      delay_ms (50);
      vLedClear (LED_LED2);
      delay_ms (150);
    }
  }
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
  int i;

  vLedInit ();
  vSerialInit (SER_BAUDRATE/100, SERIAL_DEFAULT + SERIAL_RW + SERIAL_NOBLOCK);
  vTncInit (&tnc, &xSerialPort, &xSerialPort);
  for (i = 0; i < TNC_RXBUFSIZE; i++)
    msg[i] = i;
  xScheduler = xTaskCreate (xTaskConvertMs (TRANSMIT_PERIOD), vScheduler);
  vTaskStart (xScheduler);

  for (;;) {


    i = iTncPoll (&tnc);
    vAssert (i >= 0);
    if (i == TNC_EOT) {

      for (i = 0; i < tnc.len; i++)
        vAssert (tnc.rxbuf[i] == i);
      vLedToggle (LED_LED1);
    }

    if (xMutexTryLock(&xMutexTx) == 0) {

      i = iTncWrite (&tnc, msg, sizeof(msg));
      vAssert (i == sizeof(msg));
    }
  }
  return 0;
}
