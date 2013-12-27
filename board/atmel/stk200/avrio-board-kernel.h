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
#  define AVRIO_KERNEL_vect TIMER2_COMP_vect

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

  MCUCR = _BV(SE); // Valide le mode sleep idle (AvrX)
  
  /* 
   * Timer 2 en mode CTC pour générer une it toutes les millisecondes
   * AVRIO_CPU_FREQ = 14.7456 MHz
   * Période de reccurrence des it = 250us soit 3686 périodes d'horloge
   * 16000 / 32 = 115 donc division par 32 et OCR2 = 115 - 1 = 114
   */
  TCNT2 = 0;
  OCR2  = (uint8_t) ((AVRIO_CPU_FREQ / AVRIO_KERNEL_TICK_RATE / 32) - 1);
  TCCR2 = 0b00001011;  /* mode CTC, N = 32 */
}

/*
 * Valide l'interruption timer 
 */
static inline void
vKernelIrqEnable (void) {

  sbi (TIFR, OCF2); /* clear le flag d'it de comparaison */
  sbi (TIMSK, OCIE2); /* valide it comparaison */
}

/*
 * Dévalide l'interruption timer
 */
static inline void
vKernelIrqDisable (void) {

  cbi (TIMSK, OCIE2); /* invalide it comparaison */
}

/*
 * Déclenche une interruption timer dès validation globale des interruptions
 */
static inline void
vKernelIrqGenerate (void) {
  uint8_t ucTCNT =  TCNT2;  // Valeur précédente du compteur
  uint8_t ucTCCR =  TCCR2;  // Valeur précédente du prédiviseur
  
  TCNT2  =      0xFF; // Compteur au max
  TCCR2  = _BV(CS20); // Prédivision par 1, génération Irq
  TCCR2  =    ucTCCR; // Restauration prédiviseur
  TCNT2  =    ucTCNT; // Restauration compteur
}

#endif /* __ASSEMBLER__ not defined */
/* ========================================================================== */
#endif /* _AVRIO_BOARD_KERNEL_H_ */
