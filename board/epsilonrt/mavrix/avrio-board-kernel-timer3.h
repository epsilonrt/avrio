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
#  define AVRIO_KERNEL_TICK_RATE 20UL

/*
 * Vecteur d'interruption utilisé par le modula tâche
 * Voir le paragraphe "Choosing the vector: Interrupt vector names" de la
 * documentation avr-libc :
 * http://www.nongnu.org/avr-libc/user-manual/group__avr__interrupts.html
 */
#  define AVRIO_KERNEL_vect TIMER3_COMPA_vect

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
   * Timer 3 en mode CTC pour générer une it toutes les millisecondes
   * AVRIO_CPU_FREQ = 7.3728 MHz
   * Période de recurrence des it = 50ms soit 368640 périodes d'horloge
   * 368640 / 8 = 46080 donc division par 8 et OCR3A = 46080 - 1 = 46079
   */
  TCCR3A = 0;
  OCR3A = (uint16_t) ((AVRIO_CPU_FREQ / AVRIO_KERNEL_TICK_RATE / 8) - 1);
  TCCR3B = _BV(WGM32) | _BV(CS31);
}

/*
 * Valide l'interruption timer 
 */
static inline void
vKernelIrqEnable (void) {

  ETIFR  |= _BV(OCF3A); /* clear le flag d'it de comparaison */
  ETIMSK |= _BV(OCIE3A); /* valide it comparaison */
}

/*
 * Dévalide l'interruption timer
 */
static inline void
vKernelIrqDisable (void) {

  ETIMSK &= ~_BV(OCIE3A); /* valide it comparaison */
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
