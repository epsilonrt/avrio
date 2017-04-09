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
// 1 interruption dispo. INT0
#define IRQ_ISR_MASK 0b00000001
// #define IRQ_ISR_NOBLOCK

/* inline public functions ================================================== */
// -----------------------------------------------------------------------------
INLINE void
vIrqClearFlag (int8_t i) {

  GIFR |= _BV(INTF0);
}

// -----------------------------------------------------------------------------
INLINE void
vIrqEnable (int8_t i) {

  vIrqClearFlag (0);
  GIMSK |= _BV(INT0);
}

// -----------------------------------------------------------------------------
INLINE void
vIrqDisable (int8_t i) {

  GIMSK &= ~_BV(INT0);
}

// -----------------------------------------------------------------------------
INLINE void
vIrqSetMode (int8_t i, eIrqMode mode) {

  MCUCR &= ~(3);
  MCUCR |= (mode & 3);
  xIrq[i].mode = mode;
}

// -----------------------------------------------------------------------------
INLINE bool
bIrqReadPin (int8_t i) {

  return (PINB & _BV(1)) != 0;
}

// -----------------------------------------------------------------------------
INLINE void
vIrqInitPin (int8_t i) {

  DDRB  &= ~_BV(1);
  PUEB |= _BV(1);
}

// -----------------------------------------------------------------------------
INLINE void
vIrqRaise (int8_t i) {

  // Broches en entrée, impossible !
}

/* ========================================================================== */
#endif /* _AVRIO_BOARD_IRQ_H_ */
