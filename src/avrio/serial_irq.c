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
 * @file serial_irq.c
 * @ingroup serial_module
 * @brief Liaison série asynchrone version avec interruption
 *
 * Dépend des modules:
 * - \ref queue_module (version avec interruption seulement)
 * .
 *

   ========================================================================== */
#include "avrio-config.h"

#ifdef AVRIO_SERIAL_ENABLE
/* ========================================================================== */
#include <avrio/queue.h>
#include <avrio/mutex.h>
#include <util/atomic.h>
#include <avrio/delay.h>
#include "serial_private.h"

#ifdef AVRIO_SERIAL_RXIE
/* -----------------------------------------------------------------------------
 *
 *                         Réception avec interruption
 *
 * ---------------------------------------------------------------------------*/

/* private variables ======================================================== */
QUEUE_DECLARE (xSerialRxQueue, SERIAL_RXBUFSIZE);

/* private functions ======================================================== */

// ------------------------------------------------------------------------------
ISR (USART_RXC_vect) {

  if ( (UCSRA & (_BV (PE) | _BV (FE))) == 0) {

    if (xQueueIsFull (&xSerialRxQueue)) {

      // file réception pleine
      vRtsDisable();    // Indique à l'envoyeur de stopper
      vRxIrqDisable();  // Inutile de continuer tant que la file n'a pas été vidée
    }
    else {

      vQueuePush (&xSerialRxQueue, UDR);
    }
  }
  else {

    (void) UDR; /* clear usFlags en cas d'erreur */
  }
}

/* internal public functions ================================================ */

// -----------------------------------------------------------------------------
int
iSerialGetChar (void) {
  int iChar = _FDEV_EOF;

  if (usSerialFlags & SERIAL_RD) {

    if (xQueueIsEmpty (&xSerialRxQueue)) {

      // file de réception vide, activation de la réception
      vRxEnable();
    }

    if (usSerialFlags & SERIAL_NOBLOCK) {

      // Version non-bloquante, renvoie EOF si rien reçu
      if (xQueueIsEmpty (&xSerialRxQueue) == false) {

        ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {

          iChar = ucQueuePull (&xSerialRxQueue);
        }
      }
    }
    else {

      // Version bloquante, on attends tant que rien n'est reçue
      while (xQueueIsEmpty (&xSerialRxQueue))
        ;
      ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {

        iChar = ucQueuePull (&xSerialRxQueue);
      }
    }
  }
  return (unsigned int) iChar;
}

// -----------------------------------------------------------------------------
uint16_t
usSerialHit (void) {
  uint16_t usSize;

  ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {

    usSize = xQueueLength (&xSerialRxQueue);
  }
  return usSize;
}

// ------------------------------------------------------------------------------
#endif  /* AVRIO_SERIAL_RXIE defined */

#ifdef AVRIO_SERIAL_TXIE
/* -----------------------------------------------------------------------------
 *
 *                         Transmission avec interruption
 *
 * ---------------------------------------------------------------------------*/

/* private variables ======================================================== */
QUEUE_DECLARE (xSerialTxQueue, SERIAL_TXBUFSIZE);

/* private functions ======================================================== */

// ------------------------------------------------------------------------------
ISR (USART_UDRE_vect) {

  if (xQueueIsEmpty (&xSerialTxQueue) == false) {

    if (bCtsIsEnabled() == true) {

      uint8_t ucNextByte;

      ucNextByte = ucQueuePull (&xSerialTxQueue);
      UDR = ucNextByte;
    }
  }
  else {

    UCSRB &= ~_BV (UDRIE);
    UCSRB |= _BV (TXCIE);
  }
}

// ------------------------------------------------------------------------------
ISR (USART_TXC_vect) {

  if (xQueueIsEmpty (&xSerialTxQueue)) {

    vTxDisable();
  }
}

/* internal public functions ================================================ */

// -----------------------------------------------------------------------------
int
iSerialPutChar (char c) {

  if (usSerialFlags & SERIAL_WR) {

#if defined(CONFIG_EOL_CRLF)
    if (c == '\n') {
      (void) iSerialPutChar ('\r');
    }
#elif defined(CONFIG_EOL_CR)
    if (c == '\n') {
      c = '\r';
    }
#elif defined(CONFIG_EOL_LF)
    if (c == '\r') {
      c = '\n';
    }
#endif

    while (xQueueIsFull (&xSerialTxQueue))
      ;
    ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {

      vQueuePush (&xSerialTxQueue, c);
      vTxEnable();
    }
  }
  else {

    return -1;
  }
  return 0;
}

// -----------------------------------------------------------------------------
void
vSerialPutString (const char *pcString) {

  if (usSerialFlags & SERIAL_WR) {

    while (*pcString) {

      ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {

        pcString = pcQueuePushString (&xSerialTxQueue, pcString);
      }
      vTxEnable();
    }
  }
}
// ------------------------------------------------------------------------------
#endif  /* AVRIO_SERIAL_TXIE defined */

/* ========================================================================== */
#endif /* AVRIO_SERIAL_ENABLE defined */
