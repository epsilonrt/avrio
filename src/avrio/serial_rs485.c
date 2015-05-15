/**
 * @file serial_irq.c
 * @ingroup serial_module
 * @brief Liaison série asynchrone version avec interruption
 *
 * Dépend des modules:
 * - \ref queue_module (version avec interruption seulement)
 * .
 *
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
 */
#include "avrio-config.h"

#ifdef AVRIO_SERIAL_ENABLE
/* ========================================================================== */
#include <avrio/queue.h>
#include <avrio/mutex.h>
#include <util/atomic.h>
#include <avrio/delay.h>
#include "serial_rs485_private.h"

/* private variables ======================================================== */
QUEUE_STATIC_DECLARE (xSerialRxQueue, SERIAL_RXBUFSIZE);
QUEUE_STATIC_DECLARE (xSerialTxQueue, SERIAL_TXBUFSIZE);

/*
 * Mutex pour une transmission en half-duplex
 * La transmission et la réception est impossible simultanément
 * Mutex non vérouillé au départ
 */
xMutex xSerialMutex = MUTEX_INITIALIZER;

/* private functions ======================================================== */
/* -----------------------------------------------------------------------------
 * Effectue le multiplexage half-duplex
 * A l'init, le module est en réception, un appel à putchar() bascule en
 * transmission, le retour en réception s'effectue lorsque la pile de
 * transmission est vide et que la transmission est terminée.
 */
static void
vTxEnable (bool bTxEnable) {

  if (bTxEnable) {

    // Invalide la réception
    UCSRB &= ~_BV (RXCIE);
    UCSRB &= ~_BV (RXEN);
    vRxEnClear ();
    // Valide la transmission
    vTxEnSet ();
    UCSRB |= _BV (TXEN);
    UCSRB &= ~_BV (TXCIE);
    UCSRB |= _BV (UDRIE);
  }
  else {

    // Invalide la transmission
    UCSRB &= ~ (_BV (TXCIE) | _BV (UDRIE));
    UCSRB &= ~_BV (TXEN);
    vTxEnClear ();
    // Valide la réception
    vRxEnSet();
    UCSRB |= _BV (RXEN);
    if ( (UCSRA & (_BV (PE) | _BV (FE))) != 0) {

      (void) UDR; /* clear des flags d'erreur */
    }
    UCSRB |= _BV (RXCIE);
  }
}

/* interrupt service routines =============================================== */

// ------------------------------------------------------------------------------
ISR (USART_RXC_vect) {

  if ( (UCSRA & (_BV (PE) | _BV (FE))) == 0) {

    if (xQueueIsFull (&xSerialRxQueue) == false) {

      vQueuePush (&xSerialRxQueue, UDR);
      return;
    }
  }
  (void) UDR; /* clear usFlags en cas d'erreur */
}

// ------------------------------------------------------------------------------
ISR (USART_UDRE_vect) {

  if (xQueueIsEmpty (&xSerialTxQueue) == false) {
    uint8_t ucNextByte;

    ucNextByte = ucQueuePull (&xSerialTxQueue);
    UDR = ucNextByte;
  }
  else {

    // La pile de transmission est vide, invalide l'interruption TX Buffer vide
    UCSRB &= ~_BV (UDRIE);
    // Valide l'interruption TX Complete pour revalider la réception
    UCSRB |= _BV (TXCIE);
  }
}

// ------------------------------------------------------------------------------
ISR (USART_TXC_vect) {

  vTxEnable (false);
}

/* internal public functions ================================================ */

// -----------------------------------------------------------------------------
void
vSerialPrivateInit (uint16_t usBaud, uint16_t usFlags) {

#ifdef USART_TXPIN
  USART_PORT &= ~_BV (USART_TXPIN);
  USART_DDR  |= _BV (USART_TXPIN);
#endif
#ifdef USART_RXPIN
  USART_PORT |= _BV (USART_RXPIN);
  USART_DDR  &= ~_BV (USART_RXPIN);
#endif

  vQueueFlush (&xSerialRxQueue);
  vQueueFlush (&xSerialTxQueue);
  vTxEnInit ();
  vRxEnInit ();
  vTxEnable (false);
}

// -----------------------------------------------------------------------------
int
iSerialGetChar (void) {
  int iChar = _FDEV_EOF;

  if (usSerialFlags & SERIAL_RD) {

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
int
iSerialPrivatePutChar (char c) {

  while (xQueueIsFull (&xSerialTxQueue))
    ;
  ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {

    vQueuePush (&xSerialTxQueue, c);
    vTxEnable (true);
  }
  return 0;
}

// -----------------------------------------------------------------------------
void
vSerialPutString (const char *pcString) {

  if (usSerialFlags & SERIAL_WR) {

    while (*pcString) {

      ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {
        while ( (xQueueIsFull (&xSerialTxQueue) == false) && (*pcString)) {
          uint8_t c = *pcString;
#if SERIAL_EOL == SERIAL_CRLF
          if (c == '\n') {

            vQueuePush (&xSerialTxQueue, '\r');
          }
#elif SERIAL_EOL == SERIAL_LF
          if (c == '\r') {
            c = '\n';
          }
#else // default = SERIAL_CR
          if (c == '\n') {
            c = '\r';
          }
#endif
          if (xQueueIsFull (&xSerialTxQueue) == false) {

            vQueuePush (&xSerialTxQueue, c);
            pcString++;
          }
        }
      }
      vTxEnable (true);
    }
  }
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

// -----------------------------------------------------------------------------
bool
xSerialReady (void) {

  return (UCSRB & _BV (RXEN)) != 0;
}

// -----------------------------------------------------------------------------
void 
vSerialFlush(void) {
  
  // Attente fin de transmission
  while (xSerialReady() == false)
    ;
  vQueueFlush (&xSerialTxQueue);
  vQueueFlush (&xSerialRxQueue);
}


/* ========================================================================== */
#endif /* AVRIO_SERIAL_ENABLE defined */
