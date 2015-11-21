/**
 * @file tc_irq.c
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
#include "avrio-config.h"

#ifdef AVRIO_TC_ENABLE
/* ========================================================================== */
#include "tc_irq_private.h"
#include <avrio/delay.h>
#include <util/atomic.h>

/* private variables ======================================================== */
/*
 * Table d'indirection utilisée par les routines d'interruption
 */
static xFileHook * xIrqHook[] = {
#ifdef TC0_RX_vect
  NULL,
#endif  
#ifdef TC1_RX_vect
  NULL,
#endif  
#ifdef TC2_RX_vect
  NULL,
#endif  
#ifdef TC3_RX_vect
  NULL,
#endif  
}

/*
 * Piles fifo d'emission et réception
 */
QUEUE_STATIC_DECLARE (xTcRxQueue0, TC_RXBUFSIZE);
QUEUE_STATIC_DECLARE (xTcTxQueue0, TC_TXBUFSIZE);
#if TC_NUMOF_PORT > 1
QUEUE_STATIC_DECLARE (xTcRxQueue1, TC_RXBUFSIZE);
QUEUE_STATIC_DECLARE (xTcTxQueue1, TC_TXBUFSIZE);
#endif
#if TC_NUMOF_PORT > 2
QUEUE_STATIC_DECLARE (xTcRxQueue2, TC_RXBUFSIZE);
QUEUE_STATIC_DECLARE (xTcTxQueue2, TC_TXBUFSIZE);
#endif
#if TC_NUMOF_PORT > 3
QUEUE_STATIC_DECLARE (xTcRxQueue3, TC_RXBUFSIZE);
QUEUE_STATIC_DECLARE (xTcTxQueue3, TC_TXBUFSIZE);
#endif

/*
 * Table des DCB d'interruption UART
 */
static xTcIrqDcb xIrqDcb[TC_NUMOF_PORT] = {
  { .rxbuf = &xTcRxQueue0, .txbuf = &xTcTxQueue0},
#if TC_NUMOF_PORT > 1
  { .rxbuf = &xTcRxQueue1, .txbuf = &xTcTxQueue1},
#endif
#if TC_NUMOF_PORT > 2
  { .rxbuf = &xTcRxQueue1, .txbuf = &xTcTxQueue2},
#endif
#if TC_NUMOF_PORT > 3
  { .rxbuf = &xTcRxQueue1, .txbuf = &xTcTxQueue3},
#endif
};

/* interrupt service routines =============================================== */

//------------------------------------------------------------------------------
static void 
vIsrRxComplete (xFileHook * h) {

  if (bTcIsRxError(p)) {

    // Erreur parité ou format
    (void) TC_UDR; /* clear usFlags en cas d'erreur */
  }
  else {

    // Pas d'erreur ... Mémorise l'octet reçu dans la pile de réception
    vQueuePush (d->rxbuf, TC_UDR);
    if (xQueueIsFull (d->rxbuf)) {

      // file réception pleine
      vRtsDisable(h); // Indique à l'envoyeur de stopper ou signale l'erreur
      vRxIrqDisable();
      d->stopped = true;
    }
  }
}

//------------------------------------------------------------------------------
static void 
vIsrUdrEmpty (xFileHook * h) {

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

//------------------------------------------------------------------------------
static void 
vIsrTxComplete (xFileHook * h) {

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
vTcPrivateInit (xFileHook * h) {
  xTcIrqDcb * d = &xIrqDcb[h->inode];
  
  d->stopped = false;
  d->rxen = -1;
  d->txen = -1;
  vQueueFlush (d->rxbuf);
  vQueueFlush (d->txbuf);
  p->dcb = d; /* lien port -> irq */
  xIrqHook[TC_UART] = h; /* lien irq -> port */
}

// -----------------------------------------------------------------------------
// Retourne le caractère comme un unsigned ou _FDEV_ERR en cas d'erreur ou
// _FDEV_EOF si aucun caractère reçu
int
iTcPrivateGetChar (xFileHook * h) {
  int c = _FDEV_EOF;
  xTcPort * p = (xTcPort *) h->dev;
  xTcIrqDcb * d = (xTcIrqDcb *) p->deb;

  if ( (d->stopped) && xQueueIsEmpty (d->rxbuf)) {
    /*
     * Réception stoppée suite à pile pleine, maintenant la pile est vide,
     * on peut revalider la réception
     */
    ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {
      d->stopped = false;
      vRxIrqEnable(p);
      vRtsEnable();
    }
  }

  if (usTcFlags & O_NONBLOCK) {

    // Version non-bloquante, renvoie _FDEV_EOF si rien reçu
    if (xQueueIsEmpty (d->rxbuf) == false) {

      // la pile de réception contient des octets, on extrait le premier
      ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {

        c = ucQueuePull (d->rxbuf);
      }
    }
  }
  else {

    // Version bloquante, on attends tant que rien n'est reçue
    while (xQueueIsEmpty (d->rxbuf))
      ;
    ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {

      c = ucQueuePull (d->rxbuf);
    }
  }
  return (unsigned int) c;
}

// -----------------------------------------------------------------------------
// Retourne 0 en cas de succès
int
iTcPrivatePutChar (char c, xFileHook * h) {

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
uint16_t
usTcHit (xFileHook * h) {
  uint16_t usSize;

  ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {

    usSize = xQueueLength (d->rxbuf);
  }
  return usSize;
}

// -----------------------------------------------------------------------------
bool
xTcReady (xFileHook * h) {

  return (TC_UCSRB & _BV (RXEN)) != 0;
}

// -----------------------------------------------------------------------------
void
vTcFlush (xFileHook * h) {

  // Attente fin de transmission
  while (xTcReady() == false)
    ;
  vQueueFlush (&xTcTxQueue);
  vQueueFlush (d->rxbuf);
}

//------------------------------------------------------------------------------
void
vTcPrivateTxEn (bool bTxEn, xFileHook * h) {

  if ( (int8_t) bTxEn != p->txen) {
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
    p->txen = bTxEn;
  }
}

// -----------------------------------------------------------------------------
void
vTcPrivateRxEn (bool bRxEn, xFileHook * h) {

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

/* interrupt service routines =============================================== */

//------------------------------------------------------------------------------
ISR (TC0_RX_vect) {
  vIsrRxComplete(&xIrqHook[0]);
}

//------------------------------------------------------------------------------
ISR (TC0_UDRE_vect) {
  vIsrUdrEmpty(&xIrqHook[0]);
}

//------------------------------------------------------------------------------
ISR (TC0_TX_vect) {
  vIsrTxComplete(&xIrqHook[0]);
}

#ifdef TC1_IO
//------------------------------------------------------------------------------
ISR (TC1_RX_vect) {
  vIsrRxComplete(&xIrqHook[1]);
}

//------------------------------------------------------------------------------
ISR (TC1_UDRE_vect) {
  vIsrUdrEmpty(&xIrqHook[1]);
}

//------------------------------------------------------------------------------
ISR (TC1_TX_vect) {
  vIsrTxComplete(&xIrqHook[1]);
}
//------------------------------------------------------------------------------
#endif /* TC1_IO defined */

#ifdef TC2_IO
//------------------------------------------------------------------------------
ISR (TC2_RX_vect) {
  vIsrRxComplete(&xIrqHook[2]);
}

//------------------------------------------------------------------------------
ISR (TC2_UDRE_vect) {
  vIsrUdrEmpty(&xIrqHook[2]);
}

//------------------------------------------------------------------------------
ISR (TC2_TX_vect) {
  vIsrTxComplete(&xIrqHook[2]);
}
//------------------------------------------------------------------------------
#endif /* TC2_IO defined */

#ifdef TC3_IO
//------------------------------------------------------------------------------
ISR (TC3_RX_vect) {
  vIsrRxComplete(&xIrqHook[3]);
}

//------------------------------------------------------------------------------
ISR (TC3_UDRE_vect) {
  vIsrUdrEmpty(&xIrqHook[3]);
}

//------------------------------------------------------------------------------
ISR (TC3_TX_vect) {
  vIsrTxComplete(&xIrqHook[3]);
}
//------------------------------------------------------------------------------
#endif /* TC3_IO defined */

/* ========================================================================== */
#endif /* AVRIO_TC_ENABLE defined */
