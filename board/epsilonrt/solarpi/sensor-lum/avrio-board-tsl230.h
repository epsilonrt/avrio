/**
 * Copyright © 2015 Pascal JEAN aka epsilonRT <pascal.jean--AT--btssn.net>
 * All rights reserved.
 */
#ifndef _AVRIO_BOARD_TSL230_H_
#define _AVRIO_BOARD_TSL230_H_
/* ========================================================================== */
#include <avr/io.h>

/* TSL230 Sensor ============================================================ */

/* configuration ============================================================ */
#define TSL230_INT INT0
#define TSL230_SENS_DEFAULT eTsl230Sensitivity10
#define TSL230_SCAL_DEFAULT eTsl230Scale1
#define TSL230_DARK_FREQ_DEFAULT 0.4

#define TSL230_S0 0
#define TSL230_S1 1
#define TSL230_SENS_PORT  PORTC
#define TSL230_SENS_DDR   DDRC

#define TSL230_S2 2
#define TSL230_S3 3
#define TSL230_SCAL_PORT  PORTC
#define TSL230_SCAL_DDR   DDRC

// #define TSL230_OE 4
#define TSL230_OE_PORT  PORTC
#define TSL230_OE_DDR   DDRC

/* inline public functions ================================================== */
#if defined(TSL230_OE) && defined(TSL230_OE_PORT) && defined(TSL230_OE_DDR)
#define TSL230_OE_ENABLE
#endif


// -----------------------------------------------------------------------------
INLINE void
vTsl230PinSetSensitivity (eTsl230Sensitivity eSensitivity) {

  if (eSensitivity & _BV (0)) {

    TSL230_SENS_PORT |= _BV (TSL230_S0);
  }
  else {

    TSL230_SENS_PORT &= ~_BV (TSL230_S0);
  }

  if (eSensitivity & _BV (1)) {

    TSL230_SENS_PORT |= _BV (TSL230_S1);
  }
  else {

    TSL230_SENS_PORT &= ~_BV (TSL230_S1);
  }
}

// -----------------------------------------------------------------------------
INLINE void
vTsl230PinSetScale (eTsl230Scale eScale) {

  if (eScale & _BV (0)) {

    TSL230_SCAL_PORT |= _BV (TSL230_S2);
  }
  else {

    TSL230_SCAL_PORT &= ~_BV (TSL230_S2);
  }
  if (eScale & _BV (1)) {

    TSL230_SCAL_PORT |= _BV (TSL230_S3);
  }
  else {

    TSL230_SCAL_PORT &= ~_BV (TSL230_S3);
  }
}

// -----------------------------------------------------------------------------
INLINE void
vTsl230Disable (void) {
#ifdef TSL230_OE_ENABLE
  TSL230_OE_PORT |= _BV (TSL230_OE);
#endif
}

// -----------------------------------------------------------------------------
INLINE void
vTsl230Enable (void) {
#ifdef TSL230_OE_ENABLE
  TSL230_OE_PORT &= ~_BV (TSL230_OE);
#endif
}

// -----------------------------------------------------------------------------
INLINE void
vTsl230PinInit (void) {

  TSL230_SENS_DDR |= _BV (TSL230_S1) | _BV (TSL230_S0);
  TSL230_SCAL_DDR |= _BV (TSL230_S3) | _BV (TSL230_S2);
#ifdef TSL230_OE_ENABLE
  TSL230_OE_DDR |= _BV (TSL230_OE);
#endif
}

/* ========================================================================== */
#endif /* _AVRIO_BOARD_TSL230_H_ */
