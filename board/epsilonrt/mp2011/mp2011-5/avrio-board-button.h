/*
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
#ifndef _AVRIO_BOARD_BUTTON_H_
#  define _AVRIO_BOARD_BUTTON_H_
/* ========================================================================== */

/* BUTTON==================================================================== */
#  include <avrio/defs.h>
#  include <avr/io.h>

/* constants ================================================================ */
#  define BUTTON_QUANTITY  4
#  define BUTTON_BUTTON1 _BV(4)
#  define BUTTON_BUTTON2 _BV(5)
#  define BUTTON_BUTTON3 _BV(6)
#  define BUTTON_BUTTON4 _BV(7)
#  define BUTTON_ALL_BUTTONS (  BUTTON_BUTTON1 | BUTTON_BUTTON2 | \
                              BUTTON_BUTTON3 | BUTTON_BUTTON4 )
#  define BUTTON_NO_BUTTON (0)

/* types ==================================================================== */
typedef uint8_t xButMask;

/* inline public functions ================================================== */
#  define BUTTON_COMMON _BV(0)// ligne 1 du clavier

// ------------------------------------------------------------------------------
static inline void
vButHardwareInit (void) {

  PORTA |= BUTTON_ALL_BUTTONS;
  DDRA &= ~BUTTON_ALL_BUTTONS;
  // Mets la ligne commune des boutons en sortie à l'état bas
  PORTA &= ~BUTTON_COMMON;
  DDRA |= BUTTON_COMMON;
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
      BUTTON_BUTTON1, \
      BUTTON_BUTTON2, \
      BUTTON_BUTTON3, \
      BUTTON_BUTTON4 \
    }
#  else
#    define DECLARE_BUTTON_MASK_ARRAY
#  endif

/* ========================================================================== */
#endif /* _AVRIO_BOARD_BUTTON_H_ */
