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
 * @file serial_private.h
 * @brief Entête privé liaison série asynchrone
 */
/* Copyright © 2009-2012 epsilonRT. All rights reserved.
 *  $Id$ */
#ifndef _AVRIO_SERIAL_PRIVATE_H_
#define _AVRIO_SERIAL_PRIVATE_H_
/* ========================================================================== */
#include "avrio-config.h"

#ifdef AVRIO_SERIAL_ENABLE
/* ========================================================================== */
#include "avrio-board-serial.h"
#include <avrio/mutex.h>
#include <avrio/serial.h>
#include <avrio/queue.h>

/* constants ================================================================ */
# define CONFIG_EOL_CR
//# define CONFIG_EOL_LF
//# define CONFIG_EOL_CRLF

/* public variables ======================================================== */
extern uint16_t usSerialFlags;
extern xMutex xSerialMutex;
extern xQueue xSerialTxQueue;
extern xQueue xSerialRxQueue;

/* macros =================================================================== */
#ifdef SERIAL_HALF_DUPLEX
#define SERIAL_MUTEX_LOCK() vMutexLock(&xSerialMutex)
#define SERIAL_MUTEX_UNLOCK() vMutexUnlock(&xSerialMutex)
#else
#define SERIAL_MUTEX_LOCK()
#define SERIAL_MUTEX_UNLOCK()
#endif

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

/* private functions ======================================================== */

#ifdef SERIAL_TXEN_ENABLE
/* -----------------------------------------------------------------------------
 *
 *                   Broche de validation Transmission
 *
 * ---------------------------------------------------------------------------*/
// -----------------------------------------------------------------------------
INLINE void
vTxEnInit (void) {

  SERIAL_TXEN_PORT &= _BV (SERIAL_TXEN_BIT);
  SERIAL_TXEN_DDR |= _BV (SERIAL_TXEN_BIT);
}

// -----------------------------------------------------------------------------
// Active à l'état haut
INLINE void
vTxEnSet (void) {

  SERIAL_TXEN_PORT |= _BV (SERIAL_TXEN_BIT);
}

// -----------------------------------------------------------------------------
// Inactive à l'état bas
INLINE void
vTxEnClear (void) {

  SERIAL_TXEN_PORT &= _BV (SERIAL_TXEN_BIT);
}
#else /* SERIAL_TXEN_ENABLE not defined */
// -----------------------------------------------------------------------------
#define vTxEnInit()
#define vTxEnSet()
#define vTxEnClear()
// -----------------------------------------------------------------------------
#endif /* SERIAL_TXEN_ENABLE */

#ifdef SERIAL_RXEN_ENABLE
/* -----------------------------------------------------------------------------
 *
 *                   Broche de validation Réception
 *
 * ---------------------------------------------------------------------------*/
// -----------------------------------------------------------------------------
INLINE void
vRxEnInit (void) {

  SERIAL_RXEN_PORT &= _BV (SERIAL_RXEN_BIT);
  SERIAL_RXEN_DDR |= _BV (SERIAL_RXEN_BIT);
}

// -----------------------------------------------------------------------------
// Active à l'état bas
INLINE void
vRxEnSet (void) {

  SERIAL_RXEN_PORT &= _BV (SERIAL_RXEN_BIT);
}

// -----------------------------------------------------------------------------
// Inactive à l'état haut
INLINE void
vRxEnClear (void) {

  SERIAL_RXEN_PORT |= _BV (SERIAL_RXEN_BIT);
}
#else /* SERIAL_RXEN_ENABLE not defined */
// -----------------------------------------------------------------------------
#define vRxEnInit()
#define vRxEnClear()
#define vRxEnSet()
// -----------------------------------------------------------------------------
#endif /* SERIAL_RXEN_ENABLE */

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

/* -----------------------------------------------------------------------------
 *
 *                              Partie commune
 *
 * ---------------------------------------------------------------------------*/
static inline void vTxEnable (void);
static inline void vRxEnable (void);
static inline void vTxDisable (void);
static inline void vRxDisable (void);

// -----------------------------------------------------------------------------
INLINE void
vRxInit (void) {

#ifdef AVRIO_SERIAL_RXIE
  vQueueFlush (&xSerialRxQueue);
#endif
}

// -----------------------------------------------------------------------------
INLINE void
vRxIrqEnable (void) {
#ifdef AVRIO_SERIAL_RXIE
  UCSRB |= _BV (RXCIE);
#endif
}

// -----------------------------------------------------------------------------
INLINE void
vRxIrqDisable (void) {
#ifdef AVRIO_SERIAL_RXIE
  UCSRB &= ~_BV (RXCIE);
#endif
}

// -----------------------------------------------------------------------------
INLINE void
vRxEnable (void) {

  if (usSerialFlags & SERIAL_RD) {

#ifdef SERIAL_HALF_DUPLEX
    // Attente fin de transmission
    SERIAL_MUTEX_LOCK();
    ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {

      vTxDisable();
      SERIAL_MUTEX_LOCK();
    }
#endif

    UCSRB |= _BV (RXEN);
    if ( (UCSRA & (_BV (PE) | _BV (FE))) != 0) {

      (void) UDR; /* clear des flags d'erreur */
    }
    vRxIrqEnable();
    vRtsEnable();
  }
}

// -----------------------------------------------------------------------------
INLINE void
vRxDisable (void) {

  vRtsDisable();
  vRxIrqDisable();
  UCSRB &= ~_BV (RXEN);
  SERIAL_MUTEX_UNLOCK();
}

// -----------------------------------------------------------------------------
INLINE void
vTxInit (void) {

#ifdef AVRIO_SERIAL_TXIE
  vQueueFlush (&xSerialTxQueue);
#endif
}

// -----------------------------------------------------------------------------
INLINE void
vTxIrqEnable (void) {
#ifdef AVRIO_SERIAL_TXIE
  UCSRB &= ~_BV (TXCIE);
  UCSRB |= _BV (UDRIE);
#endif
}

// -----------------------------------------------------------------------------
INLINE void
vTxIrqDisable (void) {

#ifdef AVRIO_SERIAL_TXIE
  UCSRB &= ~ (_BV (TXCIE) | _BV (UDRIE));
#endif
}

// -----------------------------------------------------------------------------
INLINE void
vTxEnable (void) {

  if (usSerialFlags & SERIAL_WR) {

#ifdef SERIAL_HALF_DUPLEX
    vRxDisable();
#endif
    SERIAL_MUTEX_LOCK();
    vTxEnSet ();
    UCSRB |= _BV (TXEN);
    vTxIrqEnable();
  }
}

// -----------------------------------------------------------------------------
INLINE void
vTxDisable (void) {

  vTxIrqDisable();
  vTxEnClear ();
  UCSRB &= ~_BV (TXEN);
  SERIAL_MUTEX_UNLOCK();
}

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

#endif
/* AVRIO_SERIAL_ENABLE defined */
/* ========================================================================== */
#endif /* _AVRIO_SERIAL_PRIVATE_H_ */
