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

#if defined(AVRIO_TC_ENABLE) && (AVRIO_TC_FLAVOUR == TC_FLAVOUR_POLL)
/* ========================================================================== */

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

  vUartEnableTx(p);
  vRtsDisable(p);
}

// -----------------------------------------------------------------------------
// Retourne le caractère comme un unsigned ou _FDEV_ERR en cas d'erreur ou
// _FDEV_EOF si aucun caractère reçu
int
iTcPrivGetChar (xTcPort * p) {
  int c = _FDEV_EOF;
  int iError = 0;

  vRtsEnable(p);
  do {
    
    // Version non bloquante
    if (p->hook->flag & O_NONBLOCK) {

      if (TC_UCSRA & _BV (RXC)) {

        c = TC_UDR;
        iError = iTcPrivRxError(p);
      }
    }
    else {

    // Version bloquante
      while ( (TC_UCSRA & _BV (RXC)) == 0)
        ;
      c = TC_UDR;
      iError = iTcPrivRxError(p);
    }
  }
  while (iError != 0);
  vRtsDisable(p);
  return (unsigned int) c;
}

// -----------------------------------------------------------------------------
// Retourne 0 en cas de succès
int
iTcPrivPutChar (char c, xTcPort * p) {
  
  // Attente receveur prêt (CTS=0)
  while (bCtsIsEnabled(p) == false)
    ;
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

// ------------------------------------------------------------------------------
void
vTcPrivTxEn (bool bTxEn, xTcPort * p) {

  if (bTxEn) {

    vTxenSet (p);
  }
  else {

    vTxenClear (p);
  }
}

// -----------------------------------------------------------------------------
void
vTcPrivRxEn (bool bRxEn, xTcPort * p) {

  if (bRxEn) {

    // Valide la réception
    vRxenSet(p);
    vUartEnableRx(p);
    vUartClearRxError(p);
  }
  else {

    // Invalide la réception
    vUartDisableRx(p);
    vRxenClear (p);
  }
}

/* ========================================================================== */
#endif /* AVRIO_TC_FLAVOUR == TC_FLAVOUR_POLL */
