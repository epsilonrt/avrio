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
#ifndef _AVRIO_BOARD_RTC_H_
#  define _AVRIO_BOARD_RTC_H_
/* ========================================================================== */

/* KERNEL =================================================================== */
#  include <avrio/defs.h>
#  include <avr/interrupt.h>

/* constants ================================================================ */
//#define RTC_IRQ_vect  INT6_vect

/* inline public functions ================================================== */
/*
 * Valide l'interruption RTC 
 */
static inline void
vRtcIrqEnable (void) {

#ifdef RTC_IRQ_vect
  EIFR  |= _BV(INTF6);
  EIMSK |= _BV(INT6);
#endif
}

/*
 * Dévalide l'interruption RTC
 */
static inline void
vRtcIrqDisable (void) {

#ifdef RTC_IRQ_vect
  EIMSK &= ~_BV(INT6);
#endif
}

/*
 * Initialise la partie matérielle spécifique à la carte
 */
static inline void
vRtcHardwareInit (void) {

#ifdef RTC_IRQ_vect
  // Config entrée interruption
  PORTE |= _BV(6);
  DDRE  &= ~_BV(6);
  
  vRtcIrqDisable ();
  EICRB |= _BV(ISC61);
  EICRB &= ~_BV(ISC60);
#endif
}

/* ========================================================================== */
#endif /* _AVRIO_BOARD_RTC_H_ */
