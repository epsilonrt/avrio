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

/* BLYSS ==================================================================== */
#include <avr/io.h>

enum {
  eEdgeNone,
  eEdgeFalling,
  eEdgeRising
};

/* constants ================================================================ */
#define BLYSS_OUT_PORT PORTB
#define BLYSS_OUT_DDR  DDRB
#define BLYSS_OUT_BIT  0

#define BLYSS_IN_DDR  DDRB
#define BLYSS_IN_BIT  5     // ICP3

/*
 * Partie Timer 16 bits en mode capture
 */
#define BLYSS_IN_TIMER_vect TIMER3_CAPT_vect

// -----------------------------------------------------------------------------
static inline void
vBlyssTimerInit (void) {

  /*
   * Mode normal: WGM = 0000
   * Input Capture Noise Canceler: ICNCn = 1
   * Horloge /64 -> tick 4µs: CS = 010
   */
  TCCR3A = 0x00;
  TCCR3B = 0x82;
}

// -----------------------------------------------------------------------------
static inline void
vBlyssTimerSetEdge (uint8_t edge) {
  
  TCNT3 = 0;
  TIMSK3 &= ~_BV (ICIE3);
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
  TIFR3 |= _BV (ICF3);
  TIMSK3 |= _BV (ICIE3);
}

// -----------------------------------------------------------------------------
static inline uint16_t
usBlyssTimerRead (void) {

  return ICR3 * 4;
}

/* ========================================================================== */
#endif /* _AVRIO_BOARD_BLYSS_H_ */
