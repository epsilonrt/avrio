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
 * @file itimer.c
 * @brief Driver Timer - Timer0 ATmega168/328 version
 *
 * Revision History ------------------------------------------------------------
 *    20120229 - Initial version by epsilonRT
 */
#include "timer.h"
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>

/* public variables ========================================================= */
U8 gu8MsTimer1;
U8 gu8MsTimer2;
U8 gu8MsClock;

/* internal public functions ================================================ */

// -----------------------------------------------------------------------------
void 
TimerInit (void) {

  TCCR0B = _BV(CS01) | _BV(CS00);
  TCCR0A = 0;
  TIMSK0 = _BV(TOIE0);
}

// -----------------------------------------------------------------------------
void 
TimerDelayUsec (U16 Us) {

  while (Us) {

    _delay_loop_2 (F_CPU / (4.0 * 1E6));
    Us--;
  }
}

// -----------------------------------------------------------------------------
void 
TimerDelayMsec (U16 Ms) {

  while (Ms) {

    _delay_loop_2 (F_CPU / (4.0 * 1000.0));
    Ms--;
  }
}

/* private functions ======================================================== */

// -----------------------------------------------------------------------------
ISR(TIMER0_OVF_vect) {
  
  TCNT0 = 0x8D; // 0xFF - 0x8D = 0x73 = 115 -> 115 * 64 / F_CPU # 1 ms
  gu8MsTimer1++;
  gu8MsTimer2++;
  gu8MsClock++;
}

/* ========================================================================== */
