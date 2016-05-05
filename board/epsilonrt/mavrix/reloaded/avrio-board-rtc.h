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
