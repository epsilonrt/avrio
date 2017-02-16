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
