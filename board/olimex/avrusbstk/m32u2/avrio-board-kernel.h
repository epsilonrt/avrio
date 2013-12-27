/* Copyright © 2009-2012 epsilonRT. All rights reserved.
 *  $Id$ */
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

  MCUCR = _BV(SE); // Valide le mode sleep idle (AvrX) <TODO>

  /* 
   * Timer 0 en mode CTC pour générer une it toutes les millisecondes
   * AVRIO_CPU_FREQ = 8 MHz
   * Période de reccurrence des it = 1 ms soit 8000 périodes d'horloge
   * 8000 / 64 = 125 donc division par 64 et OCR0 = 125 - 1 = 124
   */
  OCR0A = (uint8_t) (AVRIO_CPU_FREQ / AVRIO_KERNEL_TICK_RATE / 64) - 1;
  TCCR0A = 0x02;  /* mode CTC */
  TCCR0B = 0x03;  /* N = 64 */
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
