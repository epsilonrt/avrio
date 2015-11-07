/**
 * @file serial.c
 * @brief Liaison série asynchrone version
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

#ifdef AVRIO_SERIAL_ENABLE
/* ========================================================================== */
#include "serial_private.h"

/* public variables ======================================================== */
uint16_t usSerialFlags;
int iSerialError;

/* internal public functions ================================================ */

// -----------------------------------------------------------------------------
uint16_t
usIosToFlags (const xSerialIos * ios) {
  uint16_t flags = 0;

  switch (ios->dbits) {

    case SERIAL_DATABIT_6:
      flags |= SERIAL_6BIT;
      break;

    case SERIAL_DATABIT_7:
      flags |= SERIAL_7BIT;
      break;

    case SERIAL_DATABIT_8:
      flags |= SERIAL_8BIT;
      break;

    default:
      break;
  }

  switch (ios->parity) {

    case SERIAL_PARITY_EVEN:
      flags |= SERIAL_EVEN;
      break;
      
    case SERIAL_PARITY_ODD:
      flags |= SERIAL_ODD;
      break;
      
    default:
      break;
  }

  if (ios->sbits == SERIAL_STOPBIT_TWO) {

    flags |= SERIAL_2STP;
  }

  switch (ios->flow) {

    case SERIAL_FLOW_RTSCTS:
      flags |= SERIAL_RTSCTS;
      break;

    case SERIAL_FLOW_XONXOFF:
      flags |= SERIAL_XONXOFF;
      break;

    default:
      break;
  }
  return flags;
}

// -----------------------------------------------------------------------------
void
vSerialInit (uint16_t usBaud, uint16_t usFlags) {
  uint16_t usUBRR;

  UCSRB = 0;
  UCSRA = 0;
  UCSRC = 0;
  usSerialFlags = 0;

#if defined(AVRIO_SERIAL_BAUD_USE_X2)
  // Utilisation exclusive de X2
  usUBRR = SERIAL_BAUD_X2 (usBaud);
  UCSRA |= _BV (U2X);
#else
  usUBRR = SERIAL_BAUD_X1 (usBaud);
  if (usUBRR == 0) {
    // Vitesse trop grande, on passe en X2
    usUBRR = SERIAL_BAUD_X2 (usBaud);
    UCSRA |= _BV (U2X);
  }
#endif

  UBRRL = usUBRR & 0xFF;
  UBRRH = usUBRR >> 8;
  vSerialSetFlags (usFlags);

#if defined(USART_TXPIN)
  // Permet de mettre en sortie à 0 la broche TXD si TXEN=0
  // USART_PORT &= ~_BV (USART_TXPIN);
  // Permet de mettre en sortie à 1 la broche TXD si TXEN=0
  USART_PORT |= _BV (USART_TXPIN);
  USART_DDR  |= _BV (USART_TXPIN);
#endif
#if defined(USART_RXPIN)
  // Permet de mettre en entrée avec pull-up la broche RXD si RXEN=0
  USART_PORT |= _BV (USART_RXPIN);
  USART_DDR  &= ~_BV (USART_RXPIN);
#endif

  vSerialPrivateInit (usBaud, usFlags);
  vTxEnInit ();
  vRxEnInit ();
  vRtsInit();
  vCtsInit();
  vSerialPrivateRxEn ( (usFlags & SERIAL_RD) != 0);
}

// -----------------------------------------------------------------------------
int
iSerialPutChar (char c) {

  if (usSerialFlags & SERIAL_WR) {

#if SERIAL_EOL == SERIAL_CRLF
    if (c == '\n') {
      if (iSerialPrivatePutChar ('\r') != 0) {

        return _FDEV_EOF;
      }
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

    return iSerialPrivatePutChar (c);
  }

  return _FDEV_EOF;
}

// -----------------------------------------------------------------------------
int
iSerialGetChar (void) {
  int c = _FDEV_EOF;

  if (usSerialFlags & SERIAL_RD) {

    c = iSerialPrivateGetChar();
    if ( (usSerialFlags & SERIAL_ECHO) && (c != _FDEV_EOF)) {

      (void) iSerialPutChar (c);
    }
  }
  return (unsigned int) c;
}

// -----------------------------------------------------------------------------
void
vSerialSetFlags (uint16_t usFlags) {

  /*
   * Configure: parité, stop bits, data bits
   */
#ifdef UCSZ2
  if ( (usFlags & SERIAL_9BIT) == SERIAL_9BIT) {

    UCSRB |= _BV (UCSZ2);
  }
  else {

    UCSRB &= ~_BV (UCSZ2);
  }
#endif
  UCSRC = UCSRC_SEL | (usFlags & ~ (SERIAL_ECHO | SERIAL_RW));

  usSerialFlags = usFlags;
}

// -----------------------------------------------------------------------------
uint16_t
usSerialGetFlags (void) {

  return usSerialFlags;
}

// -----------------------------------------------------------------------------
void
vSerialEnable (uint16_t usFlags) {

  if ( (usFlags & SERIAL_RW) != (usSerialFlags & SERIAL_RW)) {

    if (usFlags & SERIAL_RD) {

      UCSRB |= _BV (RXEN);
    }
    else {

      UCSRB &= ~_BV (RXEN);
    }
    if (usFlags & SERIAL_WR) {

      UCSRB |= _BV (TXEN);
    }
    else {

      UCSRB &= ~_BV (TXEN);
    }
    usSerialFlags = (usFlags & SERIAL_RW) | (usSerialFlags & ~SERIAL_RW);
  }
}

// -----------------------------------------------------------------------------
bool
bSerialIsRxError (void) {

  uint8_t s = UCSRA & (_BV (PE) | _BV (FE));
  if (s & _BV (PE)) {

    iSerialError |= eSerialRxParityError;
  }
  if (s & _BV (FE)) {

    iSerialError |= eSerialRxFormatError;
  }
  return s != 0;
}

#include "serial_irq.c"
#include "serial_rs485.c"
#include "serial_poll.c"

/* avr-libc stdio interface ================================================= */
static int iPutChar (char c, FILE * pxStream);
static int iGetChar (FILE * pxStream);

FILE xSerialPort = FDEV_SETUP_STREAM (iPutChar, iGetChar, _FDEV_SETUP_RW);

// -----------------------------------------------------------------------------
static int
iPutChar (char c, FILE * pxStream) {

  return iSerialPutChar (c);
}

// -----------------------------------------------------------------------------
static int
iGetChar (FILE * pxStream) {

  clearerr (pxStream);
  return iSerialGetChar();
}

#endif /* AVRIO_SERIAL_ENABLE defined */
/* ========================================================================== */
