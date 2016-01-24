#ifndef _AVRIO_BOARD_BUTTON_H_
#  define _AVRIO_BOARD_BUTTON_H_
/* ========================================================================== */

/* BUTTON==================================================================== */
#  include <avrio/defs.h>
#  include <avr/io.h>
#  include <avr/interrupt.h>

/* constants ================================================================ */
/* Configuration des boutons
 * BUTTON   PIN     PCINT   ACTIVE LEVEL
 * -----------------------------------------------------------------------------
 * BUTTON1  PB4     PCINT4  LOW
 */
#  define BUTTON_QUANTITY  1
#  define BUTTON_BUTTON1 _BV(4)
#  define BUTTON_ALL_BUTTONS (BUTTON_BUTTON1)
#  define BUTTON_NO_BUTTON (0)

// PCINT
#  define BUTTON_vect           PCINT0_vect
#  define BUTTON_PCIE           PCIE0
#  define BUTTON_PCMSK          PCMSK0
#  define BUTTON_BUTTON1_PCINT  PCINT4

#  if !defined(PCICR) && defined(GIMSK)
#   define PCICR GIMSK
#  endif

/* types ==================================================================== */
typedef uint8_t xButMask;

/* inline public functions ================================================== */
// ------------------------------------------------------------------------------
static inline void
vButHardwareInit (void) {

  PORTB |= BUTTON_ALL_BUTTONS;
  DDRB &= ~BUTTON_ALL_BUTTONS;
}

// ------------------------------------------------------------------------------
static inline xButMask
xButHardwareGet (xButMask xMask) {

  return (PINB ^ BUTTON_ALL_BUTTONS) & xMask;
}

/* public variables ========================================================= */
#  if defined(BUTTON_MASK_ARRAY_ENABLE)
#    define DECLARE_BUTTON_MASK_ARRAY  \
  const xButMask \
    xButMaskArray [BUTTON_QUANTITY] = { \
      BUTTON_BUTTON1 \
    }
#  else
#    define DECLARE_BUTTON_MASK_ARRAY
#  endif

/* ========================================================================== */
#endif /* _AVRIO_BOARD_BUTTON_H_ */
