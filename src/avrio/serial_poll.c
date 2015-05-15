/**
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
 *
 * @file serial.c
 * @ingroup serial_module
 * @brief Liaison série asynchrone version sans interruption
 *
 * Dépend des modules:
 * - \ref queue_module (version avec interruption seulement)
 * .
 *
   ========================================================================== */
#include "avrio-config.h"

#ifdef AVRIO_SERIAL_ENABLE
/* ========================================================================== */
#include "serial_private.h"

/* internal public functions ================================================ */

// -----------------------------------------------------------------------------
void
vSerialPrivateInit (uint16_t usBaud, uint16_t usFlags) {

  vSerialEnable (usFlags);
}

// -----------------------------------------------------------------------------
int
iSerialGetChar (void) {
  int iChar = _FDEV_EOF;

  if (usSerialFlags & SERIAL_RD) {
    bool bError = false;

    vRxEnable();
    vRtsEnable();
    do {

      if (usSerialFlags & SERIAL_NOBLOCK) {

        if (UCSRA & _BV (RXC)) {

          iChar = UDR;
          bError = bCheckError();
        }
      }
      else {

        while ( (UCSRA & _BV (RXC)) == 0)
          ;
        iChar = UDR;
        bError = bCheckError();
      }

    } while (bError == true);
    vRtsDisable();

    if ((usSerialFlags & SERIAL_ECHO) && (iChar != _FDEV_EOF)) {

      (void)iSerialPutChar ( (char) iChar);
    }
  }
  return (unsigned int) iChar;
}

// -----------------------------------------------------------------------------
uint16_t
usSerialHit (void) {

  return ( (UCSRA & _BV (RXC)) != 0);
}

// -----------------------------------------------------------------------------
int
iSerialPutChar (char c) {

  if (usSerialFlags & SERIAL_WR) {

#if SERIAL_EOL == SERIAL_CRLF
    if (c == '\n') {
      (void) iSerialPutChar ('\r');
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

    while (bCtsIsEnabled() == false)
      ; // Attente CTS = 0
    vTxEnable();
    while ( (UCSRA & _BV (UDRE)) == 0)
      ;
    UCSRA |= _BV (TXC);
    UDR = c;
    while ( (UCSRA & _BV (TXC)) == 0)
      ;
    vTxDisable();
  }
  else {

    return -1;
  }
  return 0;
}

// -----------------------------------------------------------------------------
void
vSerialPutString (const char *pcString) {

  if (usSerialFlags & SERIAL_WR) {
    while (*pcString) {

      (void)iSerialPutChar (*pcString++);
    }
  }
}

#endif /* AVRIO_SERIAL_ENABLE defined */
/* ========================================================================== */
