/*
 * Copyright © 2016 epsilonRT, All rights reserved.
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
#ifndef _AVRIO_BOARD_BLYSS_H_
#define _AVRIO_BOARD_BLYSS_H_
/* ========================================================================== */
#include <avr/io.h>

/* BLYSS ==================================================================== */

/* types ==================================================================== */
typedef enum {
  eEdgeNone,
  eEdgeFalling,
  eEdgeRising
} eBlyssEdge;

/* configuration ============================================================ */
/*
 * Partie transmission, une broche binaire est nécessaire
 * Ici, la broche DATA du transmetteur UHF est connecté à PB0.
 */
#define BLYSS_OUT_PORT PORTB
#define BLYSS_OUT_DDR  DDRB
#define BLYSS_OUT_BIT  0

/*
 * Partie réception, utilise un timer 16 bits en mode capture
 * Ici c'est le TIMER3, la broche DATA du récepteur UHF est connecté à ICP3 (PB5).
 */
#define BLYSS_IN_TIMER_vect TIMER3_CAPT_vect

// -----------------------------------------------------------------------------
static inline void
vBlyssTimerInit (void) {

  /*
   * Mode normal: WGM = 0000
   * Input Capture Noise Canceler: ICNCn = 1
    */
  TCCR3A = 0x00;
  TCCR3B = _BV(ICNC3);
}

// -----------------------------------------------------------------------------
static inline void
vBlyssTimerSetEdge (eBlyssEdge edge) {
  
  TIMSK3 &= ~_BV (ICIE3);
  TCCR3B &= ~(_BV(CS32)|_BV(CS31)|_BV(CS30)); /* Arrêt Horloge, CS = 000 */
  TCNT3 = 0;
  switch (edge) {
    case eEdgeFalling:
      TCCR3B &= ~_BV (ICES3);
      break;
    case eEdgeRising:
      TCCR3B |= _BV (ICES3);
      break;
    default:
      return;
  }
  TIFR3  |= _BV (ICF3); /* Clear IRQ */
  TIMSK3 |= _BV (ICIE3); /* Valide interruption */
  TCCR3B |= _BV(CS31)|_BV(CS30); /* Démarrage Horloge /64 -> tick 4µs: CS = 011 */
}

// -----------------------------------------------------------------------------
static inline uint16_t
usBlyssTimerRead (void) {

  /* Horloge /64 -> tick 4µs */
  return ICR3 * 4;
}

/* ========================================================================== */
#endif /* _AVRIO_BOARD_BLYSS_H_ */
