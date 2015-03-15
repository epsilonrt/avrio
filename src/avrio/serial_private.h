/**
 * @file serial_private.h
 * @brief Entête privé liaison série asynchrone
 * @author Copyright © 2011-2012 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * @version $Id$
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

/* constants ================================================================ */
# define CONFIG_EOL_CR
//# define CONFIG_EOL_LF
//# define CONFIG_EOL_CRLF

/* public variables ======================================================== */
extern uint16_t usSerialFlags;
extern xMutex xSerialMutex;

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

#ifdef SERIAL_TXEN_ENABLE
// -----------------------------------------------------------------------------
static inline void
vTxEnInit (void) {

  SERIAL_TXEN_PORT &= _BV(SERIAL_TXEN_BIT);
  SERIAL_TXEN_DDR |= _BV(SERIAL_TXEN_BIT);
}
// -----------------------------------------------------------------------------
static inline void
vTxEnSet (void) {

  SERIAL_TXEN_PORT |= _BV(SERIAL_TXEN_BIT);
}
// -----------------------------------------------------------------------------
static inline void
vTxEnClear (void) {

  SERIAL_TXEN_PORT &= _BV(SERIAL_TXEN_BIT);
}
#else /* SERIAL_TXEN_ENABLE not defined */
// -----------------------------------------------------------------------------
#define vTxEnInit()
#define vTxEnSet()
#define vTxEnClear()
// -----------------------------------------------------------------------------
#endif /* SERIAL_TXEN_ENABLE */

#ifdef AVRIO_SERIAL_RTSCTS
// -----------------------------------------------------------------------------
static inline void
vRtsEnable (void) {

  if ((usSerialFlags & SERIAL_RTSCTS) && (usSerialFlags & SERIAL_RD)) {

    SERIAL_RTS_PORT &= ~_BV (SERIAL_RTS_BIT);
  }
}

// -----------------------------------------------------------------------------
static inline void
vRtsDisable (void) {

  if ((usSerialFlags & SERIAL_RTSCTS) && (usSerialFlags & SERIAL_RD)) {

    SERIAL_RTS_PORT |= _BV (SERIAL_RTS_BIT);
  }
}

// -----------------------------------------------------------------------------
static inline bool
bCtsIsEnabled (void) {

  if ((usSerialFlags & SERIAL_RTSCTS) && (usSerialFlags & SERIAL_WR)) {

    return (SERIAL_CTS_PIN & _BV (SERIAL_CTS_BIT)) == 0;
  }
  return true;
}

// -----------------------------------------------------------------------------
static inline void
vRtsInit (void) {

  if ((usSerialFlags & SERIAL_RTSCTS) && (usSerialFlags & SERIAL_RD)) {

    SERIAL_RTS_DDR |= _BV (SERIAL_RTS_BIT); // RTS output
  }
}

// -----------------------------------------------------------------------------
static inline void
vCtsInit (void) {

  if ((usSerialFlags & SERIAL_RTSCTS) && (usSerialFlags & SERIAL_WR)) {

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

#endif
/* AVRIO_SERIAL_ENABLEdefined */
/* ========================================================================== */
#endif /* _AVRIO_SERIAL_PRIVATE_H_ */
