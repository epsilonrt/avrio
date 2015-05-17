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
#include <avrio/serial_private.h>

#if (AVRIO_SERIAL_FLAVOUR & SERIAL_FLAVOUR_IRQ) != 0
/* ========================================================================== */

/* private functions ======================================================== */
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
vTxUdreIrqEnable (void) {

  UCSRB &= ~_BV (TXCIE);
  UCSRB |= _BV (UDRIE);
}

// -----------------------------------------------------------------------------
INLINE void
vTxTxcIrqEnable (void) {

  UCSRB &= ~_BV (UDRIE);
  UCSRB |= _BV (TXCIE);
}

// -----------------------------------------------------------------------------
INLINE void
vTxIrqDisable (void) {

  UCSRB &= ~ (_BV (TXCIE) | _BV (UDRIE));
}

#else /* AVRIO_SERIAL_FLAVOUR & SERIAL_FLAVOUR_IRQ == 0 */
// -----------------------------------------------------------------------------
#define vRxIrqEnable()
#define vRxIrqDisable()
#define vTxUdreIrqEnable()
#define vTxIrqDisable()
// -----------------------------------------------------------------------------
#endif /* AVRIO_SERIAL_FLAVOUR & SERIAL_FLAVOUR_IRQ != 0 */

/* ========================================================================== */
#endif /* AVRIO_SERIAL_ENABLE defined */
#endif /* _AVRIO_SERIAL_IRQ_PRIVATE_H_ */
