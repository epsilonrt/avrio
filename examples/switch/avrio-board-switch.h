/**
 * @file avrio-board-switch.h
 * @brief Driver pour module micro-interrupteurs.
 * @author Copyright © 2014 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>

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
