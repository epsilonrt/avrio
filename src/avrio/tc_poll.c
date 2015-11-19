/**
 * @file serial_poll.c
 * @brief Liaison série asynchrone version sans interruption (scrutation)
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
#include "tc_private.h"

#ifdef AVRIO_TC_ENABLE
/* ========================================================================== */

/* internal public functions ================================================ */

// -----------------------------------------------------------------------------
uint16_t
usTcHit (xFileHook * h) {
  xTcPort * p = h->dev;

  return ( (TC_UCSRA & _BV (RXC)) != 0);
}

// -----------------------------------------------------------------------------
bool
xTcReady (xFileHook * h) {
  xTcPort * p = h->dev;

  return (TC_UCSRB & _BV (RXEN)) != 0;
}

// -----------------------------------------------------------------------------
void
vTcFlush (xFileHook * h) {
  
  // Aucun buffer à vider
}

/* internal private functions =============================================== */

// -----------------------------------------------------------------------------
void
vTcPrivateInit (xFileHook * h) {

  vTxEnable(h->dev);
}

// -----------------------------------------------------------------------------
// Retourne le caractère comme un unsigned ou _FDEV_ERR en cas d'erreur ou
// _FDEV_EOF si aucun caractère reçu
int
iTcPrivateGetChar (xFileHook * h) {
  int c = _FDEV_EOF;
  bool bError = false;

  xTcPort * p = h->dev;

  vRtsEnable(h);
  do {
    
    // Version non bloquante
    if (h->flag & O_NONBLOCK) {

      if (TC_UCSRA & _BV (RXC)) {

        c = TC_UDR;
        bError = bTcIsRxError(p);
      }
    }
    else {

    // Version bloquante
      while ( (TC_UCSRA & _BV (RXC)) == 0)
        ;
      c = TC_UDR;
      bError = bTcIsRxError(p);
    }
  }
  while (bError == true);
  vRtsDisable(h);
  return (unsigned int) c;
}

// -----------------------------------------------------------------------------
// Retourne 0 en cas de succès
int
iTcPrivatePutChar (char c, xFileHook * h) {
  xTcPort * p = h->dev;
  
  // Attente receveur prêt (CTS=0)
  while (bCtsIsEnabled(h) == false)
    ;
  // Valide la transmission
  vTcPrivateTxEn (true, h);
  // Attente vidage buffer de transmission
  while ( (TC_UCSRA & _BV (UDRE)) == 0)
    ;
  TC_UCSRA |= _BV (TXC); // Clear du flag transmission terminée
  TC_UDR = c; // transmission
  // Attente transmission terminée
  while ( (TC_UCSRA & _BV (TXC)) == 0)
    ;
  // Invalide la réception
  vTcPrivateTxEn (false, h);
  return 0;
}

// ------------------------------------------------------------------------------
void
vTcPrivateTxEn (bool bTxEn, xFileHook * h) {
  xTcPort * p = h->dev;

  if (bTxEn) {

    vTxEnSet (p);
  }
  else {

    vTxEnClear (p);
  }
}

// -----------------------------------------------------------------------------
void
vTcPrivateRxEn (bool bRxEn, xFileHook * h) {
  xTcPort * p = h->dev;

  if (bRxEn) {

    // Valide la réception
    vRxEnSet(p);
    vRxEnable(p);
    vRxClearError(p);
  }
  else {

    // Invalide la réception
    vRxDisable(p);
    vRxEnClear (p);
  }
}

/* ========================================================================== */
#endif /* AVRIO_TC_FLAVOUR == TC_FLAVOUR_POLL */
