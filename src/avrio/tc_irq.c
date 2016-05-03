/**
 * @file tc_irq.c
 * @brief Liaison série asynchrone version avec interruption
 *
 * Copyright © 2011-2015 epsilonRT. All rights reserved.
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

#if (AVRIO_TC_FLAVOUR == TC_FLAVOUR_IRQ)
/* ========================================================================== */
#include <avrio/delay.h>
#include <util/atomic.h>

/* constants ================================================================ */
//#define TC_IRQ_DEBUG_TXEN

/* private variables ======================================================== */
/*
 * Table d'indirection utilisée par les routines d'interruption
 */
static xTcPort * xIrqToPort[] = {
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
};

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

/* macros =================================================================== */
#ifdef TC_IRQ_DEBUG_TXEN
#include <avrio/led.h>
// Debug validation transmetteur avec leds 4 à 7 (UART0 à UART3)
#define vDebugClearTxenLed() vLedClear(xLedGetMask (p->uart+ 4));
#define vDebugSetTxenLed() vLedSet(xLedGetMask (p->uart + 4));
#else
#define vDebugClearTxenLed()
#define vDebugSetTxenLed()
#endif

/* private functions ======================================================== */

// -----------------------------------------------------------------------------
static void
vUartEnableRxIrq (xTcPort * p) {

  TC_UCSRB |= _BV (RXCIE);
}

// -----------------------------------------------------------------------------
static void
vUartDisableRxIrq (xTcPort * p) {

  TC_UCSRB &= ~_BV (RXCIE);
}

// -----------------------------------------------------------------------------
static void
vUartEnableUdreIrq (xTcPort * p) {

  TC_UCSRB &= ~_BV (TXCIE);
  TC_UCSRB |= _BV (UDRIE);
}

// -----------------------------------------------------------------------------
static void
vUartEnableTxcIrq (xTcPort * p) {

  TC_UCSRB &= ~_BV (UDRIE);
  TC_UCSRB |= _BV (TXCIE);
}

// -----------------------------------------------------------------------------
static void
vUartDisableTxIrqs (xTcPort * p) {

  TC_UCSRB &= ~ (_BV (TXCIE) | _BV (UDRIE));
}

/* -----------------------------------------------------------------------------
 * Valide/Invalide Réception et/ou Transmission
 *                      iTxEn         iRxEn
 * false                dévalidé      dévalidé
 * true                 validé        validé
 * ni false, ni true    non modifié   non modifié
 */
static void
vTxRxEnable (int8_t iTxEn, int8_t iRxEn, xTcPort * p) {

  if (iRxEn == false) {

    // Invalide la réception
    vUartDisableRxIrq (p);
    if (p->hook->flag & O_HDUPLEX) {

      vUartDisableRx (p);
    }
    vRxenClear (p);
  }
  if (iTxEn == true) {

    // Valide la transmission
    vTxenSet (p);
    vDebugSetTxenLed();
    vUartEnableTx (p);
    vUartEnableUdreIrq (p);
  }
  if (iTxEn == false) {

    // Invalide la transmission
    // delay_ms(1);
    vUartDisableTxIrqs (p);
    if (p->hook->flag & O_HDUPLEX) {

      vUartDisableTx (p);
    }
    vTxenClear (p);
    vDebugClearTxenLed();
  }
  if (iRxEn == true) {

    // Valide la réception
    vRxenSet (p);
    vUartEnableRx (p);
    vUartClearRxError (p);
    vUartEnableRxIrq (p);
  }
}

#if TC_NUMOF_PORT > 1
// -----------------------------------------------------------------------------
static void
vUartDisableUdreIrq (xTcPort * p) {

  TC_UCSRB &= ~_BV (UDRIE);
}

/* -----------------------------------------------------------------------------
 * Algorithme round-robin
 * Si un autre UART a des caractères à transmettre, on dévalide l'irq
 * du transmetteur en cours afin de passer au suivant
 */
static void
vSwitchTxUart (uint8_t irq) {
  xTcPort * p = xIrqToPort[irq];

  uint8_t next = (irq + 1) & (TC_NUMOF_PORT - 1);
  while (next != irq) {

    xTcPort * n =  xIrqToPort[next];
    if (n) {  // Port initialisé
      xTcIrqDcb * nd = pxTcIrqDcb (n);

      if (nd->txen == true) {
        // des données à transmettre par le suivant... on switche

        // Mise en pause UART actif
        vUartDisableUdreIrq (p);

        // Activation du suivant
        vUartEnableUdreIrq (n);
        break;
      }
    }
    next = (next + 1) & (TC_NUMOF_PORT - 1);
  }
}
#else
#define vSwitchTxUart(i)
#endif

/* interrupt service routines =============================================== */

//------------------------------------------------------------------------------
static void
vIsrRxComplete (uint8_t irq) {
  xTcPort * p = xIrqToPort[irq];
  xTcIrqDcb * d = pxTcIrqDcb (p);

  if (iTcRxError (p)) {

    // Erreur parité ou format
    (void) TC_UDR; /* clear usFlags en cas d'erreur */
  }
  else {

    // Pas d'erreur ... Mémorise l'octet reçu dans la pile de réception
    vQueuePush (d->rxbuf, TC_UDR);
    if (xQueueIsFull (d->rxbuf)) {

      // file réception pleine
      vRtsDisable (p); // Indique à l'envoyeur de stopper ou signale l'erreur
      vUartDisableRxIrq (p);
      d->rxstop = true;
    }
  }
}

//------------------------------------------------------------------------------
static void
vIsrUdrEmpty (uint8_t irq) {
  xTcPort * p = xIrqToPort[irq];
  xTcIrqDcb * d = pxTcIrqDcb (p);

  if (xQueueIsEmpty (d->txbuf)) {

    /*
     * La pile de transmission est vide, invalide l'interruption TX Buffer vide
     * et valide l'interruption TX Complete pour terminer la transmission
     */
    vUartEnableTxcIrq (p);
  }
  else {

    // La pile de transmission contient des données
    // Teste si le receveur est prêt (CTS=0)...
    if (bCtsIsEnabled (p)) {
      uint8_t ucNextByte;

      // ... et envoie le prochain octet
      ucNextByte = ucQueuePull (d->txbuf);
      TC_UDR = ucNextByte;
    }
  }
  vSwitchTxUart (irq);
}

//------------------------------------------------------------------------------
static void
vIsrTxComplete (uint8_t irq) {
  xTcPort * p = xIrqToPort[irq];
  xTcIrqDcb * d = pxTcIrqDcb (p);

  if (xQueueIsEmpty (d->txbuf)) {
    // Transmission terminée, on invalide la transmission
    vTcPrivTxEn (false, p);
  }
  else {
    // Des octets ont été ajoutés entre temps, on repart en transmission
    vUartEnableUdreIrq (p);
  }
}

/* internal public functions ================================================ */

// -----------------------------------------------------------------------------
void
vTcPrivInit (xTcPort * p) {
  xTcIrqDcb * d = &xIrqDcb[p->inode];

  d->rxstop = false;
  d->rxen = -1;
  d->txen = -1;
  vQueueFlush (d->rxbuf);
  vQueueFlush (d->txbuf);
  p->dcb = d; /* lien port -> irq */
  xIrqToPort[TC_UART] = p; /* lien irq -> port */
}

// -----------------------------------------------------------------------------
// Retourne le caractère comme un unsigned ou _FDEV_ERR en cas d'erreur ou
// _FDEV_EOF si aucun caractère reçu
int
iTcPrivGetChar (xTcPort * p) {
  int c = _FDEV_EOF;
  xTcIrqDcb * d = pxTcIrqDcb (p);

  if ( (d->rxstop) && xQueueIsEmpty (d->rxbuf)) {
    /*
     * Réception stoppée suite à pile pleine, maintenant la pile est vide,
     * on peut revalider la réception
     */
    ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {
      d->rxstop = false;
      vUartEnableRxIrq (p);
      vRtsEnable (p);
    }
  }

  if (p->hook->flag & O_NONBLOCK) {

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
iTcPrivPutChar (char c, xTcPort * p) {
  xTcIrqDcb * d = pxTcIrqDcb (p);

  // Attente tant que la pile de transmission est pleine
  while (xQueueIsFull (d->txbuf))
    ;

  ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {

    vQueuePush (d->txbuf, c);
    vTcPrivTxEn (true, p);
  }
  return 0;
}

// -----------------------------------------------------------------------------
uint16_t
usTcPrivDataAvailable (xTcPort * p) {
  uint16_t usSize;
  xTcIrqDcb * d = pxTcIrqDcb (p);

  ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {

    usSize = xQueueLength (d->rxbuf);
  }
  return usSize;
}

// -----------------------------------------------------------------------------
bool
xTcPrivReady (xTcPort * p) {

  return (TC_UCSRB & _BV (RXEN)) != 0;
}

// -----------------------------------------------------------------------------
void
vTcPrivFlush (xTcPort * p) {
  xTcIrqDcb * d = pxTcIrqDcb (p);

  // Attente fin de transmission
  while (xTcPrivReady (p) == false)
    ;
  vQueueFlush (d->txbuf);
  vQueueFlush (d->rxbuf);
}

// -----------------------------------------------------------------------------
void
vTcPrivTxEn (bool bTxEn, xTcPort * p) {
  xTcIrqDcb * d = pxTcIrqDcb (p);

  if ( (int8_t) bTxEn != d->txen) {
    // Modifie l'état du l'USART uniquement si il est différent

    ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {
      vTxRxEnable (bTxEn, ( (p->hook->flag & O_HDUPLEX) ? !bTxEn : -1), p);
      d->txen = bTxEn;
      if (p->hook->flag & O_HDUPLEX) {

        d->rxen = !bTxEn;
      }
    }
  }
}

// -----------------------------------------------------------------------------
void
vTcPrivRxEn (bool bRxEn, xTcPort * p) {
  xTcIrqDcb * d = pxTcIrqDcb (p);

  if ( (int8_t) bRxEn != d->rxen) {
    // Modifie l'état du l'USART uniquement si il est différent

    ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {
      vTxRxEnable ( ( (p->hook->flag & O_HDUPLEX) ? !bRxEn : -1), bRxEn, p);
      d->rxen = bRxEn;
      if (p->hook->flag & O_HDUPLEX) {

        d->txen = !bRxEn;
      }
    }
  }
}

/* interrupt service routines =============================================== */

//------------------------------------------------------------------------------
ISR (TC0_RX_vect) {

  vIsrRxComplete (0);
}

//------------------------------------------------------------------------------
ISR (TC0_UDRE_vect) {

  vIsrUdrEmpty (0);
}

//------------------------------------------------------------------------------
ISR (TC0_TX_vect) {

  vIsrTxComplete (0);
}

#ifdef TC1_IO
//------------------------------------------------------------------------------
ISR (TC1_RX_vect) {

  vIsrRxComplete (1);
}

//------------------------------------------------------------------------------
ISR (TC1_UDRE_vect) {

  vIsrUdrEmpty (1);
}

//------------------------------------------------------------------------------
ISR (TC1_TX_vect) {

  vIsrTxComplete (1);
}
//------------------------------------------------------------------------------
#endif /* TC1_IO defined */

#ifdef TC2_IO
//------------------------------------------------------------------------------
ISR (TC2_RX_vect) {

  vIsrRxComplete (2);
}

//------------------------------------------------------------------------------
ISR (TC2_UDRE_vect) {

  vIsrUdrEmpty (2);
}

//------------------------------------------------------------------------------
ISR (TC2_TX_vect) {

  vIsrTxComplete (2);
}
//------------------------------------------------------------------------------
#endif /* TC2_IO defined */

#ifdef TC3_IO
//------------------------------------------------------------------------------
ISR (TC3_RX_vect) {

  vIsrRxComplete (3);
}

//------------------------------------------------------------------------------
ISR (TC3_UDRE_vect) {

  vIsrUdrEmpty (3);
}

//------------------------------------------------------------------------------
ISR (TC3_TX_vect) {

  vIsrTxComplete (3);
}
//------------------------------------------------------------------------------
#endif /* TC3_IO defined */

/* ========================================================================== */
#endif /* (AVRIO_TC_FLAVOUR == TC_FLAVOUR_IRQ) */
#endif /* AVRIO_TC_ENABLE defined */
