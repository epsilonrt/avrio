/**
 * @file serial_poll.c
 * @brief Liaison série asynchrone version sans interruption (scrutation)
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
#include "tc_private.h"

#if defined(AVRIO_TC_ENABLE) && (AVRIO_TC_FLAVOUR == TC_FLAVOUR_POLL)
/* ========================================================================== */
/*
 * Device Control Block pour scrutation
 */
typedef struct xTcPollDcb {

  int8_t txen;
  int8_t rxen;
} xTcPollDcb;

/*
 * Table des DCB de scrutation UART
 */
static xTcPollDcb xPollDcb[TC_NUMOF_PORT];

/* private functions ======================================================== */

// -----------------------------------------------------------------------------
INLINE xTcPollDcb *
pxTcPollDcb (xTcPort * p) {

  return (xTcPollDcb *) p->dcb;
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
    vUartDisableRx (p);
    vRxenClear (p);
  }
  if (iTxEn == true) {

    // Valide la transmission
    vTxenSet (p);
    vUartEnableTx (p);
  }
  if (iTxEn == false) {

    // Invalide la transmission
    // delay_ms(1);
    vUartDisableTx (p);
    vTxenClear (p);
  }
  if (iRxEn == true) {

    // Valide la réception
    vRxenSet (p);
    vUartEnableRx (p);
    vUartClearRxError (p);
  }
}

/* internal public functions ================================================ */

// -----------------------------------------------------------------------------
uint16_t
usTcPrivDataAvailable (xTcPort * p) {

  return ( (TC_UCSRA & _BV (RXC)) != 0);
}

// -----------------------------------------------------------------------------
bool
xTcPrivReady (xTcPort * p) {

  return (TC_UCSRB & _BV (RXEN)) != 0;
}

// -----------------------------------------------------------------------------
void
vTcPrivFlush (xTcPort * p) {

  // Aucun buffer à vider
}

/* internal private functions =============================================== */

// -----------------------------------------------------------------------------
void
vTcPrivInit (xTcPort * p) {
  xTcPollDcb * d = &xPollDcb[p->inode];

  d->rxen = -1;
  d->txen = -1;
  p->dcb = d; /* lien port -> dcb */
  vUartEnableTx (p);
  vRtsDisable (p);
}

// -----------------------------------------------------------------------------
// Retourne le caractère comme un unsigned ou _FDEV_ERR en cas d'erreur ou
// _FDEV_EOF si aucun caractère reçu
int
iTcPrivGetChar (xTcPort * p) {
  int c = _FDEV_EOF;
  int iError = 0;

  vRtsEnable (p);
  do {

    // Version non bloquante
    if (p->hook->flag & O_NONBLOCK) {

      if (TC_UCSRA & _BV (RXC)) {

        c = TC_UDR;
        iError = iTcRxError (p);
      }
    }
    else {

      // Version bloquante
      while ( (TC_UCSRA & _BV (RXC)) == 0)
        ;
      c = TC_UDR;
      iError = iTcRxError (p);
    }
  }
  while (iError != 0);
  vRtsDisable (p);
  return (unsigned int) c;
}

// -----------------------------------------------------------------------------
// Retourne 0 en cas de succès
int
iTcPrivPutChar (char c, xTcPort * p) {

  if ( (p->hook->flag & O_NONBLOCK) && (bCtsIsEnabled (p) == false)) {

    // Version non bloquante, receveur non prêt: on signale l'erreur
    return _FDEV_EOF;
  }
  else {

    // Attente receveur prêt (CTS=0)
    while (bCtsIsEnabled (p) == false)
      ;
  }
  // Valide la transmission
  vTcPrivTxEn (true, p);

  // Attente vidage buffer de transmission
  while ( (TC_UCSRA & _BV (UDRE)) == 0)
    ;
  TC_UCSRA |= _BV (TXC); // Clear du flag transmission terminée
  TC_UDR = c; // transmission
  // Attente transmission terminée
  while ( (TC_UCSRA & _BV (TXC)) == 0)
    ;
  // Invalide la réception
  vTcPrivTxEn (false, p);
  return 0;
}

// -----------------------------------------------------------------------------
void
vTcPrivTxEn (bool bTxEn, xTcPort * p) {
  xTcPollDcb * d = pxTcPollDcb (p);

  if ( (int8_t) bTxEn != d->txen) {

    // Modifie l'état du l'USART uniquement si il est différent
    vTxRxEnable (bTxEn, ( (p->hook->flag & O_HDUPLEX) ? !bTxEn : -1), p);
    d->txen = bTxEn;
    if (p->hook->flag & O_HDUPLEX) {

      d->rxen = !bTxEn;
    }
  }
}

// -----------------------------------------------------------------------------
void
vTcPrivRxEn (bool bRxEn, xTcPort * p) {
  xTcPollDcb * d = pxTcPollDcb (p);

  if ( (int8_t) bRxEn != d->rxen) {

    // Modifie l'état du l'USART uniquement si il est différent
    vTxRxEnable ( ( (p->hook->flag & O_HDUPLEX) ? !bRxEn : -1), bRxEn, p);
    d->rxen = bRxEn;
    if (p->hook->flag & O_HDUPLEX) {

      d->txen = !bRxEn;
    }
  }
}
/* ========================================================================== */
#endif /* AVRIO_TC_FLAVOUR == TC_FLAVOUR_POLL */
