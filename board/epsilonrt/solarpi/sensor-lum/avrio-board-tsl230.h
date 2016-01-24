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
#ifndef _AVRIO_BOARD_TSL230_H_
#define _AVRIO_BOARD_TSL230_H_
/* ========================================================================== */
#include <avr/io.h>

/* TSL230 Sensor ============================================================ */

/* configuration ============================================================ */
#define TSL230_INT INT0
#define TSL230_DEFAULT_SENSITIVITY eTsl230Sensitivity10
#define TSL230_DEFAULT_SCALE eTsl230Scale1
#define TSL230_DEFAULT_DARK_FREQ 0.4
#define TSL230_DEFAULT_RESPONSITIVITY 790.

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
