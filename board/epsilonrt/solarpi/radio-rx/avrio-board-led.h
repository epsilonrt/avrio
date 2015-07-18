/* Copyright © 2009-2012 epsilonRT. All rights reserved.
 *  $Id$ */
#ifndef _AVRIO_BOARD_LED_H_
#define _AVRIO_BOARD_LED_H_
/* ========================================================================== */

/* LED ====================================================================== */
#include <avrio/defs.h>
#include <avr/io.h>

/* constants ================================================================ */
#define LED_LED1       _BV(2)
#define LED_LED2       _BV(4)
#define LED_QUANTITY   2
#define LED_ALL_LEDS   (LED_LED1 | LED_LED2)

#define LED_NO_LED (0)

#define LED_RCV  LED_LED1
#define LED_RX   LED_LED1
#define LED_D1   LED_LED1
#define LED_FRM  LED_LED2
#define LED_D3   LED_LED2
/* types ==================================================================== */
typedef uint8_t xLedMask;

/* inline public functions ================================================== */
// ------------------------------------------------------------------------------
static inline void
vLedClear (xLedMask xMask) {

  PORTB &= ~(xMask & LED_LED1);
  PORTD &= ~(xMask & LED_LED2);
}

// ------------------------------------------------------------------------------
static inline void
vLedInit (void) {

  DDRB |= LED_LED1;
  DDRD |= LED_LED2;
  vLedClear (LED_ALL_LEDS);
}

// ------------------------------------------------------------------------------
static inline void
vLedSet (xLedMask xMask) {

  PORTB |= (xMask & LED_LED1);
  PORTD |= (xMask & LED_LED2);
}

// ------------------------------------------------------------------------------
static inline void
vLedToggle (xLedMask xMask) {

  PORTB ^= (xMask & LED_LED1);
  PORTD ^= (xMask & LED_LED2);
}

/* public variables ========================================================= */
#if defined(LED_MASK_ARRAY_ENABLE)

#define DECLARE_LED_MASK_ARRAY  \
      const xLedMask \
       xLedMaskArray [LED_QUANTITY] = { \
          LED_LED1, \
          LED_LED2 \
        }
#else
#define DECLARE_LED_MASK_ARRAY
#endif

/* ========================================================================== */
#endif /* _AVRIO_BOARD_LED_H_ */
