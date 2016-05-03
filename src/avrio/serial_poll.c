/**
 * @file serial_poll.c
 * @brief Liaison série asynchrone version sans interruption (scrutation)
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
#include "serial_private.h"

#if AVRIO_SERIAL_FLAVOUR & SERIAL_FLAVOUR_POLL
/* ========================================================================== */

/* internal public functions ================================================ */

// -----------------------------------------------------------------------------
void
vSerialPrivateInit (uint16_t usBaud, uint16_t usFlags) {

  vTxEnable();
}

// -----------------------------------------------------------------------------
// Retourne le caractère comme un unsigned ou _FDEV_ERR en cas d'erreur ou
// _FDEV_EOF si aucun caractère reçu
int
iSerialPrivateGetChar (void) {
  int c = _FDEV_EOF;
  bool bError = false;

  vRtsEnable();
  do {
    
    // Version non bloquante
    if (usSerialFlags & SERIAL_NOBLOCK) {

      if (UCSRA & _BV (RXC)) {

        c = UDR;
        bError = bSerialIsRxError();
      }
    }
    else {

    // Version bloquante
      while ( (UCSRA & _BV (RXC)) == 0)
        ;
      c = UDR;
      bError = bSerialIsRxError();
    }

  }
  while (bError == true);
  vRtsDisable();
  return (unsigned int) c;
}

// -----------------------------------------------------------------------------
// Retourne 0 en cas de succès
int
iSerialPrivatePutChar (char c) {

  // Attente receveur prêt (CTS=0)
  while (bCtsIsEnabled() == false)
    ;
  // Valide la transmission
  vSerialPrivateTxEn (true);
  // Attente vidage buffer de transmission
  while ( (UCSRA & _BV (UDRE)) == 0)
    ;
  UCSRA |= _BV (TXC); // Clear du flag transmission terminée
  UDR = c; // transmission
  // Attente transmission terminée
  while ( (UCSRA & _BV (TXC)) == 0)
    ;
  // Invalide la réception
  vSerialPrivateTxEn (false);
  return 0;
}

// -----------------------------------------------------------------------------
void
vSerialPutString (const char *pcString) {

  if (usSerialFlags & SERIAL_WR) {
    while (*pcString) {

      (void) iSerialPutChar (*pcString++);
    }
  }
}

// -----------------------------------------------------------------------------
uint16_t
usSerialHit (void) {

  return ( (UCSRA & _BV (RXC)) != 0);
}

// -----------------------------------------------------------------------------
bool
xSerialReady (void) {

  return (UCSRB & _BV (RXEN)) != 0;
}

// -----------------------------------------------------------------------------
void
vSerialFlush (void) {
  
  // Aucun buffer à vider
}

// ------------------------------------------------------------------------------
void
vSerialPrivateTxEn (bool bTxEn) {

  if (bTxEn) {

    vTxEnSet ();
  }
  else {

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
  }
  else {

    // Invalide la réception
    vRxDisable();
    vRxEnClear ();
  }
}

/* ========================================================================== */
#endif /* AVRIO_SERIAL_FLAVOUR == SERIAL_FLAVOUR_POLL */
