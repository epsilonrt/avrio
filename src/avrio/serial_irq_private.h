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
#ifndef _AVRIO_SERIAL_IRQ_PRIVATE_H_
#define _AVRIO_SERIAL_IRQ_PRIVATE_H_
/* ========================================================================== */
#include "avrio-config.h"

#ifdef AVRIO_SERIAL_ENABLE
/* ========================================================================== */
#include "avrio-board-serial.h"
#include <avrio/serial_private.h>
#include <avrio/queue.h>

/* constants ================================================================ */
/* macros =================================================================== */

/* public variables ======================================================== */
extern xQueue xSerialTxQueue;
extern xQueue xSerialRxQueue;

/* private functions ======================================================== */

// -----------------------------------------------------------------------------
INLINE void
vRxInit (void) {

  vQueueFlush (&xSerialRxQueue);
}

// -----------------------------------------------------------------------------
INLINE void
vRxIrqEnable (void) {

  UCSRB |= _BV (RXCIE);
}

// -----------------------------------------------------------------------------
INLINE void
vRxIrqDisable (void) {

  UCSRB &= ~_BV (RXCIE);
}

// -----------------------------------------------------------------------------
INLINE void
vRxEnable (void) {

  if (usSerialFlags & SERIAL_RD) {

    // Attente fin de transmission
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
}

// -----------------------------------------------------------------------------
INLINE void
vTxInit (void) {

  vQueueFlush (&xSerialTxQueue);
}

// -----------------------------------------------------------------------------
INLINE void
vTxIrqEnable (void) {

  UCSRB &= ~_BV (TXCIE);
  UCSRB |= _BV (UDRIE);
}

// -----------------------------------------------------------------------------
INLINE void
vTxIrqDisable (void) {

  UCSRB &= ~ (_BV (TXCIE) | _BV (UDRIE));
}

// -----------------------------------------------------------------------------
INLINE void
vTxEnable (void) {

  if (usSerialFlags & SERIAL_WR) {

    UCSRB |= _BV (TXEN);
    vTxIrqEnable();
  }
}

// -----------------------------------------------------------------------------
INLINE void
vTxDisable (void) {

  vTxIrqDisable();
  UCSRB &= ~_BV (TXEN);
}

#endif /* AVRIO_SERIAL_ENABLE defined */
/* ========================================================================== */
#endif /* _AVRIO_SERIAL_IRQ_PRIVATE_H_ */
