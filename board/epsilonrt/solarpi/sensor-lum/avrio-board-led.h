/**
 * Copyright © 2015 Pascal JEAN aka epsilonRT <pascal.jean--AT--btssn.net>
 * All rights reserved.
 */
#ifndef _AVRIO_BOARD_LED_H_
#define _AVRIO_BOARD_LED_H_
/* ========================================================================== */

/* LED ====================================================================== */
#include <avrio/defs.h>
#include <avr/io.h>

/* constants ================================================================ */

#define LED_LED1       _BV(6)
#define LED_D1         LED_LED1
#define LED_QUANTITY   1
#define LED_ALL_LEDS   (LED_LED1)

#define LED_NO_LED (0)

/* types ==================================================================== */
typedef uint8_t xLedMask;

/* inline public functions ================================================== */
// ------------------------------------------------------------------------------
static inline void
vLedClear (xLedMask xMask) {

  PORTD &= ~(xMask & LED_ALL_LEDS);
}

// ------------------------------------------------------------------------------
static inline void
vLedInit (void) {

  DDRD |= LED_ALL_LEDS;
  vLedClear (LED_ALL_LEDS);
}

// ------------------------------------------------------------------------------
static inline void
vLedSet (xLedMask xMask) {

  PORTD |= (xMask & LED_ALL_LEDS);
}

// ------------------------------------------------------------------------------
static inline void
vLedToggle (xLedMask xMask) {

  PORTD ^= (xMask & LED_ALL_LEDS);
}

/* public variables ========================================================= */
#if defined(LED_MASK_ARRAY_ENABLE)

#define DECLARE_LED_MASK_ARRAY  \
      const xLedMask \
       xLedMaskArray [LED_QUANTITY] = { \
          LED_LED1\
        }
#else
#define DECLARE_LED_MASK_ARRAY
#endif

/* ========================================================================== */
#endif /* _AVRIO_BOARD_LED_H_ */
