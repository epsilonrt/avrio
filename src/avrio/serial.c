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

/* public variables ======================================================== */
uint16_t usSerialFlags;

#ifdef SERIAL_HALF_DUPLEX
/*
 * Mutex pour une transmission en half-duplex
 * La transmission et la réception est impossible simultanément
 * Mutex non vérouillé au départ
 */
xMutex xSerialMutex = MUTEX_INITIALIZER;
#endif


/* -----------------------------------------------------------------------------
 *
 *                              Partie commune
 *
 * ---------------------------------------------------------------------------*/

/* internal public functions ================================================ */

// -----------------------------------------------------------------------------
void
vSerialInit (uint16_t usBaud, uint16_t usFlags) {
  uint16_t usUBRR;

  UCSRB = 0;
  usSerialFlags = 0;

  usUBRR = SERIAL_BAUD_X1 (usBaud);
  if (usUBRR == 0) {

    usUBRR = SERIAL_BAUD_X2 (usBaud);
    UCSRA |= _BV (U2X);
  }
  UBRRL = usUBRR & 0xFF;
  UBRRH = usUBRR >> 8;

  vTxEnInit ();
  vSerialSetFlags (usFlags);
  vRtsInit();
  vCtsInit();
  vTxInit();
  vRxInit();
  vRxEnable();
}

// -----------------------------------------------------------------------------
void
vSerialSetFlags (uint16_t usFlags) {

  UCSRC = UCSRC_SEL | (usFlags & ~ (SERIAL_ECHO | SERIAL_RW));
  vSerialEnable (usFlags);
  usSerialFlags = usFlags;
}

// -----------------------------------------------------------------------------
void
vSerialEnable (uint16_t usFlags) {

  if ( (usFlags & SERIAL_RW) != (usSerialFlags & SERIAL_RW)) {

    if (usFlags & SERIAL_RD) {

      vRxEnable();
    }
    else {

      vRxDisable();
    }
    if (usFlags & SERIAL_WR) {

      UCSRB |= _BV (TXEN);
    }
    else {

      UCSRB &= ~_BV (TXEN);
    }
    usSerialFlags = (usFlags & SERIAL_RW) | (usSerialFlags & ~SERIAL_RW);
    SERIAL_MUTEX_UNLOCK();
  }
}

// -----------------------------------------------------------------------------
uint16_t
usSerialGetFlags (void) {

  return usSerialFlags;
}

// -----------------------------------------------------------------------------
bool
xSerialReady (void) {

#ifdef SERIAL_HALF_DUPLEX
  return xSerialMutex == MUTEX_UNLOCK;
#else
  return true;
#endif
}

#ifndef AVRIO_SERIAL_RXIE
/* -----------------------------------------------------------------------------
 *
 *                         Réception sans interruption
 *
 * ---------------------------------------------------------------------------*/

/* private functions ======================================================== */

/* internal public functions ================================================ */
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
#endif /* AVRIO_SERIAL_RXIE not defined */





#ifndef AVRIO_SERIAL_TXIE
/* -----------------------------------------------------------------------------
 *
 *                         Transmission sans interruption
 *
 * ---------------------------------------------------------------------------*/

/* internal public functions ================================================ */

// -----------------------------------------------------------------------------
int
iSerialPutChar (char c) {

  if (usSerialFlags & SERIAL_WR) {

#if defined(CONFIG_EOL_CRLF)
    if (c == '\n') {
      (void)iSerialPutChar ('\r');
    }
#elif defined(CONFIG_EOL_CR)
    if (c == '\n') {
      c = '\r';
    }
#elif defined(CONFIG_EOL_LF)
    if (c == '\r') {
      c = '\n';
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
// -----------------------------------------------------------------------------
#endif /* AVRIO_SERIAL_TXIE not defined */

/* avr-libc stdio interface ================================================= */
static int PutChar (char c, FILE * pxStream);
static int GetChar (FILE * pxStream);

FILE xSerialPort = FDEV_SETUP_STREAM (PutChar, GetChar, _FDEV_SETUP_RW);

// -----------------------------------------------------------------------------
static int
PutChar (char c, FILE * pxStream) {

  return iSerialPutChar (c);
}

// -----------------------------------------------------------------------------
static int
GetChar (FILE * pxStream) {

  clearerr (&xSerialPort);
  return iSerialGetChar ();
}


#endif /* AVRIO_SERIAL_ENABLE defined */
/* ========================================================================== */
