/**
 * @file serial_irq.c
 * @brief Liaison série asynchrone version avec interruption
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
#include "serial_irq_private.h"

#if AVRIO_SERIAL_FLAVOUR & SERIAL_FLAVOUR_IRQ
/* ========================================================================== */
#include <avrio/queue.h>
#include <util/atomic.h>

/* private variables ======================================================== */
QUEUE_STATIC_DECLARE (xSerialRxQueue, SERIAL_RXBUFSIZE);
QUEUE_STATIC_DECLARE (xSerialTxQueue, SERIAL_TXBUFSIZE);
static volatile bool bIsStopped;

/* interrupt service routines =============================================== */

// ------------------------------------------------------------------------------
ISR (USART_RXC_vect) {

  if (bSerialIsRxError()) {

    // Erreur parité ou format
    (void) UDR; /* clear usFlags en cas d'erreur */
  }
  else {

    // Pas d'erreur ... Mémorise l'octet reçu dans la pile de réception
    vQueuePush (&xSerialRxQueue, UDR);
    if (xQueueIsFull (&xSerialRxQueue)) {

      // file réception pleine
      vRtsDisable(); // Indique à l'envoyeur de stopper ou signale l'erreur
      vRxIrqDisable();
      bIsStopped = true;
    }
  }
}

// ------------------------------------------------------------------------------
ISR (USART_UDRE_vect) {

  if (xQueueIsEmpty (&xSerialTxQueue)) {

    // La pile de transmission est vide, invalide l'interruption TX Buffer vide
    UCSRB &= ~_BV (UDRIE);
    // Valide l'interruption TX Complete pour terminer la transmission
    UCSRB |= _BV (TXCIE);
  }
  else {

    // La pile de transmission contient des données
    // Teste si le receveur est prêt (CTS=0)...
    if (bCtsIsEnabled()) {
      uint8_t ucNextByte;

      // ... et envoie le prochain octet
      ucNextByte = ucQueuePull (&xSerialTxQueue);
      UDR = ucNextByte;
    }
  }
}

// ------------------------------------------------------------------------------
ISR (USART_TXC_vect) {

  // Transmission terminée, on invalide la transmission
  vSerialPrivateTxEn (false);
}

/* internal public functions ================================================ */

// -----------------------------------------------------------------------------
void
vSerialPrivateInit (uint16_t usBaud, uint16_t usFlags) {

  vQueueFlush (&xSerialRxQueue);
  vQueueFlush (&xSerialTxQueue);
}

// -----------------------------------------------------------------------------
// Retourne le caractère comme un unsigned ou _FDEV_ERR en cas d'erreur ou
// _FDEV_EOF si aucun caractère reçu
int
iSerialPrivateGetChar (void) {
  int c = _FDEV_EOF;

  if ( (bIsStopped) && xQueueIsEmpty (&xSerialRxQueue)) {
    /*
     * Réception stoppée suite à pile pleine, maintenant la pile est vide, 
     * on peut revalider la réception
     */
    ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {
      bIsStopped = false;
      vRxIrqEnable();
      vRtsEnable();
    }
  }
  
  if (usSerialFlags & SERIAL_NOBLOCK) {

    // Version non-bloquante, renvoie _FDEV_EOF si rien reçu
    if (xQueueIsEmpty (&xSerialRxQueue) == false) {

      // la pile de réception contient des octets, on extrait le premier
      ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {

        c = ucQueuePull (&xSerialRxQueue);
      }
    }
  }
  else {

    // Version bloquante, on attends tant que rien n'est reçue
    while (xQueueIsEmpty (&xSerialRxQueue))
      ;
    ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {

      c = ucQueuePull (&xSerialRxQueue);
    }
  }
  return (unsigned int) c;
}

// -----------------------------------------------------------------------------
int
iSerialPrivatePutChar (char c) {

  // Attente tant que la pile de transmission est pleine
  while (xQueueIsFull (&xSerialTxQueue))
    ;
  ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {

    vQueuePush (&xSerialTxQueue, c);
    vSerialPrivateTxEn (true);
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
      vSerialPrivateTxEn (true);
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
vSerialFlush (void) {

  // Attente fin de transmission
  while (xSerialReady() == false)
    ;
  vQueueFlush (&xSerialTxQueue);
  vQueueFlush (&xSerialRxQueue);
}
/* ========================================================================== */
#endif /* AVRIO_SERIAL_FLAVOUR & SERIAL_FLAVOUR_IRQ */

#if AVRIO_SERIAL_FLAVOUR == SERIAL_FLAVOUR_IRQ
/* ========================================================================== */
// ------------------------------------------------------------------------------
void
vSerialPrivateTxEn (bool bTxEn) {

  if (bTxEn) {

    // Valide la transmission
    vTxEnSet ();
    vTxEnable();
    vTxIrqEnable();
  }
  else {

    // Invalide la transmission
    vTxIrqDisable();
    vTxDisable();
    vTxEnClear ();
  }
}

// -----------------------------------------------------------------------------
void
vSerialPrivateRxEn (bool bRxEn) {

  if (bRxEn) {

    // Valide la réception
    vRxEnSet();
    vRxEnable();
    vRxClearError();
    vRxIrqEnable();
  }
  else {

    // Invalide la réception
    vRxIrqDisable();
    vRxDisable();
    vRxEnClear ();
  }
}
/* ========================================================================== */
#endif /* AVRIO_SERIAL_FLAVOUR == SERIAL_FLAVOUR_IRQ */
