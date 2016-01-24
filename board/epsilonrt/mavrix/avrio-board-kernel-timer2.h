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
#ifndef _AVRIO_BOARD_KERNEL_H_
#  define _AVRIO_BOARD_KERNEL_H_
/* ========================================================================== */

/* KERNEL =================================================================== */
#  include <avrio/defs.h>
#  include <avr/io.h>

/* constants ================================================================ */
/*
 * Fréquence de récurrence de la routine d'interruption da tâche
 * La durée d'un tick vaut 1/AVRIO_KERNEL_TICK_RATE
 */
#  define AVRIO_KERNEL_TICK_RATE 1000UL

/*
 * Vecteur d'interruption utilisé par le modula tâche
 * Voir le paragraphe "Choosing the vector: Interrupt vector names" de la
 * documentation avr-libc :
 * http://www.nongnu.org/avr-libc/user-manual/group__avr__interrupts.html
 */
#  define AVRIO_KERNEL_vect TIMER2_COMP_vect

/*
 * Si AVRIO_KERNEL_CRITICAL est défini (défaut), le noyau est exécuté de
 * façon atomique (ne peut être interrompu, même par une IRQ).
 * Dans le cas contraire, le noyau peut être interrompu par n'importe
 * quelle interruption.
 */
#  define AVRIO_KERNEL_CRITICAL

/*
 * Valide l'utilisation des mutex pour sécuriser les accès concurents aux pointeurs
 * des piles xQueue
 */
// #define AVRIO_QUEUE_LOCKED_ENABLE

/*
 * Valide le mode pas à pas dans AvrX
 */
//#  define AVRX_SINGLESTEP_ENABLE

/*
 * Valide la fonction vAvrXTaskExit()
 */
//#  define AVRX_TASKEXIT_ENABLE

#  ifndef __ASSEMBLER__
/* inline public functions ================================================== */

/*
 * Initialise le timer matériel qui génère une interruption périodique et
 * exécute AVRIO_KERNEL_vect.
 * La fréquence de récurrence est AVRIO_KERNEL_TICK_RATE.
 */
static inline void
vKernelHardwareInit (void) {

  MCUCR = _BV(SE); // Valide le mode sleep idle (AvrX) <TODO>

  /* 
   * Timer 2 en mode CTC pour générer une it toutes les millisecondes
   * AVRIO_CPU_FREQ = 7.3728 MHz
   * Période de recurrence des it = 1ms soit 368640 périodes d'horloge
   * 7372.8 / 64 = 115.2 donc division par 64 et OCR2 = 115 - 1 = 114
   */
  OCR2 = (uint8_t) ((AVRIO_CPU_FREQ / AVRIO_KERNEL_TICK_RATE / 64) - 1);
  TCCR3B = _BV(WGM21) | _BV(CS21) | _BV(CS20);
}

/*
 * Valide l'interruption timer 
 */
static inline void
vKernelIrqEnable (void) {

  TIFR  |= _BV(OCF2); /* clear le flag d'it de comparaison */
  TIMSK |= _BV(OCIE2); /* valide it comparaison */
}

/*
 * Dévalide l'interruption timer
 */
static inline void
vKernelIrqDisable (void) {

  TIMSK &= ~_BV(OCIE2); /* valide it comparaison */
}

/*
 * Déclenche une interruption timer dès validation globale des interruptions
 */
static inline void
vKernelIrqGenerate (void) {

}

#endif /* __ASSEMBLER__ not defined */
/* ========================================================================== */
#endif /* _AVRIO_BOARD_KERNEL_H_ */
