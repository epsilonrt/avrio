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
