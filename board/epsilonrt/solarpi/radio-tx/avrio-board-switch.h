/**
 * Copyright © 2015 Pascal JEAN aka epsilonRT <pascal.jean--AT--btssn.net>
 * All rights reserved.
 */
#ifndef _AVRIO_BOARD_SWITCH_H_
#  define _AVRIO_BOARD_SWITCH_H_
/* ========================================================================== */

/* SWITCH =================================================================== */
#  include <avrio/defs.h>
#  include <avr/io.h>

/* types ==================================================================== */
typedef uint8_t xSwitch;

/* constants ================================================================ */
#define SWITCH0 _BV(PB0)
#define SWITCH1 _BV(PB1)

/* internal public functions ================================================ */

// -----------------------------------------------------------------------------
INLINE void
vSwitchHwInit (void) {

  PORTB |=  (SWITCH1 | SWITCH0);
}

// -----------------------------------------------------------------------------
INLINE xSwitch
xSwitchHwGet (void) {

  xSwitch xCurrent = (PINB & SWITCH0 ? 0 : 1);
  xCurrent += (PINB & SWITCH1 ? 0 : 2);
  return xCurrent;
}

/* ========================================================================== */
#endif /* _AVRIO_BOARD_SWITCH_H_ */
