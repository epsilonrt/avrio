/**
 * @file serial_irq.c
 * @brief Liaison série asynchrone version avec interruption
 *
 * Copyright © 2011-2015 epsilonRT. All rights reserved.
 *
 * This file is part of AvrIO.
 *
 * This software is governed by the CeCILL license under French law and
 * abiding by the rules of distribution of free software.  You can  use, 
 * modify and/ or redistribute the software under the terms of the CeCILL
 * license as circulated by CEA, CNRS and INRIA at the following URL
 * <http://www.cecill.info>. 
 * 
 * The fact that you are presently reading this means that you have had
 * knowledge of the CeCILL license and that you accept its terms.
 */
#include "serial_irq_private.h"

#if AVRIO_SERIAL_FLAVOUR & SERIAL_FLAVOUR_IRQ
/* ========================================================================== */
#include <avrio/queue.h>
#include <avrio/delay.h>
#include <util/atomic.h>

/* public variables ========================================================= */
int8_t iSerialTxEn;
int8_t iSerialRxEn;

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

    /*
     * La pile de transmission est vide, invalide l'interruption TX Buffer vide
     * et valide l'interruption TX Complete pour terminer la transmission
     */
    vTxTxcIrqEnable();
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

  if (xQueueIsEmpty (&xSerialTxQueue)) {
    // Transmission terminée, on invalide la transmission
    vSerialPrivateTxEn (false);
  }
  else {
    // Des octets ont été ajoutés entre temps, on repart en transmission
    vTxUdreIrqEnable ();
  }
}

/* internal public functions ================================================ */

// -----------------------------------------------------------------------------
void
vSerialPrivateInit (uint16_t usBaud, uint16_t usFlags) {

  bIsStopped = false;
  iSerialTxEn = -1;
  iSerialRxEn = -1;
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
// Retourne 0 en cas de succès
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
#elif SERIAL_EOL == SERIAL_CR
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

  if ( (int8_t) bTxEn != iSerialTxEn) {
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
    iSerialTxEn = bTxEn;
  }
}

// -----------------------------------------------------------------------------
void
vSerialPrivateRxEn (bool bRxEn) {

  if ( (int8_t) bRxEn != iSerialRxEn) {
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
    iSerialRxEn = bRxEn;
  }
}
/* ========================================================================== */
#endif /* AVRIO_SERIAL_FLAVOUR == SERIAL_FLAVOUR_IRQ */
