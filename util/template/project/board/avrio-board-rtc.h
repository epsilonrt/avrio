#ifndef _AVRIO_BOARD_RTC_H_
#  define _AVRIO_BOARD_RTC_H_
/* ========================================================================== */

/* KERNEL =================================================================== */
#  include <avrio/defs.h>
#  include <avr/interrupt.h>

/* constants ================================================================ */
#define RTC_IRQ_vect  INT1_vect

/* inline public functions ================================================== */
/*
 * Valide l'interruption RTC 
 */
static inline void
vRtcIrqEnable (void) {

#ifdef RTC_IRQ_vect
  EIFR  |= _BV(INTF1);
  EIMSK |= _BV(INT1);
#endif
}

/*
 * Dévalide l'interruption RTC
 */
static inline void
vRtcIrqDisable (void) {

#ifdef RTC_IRQ_vect
  EIMSK &= ~_BV(INT1);
#endif
}

/*
 * Initialise la partie matérielle spécifique é la carte
 */
static inline void
vRtcHardwareInit (void) {

#ifdef RTC_IRQ_vect
  // Config entrée interruption
  PORTD |= _BV(3);
  DDRD  &= ~_BV(3);
  
  vRtcIrqDisable ();
  EICRA |= _BV(ISC11);
  EICRA &= ~_BV(ISC10);
#endif
}

/* ========================================================================== */
#endif /* _AVRIO_BOARD_RTC_H_ */
