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
#  define AVRIO_KERNEL_TICK_RATE 100UL

/*
 * Vecteur d'interruption utilisé par le modula tâche
 * Voir le paragraphe "Choosing the vector: Interrupt vector names" de la
 * documentation avr-libc :
 * http://www.nongnu.org/avr-libc/user-manual/group__avr__interrupts.html
 */
#  define AVRIO_KERNEL_vect TIMER1_COMPA_vect

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

  MCUCR = _BV(SE); // Valide le mode sleep idle (AvrX) <TODO>

  /*
   * Timer 1 en mode 4 CTC pour générer une it toutes les 10 millisecondes
   * F_CPU = 8 MHz
   * Période de reccurrence des it = 10ms soit 80000 périodes d'horloge
   * 80000 / 8 = 10000 donc division par 8 et OCR1A = 10000 - 1 = 9999
   */
  OCR1A = (uint16_t) ((F_CPU / AVRIO_KERNEL_TICK_RATE / 8) - 1);
  TCCR1B = 0b00001010; /* mode 4 CTC, N = 8 */
}

/*
 * Valide l'interruption timer
 */
static inline void
vKernelIrqEnable (void) {

  sbi (TIFR1, OCF1A); /* clear le flag d'it de comparaison */
  sbi (TIMSK1, OCIE1A); /* valide it comparaison */
}

/*
 * Dévalide l'interruption timer
 */
static inline void
vKernelIrqDisable (void) {

  cbi (TIMSK1, OCIE1A); /* invalide it comparaison */
}

/*
 * Déclenche une interruption timer dès validation globale des interruptions
 */
static inline void
vKernelIrqGenerate (void) {
/* ------------------------------- TODO ----------------------------------------
  uint8_t ucTCNT =  TCNT1;  // Valeur précédente du compteur
  uint8_t ucTCCR = TCCR1B;  // Valeur précédente du prédiviseur

  TCNT1  =     OCR1A; // Compteur au max
  TCCR1B = _BV(CS20); // Prédivision par 1, génération Irq
  TCCR1B =    ucTCCR; // Restauration prédiviseur
  TCNT1  =    ucTCNT; // Restauration compteur
*/
}

#endif /* __ASSEMBLER__ not defined */
/* ========================================================================== */
#endif /* _AVRIO_BOARD_KERNEL_H_ */
