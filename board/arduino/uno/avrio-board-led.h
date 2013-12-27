/* Copyright _ 2009-2012 epsilonRT. All rights reserved.
 *  $Id$ */
#ifndef _AVRIO_BOARD_LED_H_
#  define _AVRIO_BOARD_LED_H_
/* ========================================================================== */

/* LED ====================================================================== */
#  include <avrio/defs.h>
#  include <avr/io.h>

/* constants ================================================================ */
#  ifndef LED_EXTENDED_ENABLE
#   define LED_QUANTITY  1
#   define LED_LED1 _BV(5)
#   define LED_ALL_LEDS ( LED_LED1 )
#  else
#   define LED_QUANTITY  3
#   define LED_LED1 _BV(2)
#   define LED_LED2 _BV(1)
#   define LED_LED3 _BV(5)
#   define LED_ALL_LEDS ( LED_LED1 | LED_LED2 | LED_LED3 )
#  endif

#  define LED_NO_LED (0)

/* types ==================================================================== */
typedef uint8_t xLedMask;

/* inline public functions ================================================== */
// ------------------------------------------------------------------------------
static inline void
vLedClear (xLedMask xMask) {

  PORTB &= ~(xMask & LED_ALL_LEDS);
}

// ------------------------------------------------------------------------------
static inline void
vLedInit (void) {

  DDRB |= LED_ALL_LEDS;
  vLedClear (LED_ALL_LEDS);
}

// ------------------------------------------------------------------------------
static inline void
vLedSet (xLedMask xMask) {

  PORTB |= (xMask & LED_ALL_LEDS);
}

// ------------------------------------------------------------------------------
static inline void
vLedToggle (xLedMask xMask) {

  PORTB ^= (xMask & LED_ALL_LEDS);
}

/* public variables ========================================================= */
#  if defined(LED_MASK_ARRAY_ENABLE)

#    ifndef LED_EXTENDED_ENABLE

#      define DECLARE_LED_MASK_ARRAY  \
        const xLedMask \
          xLedMaskArray [LED_QUANTITY] = { \
            LED_LED1 \
          }
#    else

#      define DECLARE_LED_MASK_ARRAY  \
        const xLedMask \
          xLedMaskArray [LED_QUANTITY] = { \
            LED_LED1, \
            LED_LED2, \
            LED_LED3 \
          }
#    endif

#  else
#    define DECLARE_LED_MASK_ARRAY
#  endif

/* ========================================================================== */
#endif /* _AVRIO_BOARD_LED_H_ */
