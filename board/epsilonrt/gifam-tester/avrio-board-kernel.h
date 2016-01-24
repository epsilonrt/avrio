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
#  define AVRIO_KERNEL_vect TIMER0_COMPA_vect

/*
 * Si AVRIO_KERNEL_CRITICAL est défini (défaut), les tâches sont exécutées de
 * façon atomique (ne peuvent être interrompues, même par une IRQ).
 * Dans le cas contraire, les tâches peuvent être interrompues par n'importe
 * quelle interruption.
 */
#  define AVRIO_KERNEL_CRITICAL

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

#ifdef AVRIO_AVRX_ENABLE
  MCUCR = _BV(SE); // Valide le mode sleep idle (AvrX) <TODO>
#endif

  /* 
   * Timer 0 en mode CTC pour générer une it toutes les millisecondes
   * F_CPU = 8.0000 MHz
   * Période de reccurrence des it = 1ms soit 8000 périodes d'horloge
   * 8000 / 64 = 125 donc division par 64 et OCR2 = 125 - 1 = 124
   */
  OCR0A = (uint8_t) ((F_CPU / AVRIO_KERNEL_TICK_RATE / 64) - 1);
  TCCR0A = 0b00000010; /* mode CTC */
  TCCR0B = 0b00000011; /* mode CTC, N = 64 */
}

/*
 * Valide l'interruption timer 
 */
static inline void
vKernelIrqEnable (void) {

  sbi (TIFR0, OCF0A); /* clear le flag d'it de comparaison */
  sbi (TIMSK0, OCIE0A); /* valide it comparaison */
}

/*
 * Dévalide l'interruption timer
 */
static inline void
vKernelIrqDisable (void) {

  cbi (TIMSK0, OCIE0A); /* invalide it comparaison */
}

/*
 * Déclenche une interruption timer dès validation globale des interruptions
 */
static inline void
vKernelIrqGenerate (void) {
/* ------------------------------- TODO ----------------------------------------
  uint8_t ucTCNT =  TCNT0;  // Valeur précédente du compteur
  uint8_t ucTCCR = TCCR0B;  // Valeur précédente du prédiviseur
  
  TCNT0  =     OCR0A; // Compteur au max
  TCCR0B = _BV(CS00); // Prédivision par 1, génération Irq
  TCCR0B =    ucTCCR; // Restauration prédiviseur
  TCNT0  =    ucTCNT; // Restauration compteur
*/
}

#endif /* __ASSEMBLER__ not defined */
/* ========================================================================== */
#endif /* _AVRIO_BOARD_KERNEL_H_ */
