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
#  define AVRIO_KERNEL_vect TIMER0_COMP_vect

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
   * AVRIO_CPU_FREQ = 14.7456 MHz
   * Période de reccurrence des it = 1ms soit 14746 périodes d'horloge
   * 14746 / 64 = 230 donc division par 64 et OCR0 = 230 - 1 = 229
   */
  OCR0 = (uint8_t) ((AVRIO_CPU_FREQ / AVRIO_KERNEL_TICK_RATE / 64) - 1);
  TCCR0 = 0b00001100;  /* mode CTC, N = 64 */
}

/*
 * Valide l'interruption timer 
 */
static inline void
vKernelIrqEnable (void) {

  sbi (TIFR, OCF0); /* clear le flag d'it de comparaison */
  sbi (TIMSK, OCIE0); /* valide it comparaison */
}

/*
 * Dévalide l'interruption timer
 */
static inline void
vKernelIrqDisable (void) {

  cbi (TIMSK, OCIE0); /* invalide it comparaison */
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
