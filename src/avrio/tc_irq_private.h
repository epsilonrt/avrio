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
#ifndef _AVRIO_TC_IRQ_PRIVATE_H_
#define _AVRIO_TC_IRQ_PRIVATE_H_
/* ========================================================================== */
#include "avrio-config.h"

#ifdef AVRIO_TC_ENABLE00
/* ========================================================================== */
#include <avrio/tc_private.h>

#if (AVRIO_TC_FLAVOUR & TC_FLAVOUR_IRQ) != 0
/* ========================================================================== */
extern int8_t iTcTxEn;
extern int8_t iTcRxEn;

/* private functions ======================================================== */
// -----------------------------------------------------------------------------
INLINE void
vRxIrqEnable (void) {

  TC_UCSRB |= _BV (RXCIE);
}

// -----------------------------------------------------------------------------
INLINE void
vRxIrqDisable (void) {

  TC_UCSRB &= ~_BV (RXCIE);
}

// -----------------------------------------------------------------------------
INLINE void
vTxUdreIrqEnable (void) {

  TC_UCSRB &= ~_BV (TXCIE);
  TC_UCSRB |= _BV (UDRIE);
}

// -----------------------------------------------------------------------------
INLINE void
vTxTxcIrqEnable (void) {

  TC_UCSRB &= ~_BV (UDRIE);
  TC_UCSRB |= _BV (TXCIE);
}

// -----------------------------------------------------------------------------
INLINE void
vTxIrqDisable (void) {

  TC_UCSRB &= ~ (_BV (TXCIE) | _BV (UDRIE));
}

#else /* AVRIO_TC_FLAVOUR & TC_FLAVOUR_IRQ == 0 */
// -----------------------------------------------------------------------------
#define vRxIrqEnable()
#define vRxIrqDisable()
#define vTxUdreIrqEnable()
#define vTxIrqDisable()
// -----------------------------------------------------------------------------
#endif /* AVRIO_TC_FLAVOUR & TC_FLAVOUR_IRQ != 0 */

/* ========================================================================== */
#endif /* AVRIO_TC_ENABLE00 defined */
#endif /* _AVRIO_TC_IRQ_PRIVATE_H_ */
