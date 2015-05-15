/**
 * @file serial_private.h
 * @brief Entête privé liaison série asynchrone
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
#ifndef _AVRIO_SERIAL_PRIVATE_H_
#define _AVRIO_SERIAL_PRIVATE_H_
/* ========================================================================== */
#include "avrio-config.h"

#ifdef AVRIO_SERIAL_ENABLE
/* ========================================================================== */
#include "serial.h"

#define  SERIAL_CR    0x0D
#define  SERIAL_LF    0x0A
#define  SERIAL_CRLF  (SERIAL_CR + SERIAL_LF)
#include "avrio-board-serial.h"

/* constants ================================================================ */
/* public variables ======================================================== */
extern uint16_t usSerialFlags;

/* macros =================================================================== */
#define SERIAL_BAUD_X1(usBaud) (AVRIO_CPU_FREQ / (1600UL * usBaud) - 1)
#define SERIAL_BAUD_X2(usBaud) (AVRIO_CPU_FREQ / (800UL * usBaud) - 1)

/* USART Control and Status Register A */
#ifndef RXC
#define    RXC          7
#define    TXC          6
#define    UDRE         5
#define    FE           4
#define    DOR          3
#define    PE           2
#define    U2X          1

#    elif !defined(PE)
#define    PE           2
#endif

/* USART Control and Status Register B */
#ifndef RXCIE
#define    RXCIE        7
#define    TXCIE        6
#define    UDRIE        5
#define    RXEN         4
#define    TXEN         3
#define    UCSZ2        2
#define    RXB8         1
#define    TXB8         0
#endif

/* USART Control and Status Register C */
#ifndef UPM1
#define    UPM1         5
#define    UPM0         4
#define    USBS         3
#define    UCSZ1        2
#define    UCSZ0        1
#endif

#ifdef URSEL
#define UCSRC_SEL _BV(URSEL)
#else
#define UCSRC_SEL 0
#endif

/* internal public functions ================================================ */
void vSerialPrivateInit (uint16_t usBaud, uint16_t usFlags);
int iSerialPrivatePutChar (char c);

/* private functions ======================================================== */

// -----------------------------------------------------------------------------
INLINE bool
bCheckError (void) {

  if ( (UCSRA & (_BV (PE) | _BV (FE))) == 0) {

    return false;
  }
  else {

    return true;
  }
}

#ifdef AVRIO_SERIAL_RTSCTS
/* -----------------------------------------------------------------------------
 *
 *                      Contrôle de flux RTS/CTS
 *
 * ---------------------------------------------------------------------------*/
// -----------------------------------------------------------------------------
INLINE void
vRtsEnable (void) {

  if ( (usSerialFlags & SERIAL_RTSCTS) && (usSerialFlags & SERIAL_RD)) {

    SERIAL_RTS_PORT &= ~_BV (SERIAL_RTS_BIT);
  }
}

// -----------------------------------------------------------------------------
INLINE void
vRtsDisable (void) {

  if ( (usSerialFlags & SERIAL_RTSCTS) && (usSerialFlags & SERIAL_RD)) {

    SERIAL_RTS_PORT |= _BV (SERIAL_RTS_BIT);
  }
}

// -----------------------------------------------------------------------------
INLINE bool
bCtsIsEnabled (void) {

  if ( (usSerialFlags & SERIAL_RTSCTS) && (usSerialFlags & SERIAL_WR)) {

    return (SERIAL_CTS_PIN & _BV (SERIAL_CTS_BIT)) == 0;
  }
  return true;
}

// -----------------------------------------------------------------------------
INLINE void
vRtsInit (void) {

  if ( (usSerialFlags & SERIAL_RTSCTS) && (usSerialFlags & SERIAL_RD)) {

    SERIAL_RTS_DDR |= _BV (SERIAL_RTS_BIT); // RTS output
  }
}

// -----------------------------------------------------------------------------
INLINE void
vCtsInit (void) {

  if ( (usSerialFlags & SERIAL_RTSCTS) && (usSerialFlags & SERIAL_WR)) {

    SERIAL_CTS_DDR &= ~_BV (SERIAL_CTS_BIT); // Set flow control pins CTS input
  }
}
#else /* AVRIO_SERIAL_RTSCTS not defined */
// -----------------------------------------------------------------------------
#define vRtsInit()
#define vRtsEnable()
#define vRtsDisable()
#define vCtsInit()
#define bCtsIsEnabled() (true)
// -----------------------------------------------------------------------------
#endif /* AVRIO_SERIAL_RTSCTS */

#endif  /* AVRIO_SERIAL_ENABLE defined */
/* ========================================================================== */
#endif /* _AVRIO_SERIAL_PRIVATE_H_ */
