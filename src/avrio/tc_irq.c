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
#include "tc_irq_private.h"

#if AVRIO_TC_FLAVOUR & TC_FLAVOUR_IRQ
/* ========================================================================== */
#include <avrio/queue.h>
#include <avrio/delay.h>
#include <util/atomic.h>

#define USART_TXC_vect  USART0_TX_vect
#define USART_UDRE_vect USART0_UDRE_vect
#define USART_RXC_vect  USART0_RX_vect
#define USART_TXC_vect  USART1_TX_vect
#define USART_UDRE_vect USART1_UDRE_vect
#define USART_RXC_vect  USART1_RX_vect

/* public variables ========================================================= */
int8_t iTcTxEn;
int8_t iTcRxEn;

/* private variables ======================================================== */
QUEUE_STATIC_DECLARE (xTcRxQueue, TC_RXBUFSIZE);
QUEUE_STATIC_DECLARE (xTcTxQueue, TC_TXBUFSIZE);
static volatile bool bIsStopped;

/* interrupt service routines =============================================== */

// ------------------------------------------------------------------------------
ISR (USART_RXC_vect) {

  if (bTcIsRxError(p)) {

    // Erreur parité ou format
    (void) TC_UDR; /* clear usFlags en cas d'erreur */
  }
  else {

    // Pas d'erreur ... Mémorise l'octet reçu dans la pile de réception
    vQueuePush (&xTcRxQueue, TC_UDR);
    if (xQueueIsFull (&xTcRxQueue)) {

      // file réception pleine
      vRtsDisable(h); // Indique à l'envoyeur de stopper ou signale l'erreur
      vRxIrqDisable();
      bIsStopped = true;
    }
  }
}

// ------------------------------------------------------------------------------
ISR (USART_UDRE_vect) {

  if (xQueueIsEmpty (&xTcTxQueue)) {

    /*
     * La pile de transmission est vide, invalide l'interruption TX Buffer vide
     * et valide l'interruption TX Complete pour terminer la transmission
     */
    vTxTxcIrqEnable();
  }
  else {

    // La pile de transmission contient des données
    // Teste si le receveur est prêt (CTS=0)...
    if (bCtsIsEnabled(h)) {
      uint8_t ucNextByte;

      // ... et envoie le prochain octet
      ucNextByte = ucQueuePull (&xTcTxQueue);
      TC_UDR = ucNextByte;
    }
  }
}

// ------------------------------------------------------------------------------
ISR (USART_TXC_vect) {

  if (xQueueIsEmpty (&xTcTxQueue)) {
    // Transmission terminée, on invalide la transmission
    vTcPrivateTxEn (false, h);
  }
  else {
    // Des octets ont été ajoutés entre temps, on repart en transmission
    vTxUdreIrqEnable ();
  }
}

/* internal public functions ================================================ */

// -----------------------------------------------------------------------------
void
vTcPrivateInit (uint16_t usBaud, uint16_t usFlags) {

  bIsStopped = false;
  iTcTxEn = -1;
  iTcRxEn = -1;
  vQueueFlush (&xTcRxQueue);
  vQueueFlush (&xTcTxQueue);
}

// -----------------------------------------------------------------------------
// Retourne le caractère comme un unsigned ou _FDEV_ERR en cas d'erreur ou
// _FDEV_EOF si aucun caractère reçu
int
iTcPrivateGetChar (void) {
  int c = _FDEV_EOF;

  if ( (bIsStopped) && xQueueIsEmpty (&xTcRxQueue)) {
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

  if (usTcFlags & O_NONBLOCK) {

    // Version non-bloquante, renvoie _FDEV_EOF si rien reçu
    if (xQueueIsEmpty (&xTcRxQueue) == false) {

      // la pile de réception contient des octets, on extrait le premier
      ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {

        c = ucQueuePull (&xTcRxQueue);
      }
    }
  }
  else {

    // Version bloquante, on attends tant que rien n'est reçue
    while (xQueueIsEmpty (&xTcRxQueue))
      ;
    ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {

      c = ucQueuePull (&xTcRxQueue);
    }
  }
  return (unsigned int) c;
}

// -----------------------------------------------------------------------------
// Retourne 0 en cas de succès
int
iTcPrivatePutChar (char c) {

  // Attente tant que la pile de transmission est pleine
  while (xQueueIsFull (&xTcTxQueue))
    ;
  ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {

    vQueuePush (&xTcTxQueue, c);
    vTcPrivateTxEn (true, h);
  }
  return 0;
}

// -----------------------------------------------------------------------------
void
vTcPutString (const char *pcString) {

  if (usTcFlags & TC_WR) {

    while (*pcString) {

      ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {
        while ( (xQueueIsFull (&xTcTxQueue) == false) && (*pcString)) {
          uint8_t c = *pcString;
#if TC_EOL == TC_CRLF
          if (c == '\n') {

            vQueuePush (&xTcTxQueue, '\r');
          }
#elif TC_EOL == TC_LF
          if (c == '\r') {
            c = '\n';
          }
#elif TC_EOL == TC_CR
          if (c == '\n') {
            c = '\r';
          }
#endif
          if (xQueueIsFull (&xTcTxQueue) == false) {

            vQueuePush (&xTcTxQueue, c);
            pcString++;
          }
        }
      }
      vTcPrivateTxEn (true, h);
    }
  }
}

// -----------------------------------------------------------------------------
uint16_t
usTcHit (void) {
  uint16_t usSize;

  ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {

    usSize = xQueueLength (&xTcRxQueue);
  }
  return usSize;
}

// -----------------------------------------------------------------------------
bool
xTcReady (void) {

  return (TC_UCSRB & _BV (RXEN)) != 0;
}

// -----------------------------------------------------------------------------
void
vTcFlush (void) {

  // Attente fin de transmission
  while (xTcReady() == false)
    ;
  vQueueFlush (&xTcTxQueue);
  vQueueFlush (&xTcRxQueue);
}
/* ========================================================================== */
#endif /* AVRIO_TC_FLAVOUR & TC_FLAVOUR_IRQ */

#if AVRIO_TC_FLAVOUR == TC_FLAVOUR_IRQ
/* ========================================================================== */

// ------------------------------------------------------------------------------
void
vTcPrivateTxEn (bool bTxEn) {

  if ( (int8_t) bTxEn != iTcTxEn) {
    // Modifie l'état du l'USART uniquement si il est différent

    if (bTxEn) {

      // Valide la transmission
      vTxEnSet ();
      vTxEnable();
      vTxUdreIrqEnable();
    }
    else {

      // Invalide la transmission
      vTxIrqDisable();
      vTxDisable();
      vTxEnClear ();
    }
    iTcTxEn = bTxEn;
  }
}

// -----------------------------------------------------------------------------
void
vTcPrivateRxEn (bool bRxEn) {

  if ( (int8_t) bRxEn != iTcRxEn) {
    // Modifie l'état du l'USART uniquement si il est différent

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
    iTcRxEn = bRxEn;
  }
}
/* ========================================================================== */
#endif /* AVRIO_TC_FLAVOUR == TC_FLAVOUR_IRQ */
