/* Copyright © 2009-2012 epsilonRT. All rights reserved.
 *  $Id$ */
#ifndef _AVRIO_BOARD_MELODY_H_
#  define _AVRIO_BOARD_MELODY_H_
/* ========================================================================== */

/* MELODY =================================================================== */
#  include <avrio/defs.h>
#  include <avr/io.h>

#  ifndef AVRIO_KTIMER_ENABLE
#    warning "ktimer doit être validé pour utiliser le module melody."
#  endif

/* inline public functions ================================================== */
/*
 * Initialise la ressource matérielle (timer) utilisée par le module melody
 * A modifier en fonction du timer et de la sortie utilisée.
 */
static inline void
vMelodyHardwareInit (void) {

  DDRD |= _BV (5);  // PD5 en sortie (OC1A)
  TCCR1A = _BV (WGM10);
  TCCR1B = _BV (WGM13) | _BV (CS11);  // mode 9 : PWM Phase et Freq Correcte,
  // Division par 8
}

/*
 * Génère un son de fréquence \c usFreq
 * A modifier en fonction du timer et de la sortie utilisée.
 * \param usFreq Fréquence du son en Hz
 */
static inline void
vMelodyHardwarePlay (uint16_t usFreq) {

  OCR1A = F_CPU / (4 * 8 * usFreq);
  TCCR1A |= _BV (COM1A0); // sortie OC1A validée (toggle)
}

/*
 * Arrête la génération de son
 * A modifier en fonction du timer et de la sortie utilisée.
 */
static inline void
vMelodyHardwareStop (void) {

  TCCR1A &= ~(_BV (COM1A1) | _BV (COM1A0)); // sortie OC1A déconnectée
}

/* ========================================================================== */
#endif /* _AVRIO_BOARD_MELODY_H_ */
