/*
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
#ifndef _AVRIO_BOARD_IRQ_H_
#define _AVRIO_BOARD_IRQ_H_
/* ========================================================================== */
#include <avr/io.h>

/* INT pins ================================================================= */

/* configuration ============================================================ */
#define IRQ_ISR_MASK 0b00000011
#define IRQ_ISR_NOBLOCK

/* inline public functions ================================================== */
// -----------------------------------------------------------------------------
INLINE void
vIrqClearFlag (int8_t i) {

  EIFR |= _BV(i);
}

// -----------------------------------------------------------------------------
INLINE void
vIrqEnable (int8_t i) {

  vIrqClearFlag (i);
  EIMSK |= _BV(i);
}

// -----------------------------------------------------------------------------
INLINE void
vIrqDisable (int8_t i) {

  EIMSK &= ~(1<<i);
}

// -----------------------------------------------------------------------------
INLINE void
vIrqSetMode (int8_t i, eIrqMode mode) {

  EICRA &= ~(3 << (i*2));
  EICRA |= (mode & 3) << (i*2);
  xIrq[i].mode = mode;
}

// -----------------------------------------------------------------------------
INLINE bool
bIrqReadPin (int8_t i) {

  return (PIND & _BV(i+2)) != 0;
}

// -----------------------------------------------------------------------------
INLINE void
vIrqInitPin (int8_t i) {

  // DDRD &= ~_BV(i+2); // Inutile, au RESET les broches sont en entrée
}

// -----------------------------------------------------------------------------
INLINE void
vIrqRaise (int8_t i) {

  // Broches en entrée, impossible !
}

/* ========================================================================== */
#endif /* _AVRIO_BOARD_IRQ_H_ */
