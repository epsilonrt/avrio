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
// 3 interruptions dispo. INT0 à INT2
#define IRQ_ISR_MASK 0b00000111
// #define IRQ_ISR_NOBLOCK

/* inline public functions ================================================== */
// -----------------------------------------------------------------------------
INLINE void
vIrqClearFlag (int8_t i) {

  EIFR |= _BV (i);
}

// -----------------------------------------------------------------------------
INLINE void
vIrqEnable (int8_t i) {

  vIrqClearFlag (i);
  EIMSK |= _BV (i);
}

// -----------------------------------------------------------------------------
INLINE void
vIrqDisable (int8_t i) {

  EIMSK &= ~ (1 << i);
}

// -----------------------------------------------------------------------------
INLINE void
vIrqSetMode (int8_t i, eIrqMode mode) {

  EICRA &= ~ (3 << (i * 2));
  EICRA |= (mode & 3) << (i * 2);
  xIrq[i].mode = mode;
}

// -----------------------------------------------------------------------------
INLINE bool
bIrqReadPin (int8_t i) {
  if (i >= 2) {
    return (PINB & _BV (2)) != 0;
  }
  return (PIND & _BV (i + 2)) != 0;
}

// -----------------------------------------------------------------------------
INLINE void
vIrqInitPin (int8_t i) {
#ifdef IRQ_INIT_ENABLE
  if (i >= 2) {
    DDRB &= ~_BV (2);
    PORTD |= _BV (2);
  }
  else {
    DDRD  &= ~_BV (i + 2);
    PORTD |= _BV (i + 2);
  }
#endif
}

// -----------------------------------------------------------------------------
INLINE void
vIrqRaise (int8_t i) {

  // Broches en entrée, impossible !
}

/* ========================================================================== */
#endif /* _AVRIO_BOARD_IRQ_H_ */
