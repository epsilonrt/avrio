/*
 * @file avrio-board-melody.h
 * @brief Configuration module Melody
 * - Mcu      : AT90CAN128
 * - Arch     : avr5
 * - Timer    : Timer1
 * - Type T   : Timer 16 bits
 * - Modes    : PWM Phase et fréquence correcte ICR (8)
 * - Sorties  : OC1C
 * .
 * @author Copyright © 2011-2012 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * @version $Id$
 * Revision History ---
 *    20110917 - Initial version by epsilonRT
 */
#ifndef _AVRIO_BOARD_MELODY_H_
#  define _AVRIO_BOARD_MELODY_H_
/* ========================================================================== */

/* MELODY =================================================================== */
#  include <avrio/melody.h>
#  include <avr/io.h>

#  ifndef AVRIO_KTIMER_ENABLE
#    warning "Le module ktimer doit être validé pour utiliser le module melody."
#  endif

/* constants ================================================================ */
/*
 * @brief Indique que le timer est utilisé en mode PWM pour d'autres fonctions
 * @warning vMelodyHardwareTopChangedCB() doit alors être définie
 */
// #define AVRIO_MELODY_TIMERCB_ENABLE

/*
 * @brief Rapport de division de l'horloge du timer
 */
#define MELODY_TIMER_CLK TIMER16_CLK8

/*
 * @brief Mode du timer
 */
#define MELODY_TIMER_MOD (8)

/* macros =================================================================== */
/*
 * @brief Sélection division fréquence du Timer
 */
#define MELODY_SET_CS(__N) ( ((MELODY_TIMER_MOD & 0x0C) << 1) | (__N) )

/* inline public functions ================================================== */
/*
 * Initialise la ressource matérielle (timer) utilisée par le module melody
 * A modifier en fonction du timer et de la sortie utilisée.
 */
static inline void
vMelodyHardwareInit (void) {

  DDRB |= _BV (7);  // PB7 en sortie (OC1C)
  TCCR1A = MELODY_TIMER_MOD & 0x03;
  TCCR1B = MELODY_SET_CS (MELODY_TIMER_CLK);
}

/*
 * Génère un son de fréquence \c usFreq
 * A modifier en fonction du timer et de la sortie utilisée.
 * \param usFreq Fréquence du son en Hz
 */
static void
vMelodyHardwarePlay (uint16_t usFreq) {
  uint16_t usTop;

  usTop = AVRIO_CPU_FREQ / (2 * 8 * usFreq);
#ifdef AVRIO_MELODY_TIMERCB_ENABLE
  TCCR1B = MELODY_SET_CS (TIMER16_STOPPED);
    vMelodyTopChangedCB (usTop);
  TCCR1B = MELODY_SET_CS (MELODY_TIMER_CLK);
#endif
  ICR1  = usTop;
  OCR1C = usTop / 2;
  TCCR1A |= _BV (COM1C1); // sortie OC1C validée (toggle)
}

/*
 * Arrête la génération de son
 * A modifier en fonction du timer et de la sortie utilisée.
 */
static inline void
vMelodyHardwareStop (void) {

  TCCR1A &= ~(_BV (COM1C1) | _BV (COM1C0)); // sortie OC1C déconnectée
}

/* ========================================================================== */
#endif /* _AVRIO_BOARD_MELODY_H_ */
