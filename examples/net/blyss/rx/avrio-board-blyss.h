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
 * Ici, la broche DATA du transmetteur UHF est connecté à PB1 
 * (IO9 sur Arduino UNO)
 */
#define BLYSS_OUT_PORT PORTB
#define BLYSS_OUT_DDR  DDRB
#define BLYSS_OUT_BIT  1

/*
 * Partie réception, utilise un timer 16 bits en mode capture
 * Ici c'est le TIMER1, la broche DATA du récepteur UHF est connecté à ICP1
 * (PB0/IO8 sur Arduino UNO).
 */
#define BLYSS_IN_TIMER_vect TIMER1_CAPT_vect

// -----------------------------------------------------------------------------
static inline void
vBlyssTimerInit (void) {

  /*
   * Mode normal: WGM = 0000
   * Input Capture Noise Canceler: ICNCn = 1
    */
  TCCR1A = 0x00;
  TCCR1B = _BV(ICNC1);
}

// -----------------------------------------------------------------------------
static inline void
vBlyssTimerSetEdge (eBlyssEdge edge) {
  
  TIMSK1 &= ~_BV (ICIE1);
  TCCR1B &= ~(_BV(CS12)|_BV(CS11)|_BV(CS10)); /* Arrêt Horloge, CS = 000 */
  TCNT1 = 0;
  switch (edge) {
    case eEdgeFalling:
      TCCR1B &= ~_BV (ICES1);
      break;
    case eEdgeRising:
      TCCR1B |= _BV (ICES1);
      break;
    default:
      return;
  }
  TIFR1  |= _BV (ICF1); /* Clear IRQ */
  TIMSK1 |= _BV (ICIE1); /* Valide interruption */
  TCCR1B |= _BV(CS11)|_BV(CS10); /* Démarrage Horloge /64 -> tick 4µs: CS = 011 */
}

// -----------------------------------------------------------------------------
static inline uint16_t
usBlyssTimerRead (void) {

  /* Horloge /64 -> tick 4µs */
  return ICR1 * 4;
}

/* ========================================================================== */
#endif /* _AVRIO_BOARD_BLYSS_H_ */
