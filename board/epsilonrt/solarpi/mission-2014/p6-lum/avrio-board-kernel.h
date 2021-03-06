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
#ifndef _AVRIO_BOARD_KERNEL_H_
#  define _AVRIO_BOARD_KERNEL_H_
/* ========================================================================== */

/* KERNEL =================================================================== */
#  include <avrio/defs.h>
#  include <avr/io.h>

/* constants ================================================================ */
/*
 * Fréquence de récurrence de la routine d'interruption da téche
 * La durée d'un tick vaut 1/AVRIO_KERNEL_TICK_RATE
 */
#  define AVRIO_KERNEL_TICK_RATE 10000UL

/*
 * Vecteur d'interruption utilisé par le modula téche
 * Voir le paragraphe "Choosing the vector: Interrupt vector names" de la
 * documentation avr-libc :
 * http://www.nongnu.org/avr-libc/user-manual/group__avr__interrupts.html
 */
#  define AVRIO_KERNEL_vect TIMER2_COMPA_vect

/*
 * Si AVRIO_KERNEL_CRITICAL est défini (défaut), les téches sont exécutées de
 * faéon atomique (ne peuvent étre interrompues, méme par une IRQ).
 * Dans le cas contraire, les téches peuvent étre interrompues par n'importe
 * quelle interruption.
 */
#  define AVRIO_KERNEL_CRITICAL

/*
 * Valide le mode pas é pas dans AvrX
 */
//#  define AVRX_SINGLESTEP_ENABLE

/*
 * Valide la fonction vAvrXTaskExit()
 */
//#  define AVRX_TASKEXIT_ENABLE

#  ifndef __ASSEMBLER__
/* inline public functions ================================================== */

/*
 * Initialise le timer matériel qui génére une interruption périodique et
 * exécute AVRIO_KERNEL_vect.
 * La fréquence de récurrence est AVRIO_KERNEL_TICK_RATE.
 */
static inline void
vKernelHardwareInit (void) {

  MCUCR = _BV(SE); // Valide le mode sleep idle (AvrX) <TODO>

  /* 
   * Timer 2 en mode CTC pour générer une it toutes les millisecondes
   * F_CPU = 7.3728 MHz
   * Période de reccurrence des it = 1ms soit 7373 périodes d'horloge
   * 7373 / 32 = 230 donc division par 32 et OCR2 = 230 - 1 = 229
   */
  OCR2A = (uint8_t) ((F_CPU / AVRIO_KERNEL_TICK_RATE / 32) - 1);
  TCCR2A = 0b00000010; /* mode CTC */
  TCCR2B = 0b00000011; /* mode CTC, N = 32 */
}

/*
 * Valide l'interruption timer 
 */
static inline void
vKernelIrqEnable (void) {

  sbi (TIFR2, OCF2A); /* clear le flag d'it de comparaison */
  sbi (TIMSK2, OCIE2A); /* valide it comparaison */
}

/*
 * Dévalide l'interruption timer
 */
static inline void
vKernelIrqDisable (void) {

  cbi (TIMSK2, OCIE2A); /* invalide it comparaison */
}

/*
 * Déclenche une interruption timer dés validation globale des interruptions
 */
static inline void
vKernelIrqGenerate (void) {
/* ------------------------------- TODO ----------------------------------------
  uint8_t ucTCNT =  TCNT2;  // Valeur précédente du compteur
  uint8_t ucTCCR = TCCR2B;  // Valeur précédente du prédiviseur
  
  TCNT2  =     OCR2A; // Compteur au max
  TCCR2B = _BV(CS20); // Prédivision par 1, génération Irq
  TCCR2B =    ucTCCR; // Restauration prédiviseur
  TCNT2  =    ucTCNT; // Restauration compteur
*/
}

#endif /* __ASSEMBLER__ not defined */
/* ========================================================================== */
#endif /* _AVRIO_BOARD_KERNEL_H_ */
