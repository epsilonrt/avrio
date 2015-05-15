/**
 * @file serial_rs485_private.h
 * @brief Entête privé liaison série rs485
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
#ifndef _AVRIO_SERIAL_RS485_PRIVATE_H_
#define _AVRIO_SERIAL_RS485_PRIVATE_H_
/* ========================================================================== */
#include "avrio-config.h"

#ifdef AVRIO_SERIAL_ENABLE
/* ========================================================================== */
#include "avrio-board-serial.h"
#include <avrio/serial_private.h>
#include <avrio/mutex.h>
#include <avrio/queue.h>
#include <util/atomic.h>

#ifndef SERIAL_TXEN_ENABLE
#error SERIAL_TXEN_ENABLE must be defined for RS485 serial link !
#endif

/* constants ================================================================ */
/* macros =================================================================== */
/* public variables ======================================================== */
/* private functions ======================================================== */

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

  SERIAL_TXEN_PORT &= ~_BV (SERIAL_TXEN_BIT);
}

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

  SERIAL_RXEN_PORT &= ~_BV (SERIAL_RXEN_BIT);
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

#endif /* AVRIO_SERIAL_ENABLE defined */
/* ========================================================================== */
#endif /* _AVRIO_SERIAL_RS485_PRIVATE_H_ */
