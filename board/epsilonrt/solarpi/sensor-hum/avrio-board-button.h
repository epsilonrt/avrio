/**
 * Copyright © 2015 Pascal JEAN aka epsilonRT <pascal.jean--AT--btssn.net>
 * All rights reserved.
 * This software is governed by the CeCILL license <http://www.cecill.info>
 */
#ifndef _AVRIO_BOARD_BUTTON_H_
#  define _AVRIO_BOARD_BUTTON_H_
/* ========================================================================== */

/* BUTTON==================================================================== */
#  include <avrio/defs.h>
#  include <avr/io.h>

/* constants ================================================================ */
/* Button         Pin Active
 * BUTTON_BUTTON1 PD2 LOW
 */
#  define BUTTON_QUANTITY  1
#  define BUTTON_BUTTON1   _BV(3)
#  define BUTTON_SW1       BUTTON_BUTTON1
#  define BUTTON_ALL_BUTTONS ( BUTTON_BUTTON1 )
#  define BUTTON_NO_BUTTON (0)

/* types ==================================================================== */
typedef uint8_t xButMask;

/* inline public functions ================================================== */
// ------------------------------------------------------------------------------
static inline void
vButHardwareInit (void) {

  PORTD |= BUTTON_BUTTON1;
  DDRD  &= ~BUTTON_BUTTON1;
}

// ------------------------------------------------------------------------------
static inline xButMask
xButHardwareGet (xButMask xMask) {

  return (PIND ^ BUTTON_BUTTON1) & xMask;
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
