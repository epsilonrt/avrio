/**
 * @file delay.c
 * @brief Mesures de temps
 * @author Copyright � 2013 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * @version $Id$
 * Revision History ---
 *    20131202 - Initial version by epsilonRT from wiring.c arduino file.
 *------------------------------------------------------------------------------
  wiring.c - Partial implementation of the Wiring API for the ATmega8.
  Part of Arduino - http://www.arduino.cc/

  Copyright (c) 2005-2006 David A. Mellis
  Copyright (c) 2013 David A. Mellis

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General
  Public License along with this library; if not, write to the
  Free Software Foundation, Inc., 59 Temple Place, Suite 330,
  Boston, MA  02111-1307  USA
*/

#include "avrio-config.h"

#if !defined(AVRIO_AVRX_ENABLE) && defined(AVRIO_TASK_ENABLE)
/* ========================================================================== */
#  include <avrio/delay.h>

/* constants ================================================================ */
// the prescaler is set so that timer0 ticks every 64 clock cycles, and the
// the overflow handler is called every 256 ticks.
#define MICROSECONDS_PER_TIMER0_OVERFLOW (clockCyclesToMicroseconds(64 * 256))

// the whole number of milliseconds per timer0 overflow
#define MILLIS_INC (MICROSECONDS_PER_TIMER0_OVERFLOW / 1000)

// the fractional number of milliseconds per timer0 overflow. we shift right
// by three to fit these numbers into a byte. (for the clock speeds we care
// about - 8 and 16 MHz - this doesn't lose precision.)
#define FRACT_INC ((MICROSECONDS_PER_TIMER0_OVERFLOW % 1000) >> 3)
#define FRACT_MAX (1000 >> 3)

/* private variables ======================================================== */
static volatile unsigned long timer_overflow_count = 0;
static volatile unsigned long timer_millis = 0;
static unsigned char timer_fract = 0;

/* private functions ======================================================== */

// -----------------------------------------------------------------------------
ISR(TIMER0_OVF_vect)
{
	// copy these to local variables so they can be stored in registers
	// (volatile variables must be read from memory on every access)
	unsigned long m = timer_millis;
	unsigned char f = timer_fract;

	m += MILLIS_INC;
	f += FRACT_INC;
	if (f >= FRACT_MAX) {
		f -= FRACT_MAX;
		m += 1;
	}

	timer_fract = f;
	timer_millis = m;
	timer_overflow_count++;
}

/* internal public functions ================================================ */

// -----------------------------------------------------------------------------
unsigned long millis()
{
	unsigned long m;
	uint8_t oldSREG = SREG;

	// disable interrupts while we read timer_millis or we might get an
	// inconsistent value (e.g. in the middle of a write to timer_millis)
	cli();
	m = timer_millis;
	SREG = oldSREG;

	return m;
}

// -----------------------------------------------------------------------------
unsigned long micros() {
	unsigned long m;
	uint8_t oldSREG = SREG, t;
	
	cli();
	m = timer_overflow_count;
	t = TCNT0;
  
#ifdef TIFR0
	if ((TIFR0 & _BV(TOV0)) && (t < 255))
		m++;
#else
	if ((TIFR & _BV(TOV0)) && (t < 255))
		m++;
#endif

	SREG = oldSREG;
	
	return ((m << 8) + t) * (64 / clockCyclesPerMicrosecond());
}


#endif /* AVRIO_DELAYTIME_ENABLE defined */
/* ========================================================================== */
