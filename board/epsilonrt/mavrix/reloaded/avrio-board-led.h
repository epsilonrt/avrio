/* Copyright _ 2009-2012 epsilonRT. All rights reserved.
 *  $Id$ */
#ifndef _AVRIO_BOARD_LED_H_
#  define _AVRIO_BOARD_LED_H_
/* ========================================================================== */

/* LED ====================================================================== */
#  include <avrio/defs.h>
#  include <avr/io.h>

/* constants ================================================================ */

#  define LED_LED1       _BV(6) // PORTD
#  define LED_LED2       _BV(7) // PORTB
#  define LED_QUANTITY   2
#  define LED_ALL_LEDS   (LED_LED1|LED_LED2)

#  define LED_NO_LED (0)

/* types ==================================================================== */
typedef uint8_t xLedMask;

/* inline public functions ================================================== */
// ------------------------------------------------------------------------------
static inline void
vLedClear (xLedMask xMask) {

  PORTD |= (xMask & LED_LED1);
  PORTB |= (xMask & LED_LED2);
}

// ------------------------------------------------------------------------------
static inline void
vLedInit (void) {

  DDRD |= LED_LED1;
  DDRB |= LED_LED2;
  vLedClear (LED_ALL_LEDS);
}

// ------------------------------------------------------------------------------
static inline void
vLedSet (xLedMask xMask) {

  PORTD &= ~(xMask & LED_LED1);
  PORTB &= ~(xMask & LED_LED2);
}

// ------------------------------------------------------------------------------
static inline void
vLedToggle (xLedMask xMask) {

  PORTD ^= (xMask & LED_LED1);
  PORTB ^= (xMask & LED_LED2);
}

/* public variables ========================================================= */
#  if defined(LED_MASK_ARRAY_ENABLE)

#    define DECLARE_LED_MASK_ARRAY  \
      const xLedMask \
       xLedMaskArray [LED_QUANTITY] = { \
          LED_LED1, \
          LED_LED2 \
        }
#  else
#    define DECLARE_LED_MASK_ARRAY
#  endif

/* ========================================================================== */
#endif /* _AVRIO_BOARD_LED_H_ */
