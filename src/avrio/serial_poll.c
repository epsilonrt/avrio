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
