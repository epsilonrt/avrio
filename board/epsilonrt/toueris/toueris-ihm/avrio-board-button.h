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
#  define BUTTON_QUANTITY  10

/* PORTD */
#  define BUTTON_BUTTON1  _BV(7)/* PD7 / Y0 = 0 */
#  define BUTTON_BUTTON2  _BV(6)/* PD6 / Y0 = 0 */
#  define BUTTON_BUTTON3  _BV(5)/* PD5 / Y0 = 0 */
#  define BUTTON_BUTTON4  _BV(4)/* PD4 / Y0 = 0 */
#  define BUTTON_BUTTON5  _BV(3)/* PD3 / Y0 = 0 */
#  define BUTTON_BUTTON6  _BV(7+8)/* PD7 / Y1 = 0 */
#  define BUTTON_BUTTON7  _BV(6+8)/* PD6 / Y1 = 0 */
#  define BUTTON_BUTTON8  _BV(5+8)/* PD5 / Y1 = 0 */
#  define BUTTON_BUTTON9  _BV(4+8)/* PD4 / Y1 = 0 */
#  define BUTTON_BUTTON10 _BV(3+8)/* PD3 / Y1 = 0 */

/* PORTB */
#  define BUTTON_Y0       _BV(1)/* PB1 */
#  define BUTTON_Y1       _BV(0)/* PB0 */

#  define BUTTON_Y0_BUTTONS ( BUTTON_BUTTON1 | BUTTON_BUTTON2 | BUTTON_BUTTON3 | \
                              BUTTON_BUTTON4 | BUTTON_BUTTON5 )
#  define BUTTON_Y1_BUTTONS ( BUTTON_BUTTON6 | BUTTON_BUTTON7 | BUTTON_BUTTON8 | \
                              BUTTON_BUTTON9 | BUTTON_BUTTON10 )

#  define BUTTON_ALL_BUTTONS (BUTTON_Y0_BUTTONS | BUTTON_Y1_BUTTONS)

#  define BUTTON_NO_BUTTON (0)

/* types ==================================================================== */
typedef uint16_t xButMask;

/* private functions ======================================================== */
// ------------------------------------------------------------------------------
static void
prvvSwitchY (void) {
  uint8_t ucDelay = 3;

  PORTB ^= BUTTON_Y0;
  PORTB ^= BUTTON_Y1;
  // Setup Time
  while (ucDelay--) {
    NOP ();
  }
}

/* inline public functions ================================================== */
// ------------------------------------------------------------------------------
static inline void
vButHardwareInit (void) {

  PORTD |= BUTTON_Y0_BUTTONS;
  DDRD &= ~BUTTON_Y0_BUTTONS;
  PORTB |= BUTTON_Y1;
  PORTB &= ~BUTTON_Y0;
  DDRB |= BUTTON_Y0 | BUTTON_Y1;
}

// ------------------------------------------------------------------------------
static inline xButMask
xButHardwareGet (xButMask xMask) {
  xButMask xReturnMask = 0;

  xReturnMask = (PIND ^ BUTTON_Y0_BUTTONS) & xMask;
  prvvSwitchY ();

  xReturnMask |= ((PIND ^ BUTTON_Y0_BUTTONS) << 8) & xMask;
  prvvSwitchY ();

  return xReturnMask;
}

/* public variables ========================================================= */
#  if defined(BUTTON_MASK_ARRAY_ENABLE)
#    define DECLARE_BUTTON_MASK_ARRAY  \
  const xButMask \
    xButMaskArray [BUTTON_QUANTITY] = { \
      BUTTON_BUTTON1, \
      BUTTON_BUTTON2, \
      BUTTON_BUTTON3, \
      BUTTON_BUTTON4, \
      BUTTON_BUTTON5, \
      BUTTON_BUTTON6, \
      BUTTON_BUTTON7, \
      BUTTON_BUTTON8, \
      BUTTON_BUTTON9, \
      BUTTON_BUTTON10 \
    }
#  else
#    define DECLARE_BUTTON_MASK_ARRAY
#  endif

/* ========================================================================== */
#endif /* _AVRIO_BOARD_BUTTON_H_ */
