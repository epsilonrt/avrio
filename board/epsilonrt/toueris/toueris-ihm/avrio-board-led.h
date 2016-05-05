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
#ifndef _AVRIO_BOARD_LED_H_
#  define _AVRIO_BOARD_LED_H_
/* ========================================================================== */

/* LED ====================================================================== */
#  include <avrio/defs.h>
#  include <avr/io.h>

/* constants ================================================================ */
#  define LED_QUANTITY  11

/* PORTD */
#  define LED_LED9  _BV(0)
#  define LED_LED8  _BV(1)

/* PORTB */
#  define LED_LED1  _BV(2)
#  define LED_LED2  _BV(3)
#  define LED_LED3  _BV(5)
#  define LED_LED7  _BV(6)
#  define LED_LED6  _BV(7)

/* PORTC */
#  define LED_LED4  _BV(0+8)/* PC0 */
#  define LED_LED5  _BV(1+8)/* PC1 */
#  define LED_LED11 _BV(2+8)/* PC2 */
#  define LED_LED10 _BV(3+8)/* PC3 */

/* PORTC */
#  define LED_PORTD_LEDS (  LED_LED8 | LED_LED9 )
#  define LED_PORTB_LEDS (  LED_LED1 | LED_LED2 | LED_LED3 | LED_LED6 | LED_LED7 )
#  define LED_PORTC_LEDS (  LED_LED4 | LED_LED5 | LED_LED10 | LED_LED11 )

#  define LED_ALL_LEDS (  LED_PORTB_LEDS | LED_PORTC_LEDS | LED_PORTD_LEDS )

#  define LED_NO_LED (0)

/* types ==================================================================== */
typedef uint16_t xLedMask;

/* inline public functions ================================================== */

// ------------------------------------------------------------------------------
static inline void
vLedClear (xLedMask xMask) {

  PORTB |= (xMask & LED_PORTB_LEDS);
  PORTD |= (xMask & LED_PORTD_LEDS);
  PORTC |= (xMask & LED_PORTC_LEDS) >> 8;
}

// ------------------------------------------------------------------------------
static inline void
vLedInit (void) {

  DDRB |= LED_PORTB_LEDS;
  DDRD |= LED_PORTD_LEDS;
  DDRC |= LED_PORTC_LEDS >> 8;
  vLedClear (LED_ALL_LEDS);
}

// ------------------------------------------------------------------------------
static inline void
vLedSet (xLedMask xMask) {

  PORTB &= ~(xMask & LED_PORTB_LEDS);
  PORTD &= ~(xMask & LED_PORTD_LEDS);
  PORTC &= ~((xMask & LED_PORTC_LEDS) >> 8);
}

// ------------------------------------------------------------------------------
static inline void
vLedToggle (xLedMask xMask) {

  PORTB ^= (xMask & LED_PORTB_LEDS);
  PORTD ^= (xMask & LED_PORTD_LEDS);
  PORTC ^= (xMask & LED_PORTC_LEDS) >> 8;
}

/* public variables ========================================================= */
#  if defined(LED_MASK_ARRAY_ENABLE)
#    define DECLARE_LED_MASK_ARRAY  \
  const xLedMask \
    xLedMaskArray [LED_QUANTITY] = { \
      LED_LED1, \
      LED_LED2, \
      LED_LED3, \
      LED_LED4, \
      LED_LED5, \
      LED_LED6, \
      LED_LED7, \
      LED_LED8, \
      LED_LED9, \
      LED_LED10, \
      LED_LED11  \
    }
#  else
#    define DECLARE_LED_MASK_ARRAY
#  endif

/* ========================================================================== */
#endif /* _AVRIO_BOARD_LED_H_ */
