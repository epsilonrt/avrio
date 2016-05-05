/**
 * @file serial_private.h
 * @brief Entête privé liaison série asynchrone
 *
 * Copyright © 2011-2015 epsilonRT. All rights reserved.
 *
 * This file is part of AvrIO.
 *
 * This software is governed by the CeCILL license under French law and
 * abiding by the rules of distribution of free software.  You can  use, 
 * modify and/ or redistribute the software under the terms of the CeCILL
 * license as circulated by CEA, CNRS and INRIA at the following URL
 * <http://www.cecill.info>. 
 * 
 * The fact that you are presently reading this means that you have had
 * knowledge of the CeCILL license and that you accept its terms.
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
extern int8_t iSerialTxEn;
extern int8_t iSerialRxEn;

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
