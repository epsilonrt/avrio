/**
 * Copyright © 2011-2015 Pascal JEAN aka epsilonRT. All rights reserved.
 *
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
 *
 * @file avrio-board-switch.h
 * @brief Driver pour module micro-interrupteurs.
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
#define SWITCH1 _BV(PD2)
#define SWITCH2 _BV(PB2)

/* internal public functions ================================================ */

// -----------------------------------------------------------------------------
INLINE void
vSwitchHwInit (void) {

  PORTD |=  SWITCH1;
  DDRD  &= ~SWITCH1;
  PORTB |=  SWITCH2;
  DDRB  &= ~SWITCH2;
}

// -----------------------------------------------------------------------------
INLINE xSwitch
xSwitchHwGet (void) {

  xSwitch xCurrent = (PIND & SWITCH1 ? 0 : 1);
  xCurrent += (PINB & SWITCH2 ? 0 : 2);
  return xCurrent;
}

/* ========================================================================== */
#endif /* _AVRIO_BOARD_SWITCH_H_ */
