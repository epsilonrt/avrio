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
