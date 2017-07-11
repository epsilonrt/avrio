/*
 * Copyright © 2016 epsilonRT, All rights reserved.
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
#define _AVRIO_BOARD_LED_H_
/* ========================================================================== */

/* LED ====================================================================== */
#include <avrio/defs.h>
#include <avr/io.h>

/* constants ================================================================ */
#define LED_QUANTITY  1

#define BOARD_LED_PORT PORTB
#define BOARD_LED_DDR  DDRB
#define LED_LED1 _BV(3)
#define LED_ALL_LEDS ( LED_LED1 )
#define LED_NO_LED (0)
#define LED_DBG LED_LED1

/* types ==================================================================== */
typedef uint8_t xLedMask;

/* inline public functions ================================================== */
// ------------------------------------------------------------------------------
static inline void
vLedClear (xLedMask xMask) {

  BOARD_LED_PORT &= ~(xMask & LED_ALL_LEDS);
}

// ------------------------------------------------------------------------------
static inline void
vLedInit (void) {

  BOARD_LED_DDR |= LED_ALL_LEDS;
  vLedClear (LED_ALL_LEDS);
}

// ------------------------------------------------------------------------------
static inline void
vLedSet (xLedMask xMask) {

  BOARD_LED_PORT |= (xMask & LED_ALL_LEDS);
}

// ------------------------------------------------------------------------------
static inline void
vLedToggle (xLedMask xMask) {

  BOARD_LED_PORT ^= (xMask & LED_ALL_LEDS);
}

/* public variables ========================================================= */

#if defined(LED_MASK_ARRAY_ENABLE)
#define DECLARE_LED_MASK_ARRAY  \
  const xLedMask \
    xLedMaskArray [LED_QUANTITY] = { \
      LED_LED1 \
    }
#else
#define DECLARE_LED_MASK_ARRAY
#endif

/* ========================================================================== */
#endif /* _AVRIO_BOARD_LED_H_ */
