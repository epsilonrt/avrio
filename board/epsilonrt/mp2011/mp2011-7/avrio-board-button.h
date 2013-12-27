/* Copyright © 2009-2012 epsilonRT. All rights reserved.
 *  $Id$ */
#ifndef _AVRIO_BOARD_BUTTON_H_
#  define _AVRIO_BOARD_BUTTON_H_
/* ========================================================================== */

/* BUTTON==================================================================== */
#  include <avrio/defs.h>
#  include <avr/io.h>

/* constants ================================================================ */
#  define BUTTON_QUANTITY  1
#  define BUTTON_BUTTON1 _BV(6)
#  define BUTTON_ALL_BUTTONS ( BUTTON_BUTTON1 )
#  define BUTTON_NO_BUTTON (0)
/* types ==================================================================== */
typedef uint8_t xButMask;

/* inline public functions ================================================== */
#  define BUTTON_COMMON _BV(2)// ligne 1 du clavier

// ------------------------------------------------------------------------------
static inline void
vButHardwareInit (void) {

  PORTA |= BUTTON_ALL_BUTTONS;
  DDRA &= ~BUTTON_ALL_BUTTONS;
  // Mets la ligne commune des boutons en sortie à l'état bas
  PORTD &= ~BUTTON_COMMON;
  DDRD |= BUTTON_COMMON;
}

// ------------------------------------------------------------------------------
static inline xButMask
xButHardwareGet (xButMask xMask) {

  return (PINA ^ BUTTON_ALL_BUTTONS) & xMask;
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
