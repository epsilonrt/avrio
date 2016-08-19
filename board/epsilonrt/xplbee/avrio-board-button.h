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
#ifndef _AVRIO_BOARD_BUTTON_H_
#define _AVRIO_BOARD_BUTTON_H_
/* ========================================================================== */

/* BUTTON==================================================================== */
#include <avrio/defs.h>
#include <avr/io.h>

/* constants ================================================================ */
#define BUTTON_QUANTITY  7
// PORTA BUTTONS
#define BUTTON_NAVC _BV(0)  // BUTTON2 (PA0/PCINT0)
#define BUTTON_NAVW _BV(1)  // BUTTON3 (PA1/PCINT1)
#define BUTTON_NAVN _BV(2)  // BUTTON4 (PA2/PCINT2)
#define BUTTON_NAVE _BV(3)  // BUTTON5 (PA3/PCINT3)
#define BUTTON_NAVS _BV(4)  // BUTTON6 (PA4/PCINT4)
#define BUTTON_DBG  _BV(6)  // BUTTON0 (PA5/PCINT6)
// PORTB BUTTONS
#define BUTTON_INIT (_BV(2)<<5)  // BUTTON1 (PB2/PCINT10/INT2), _BV(2)<<5 = _BV(7)
#define BUTTON_ALL_PORTA   (  BUTTON_NAVC | BUTTON_NAVN | \
                              BUTTON_NAVE | BUTTON_NAVS | \
                              BUTTON_NAVW | BUTTON_DBG)
#define BUTTON_ALL_PORTB   (  BUTTON_INIT>>5 )
#define BUTTON_ALL_BUTTONS (  BUTTON_ALL_PORTA | BUTTON_INIT )
#define BUTTON_NO_BUTTON (0)

#define BUTTON_BUTTON0 BUTTON_DBG
#define BUTTON_BUTTON1 BUTTON_INIT
#define BUTTON_BUTTON2 BUTTON_NAVC
#define BUTTON_BUTTON3 BUTTON_NAVW
#define BUTTON_BUTTON4 BUTTON_NAVN
#define BUTTON_BUTTON5 BUTTON_NAVE
#define BUTTON_BUTTON6 BUTTON_NAVS

/* types ==================================================================== */
typedef uint8_t xButMask;

/* inline public functions ================================================== */
// -----------------------------------------------------------------------------
static inline void
vButHardwareInit (void) {

  PORTA |=  BUTTON_ALL_PORTA;
  DDRA  &= ~BUTTON_ALL_PORTA;
  PORTB |=  BUTTON_ALL_PORTB;
  DDRB  &= ~BUTTON_ALL_PORTB;
}

// -----------------------------------------------------------------------------
static inline xButMask
xButHardwareGet (xButMask xMask) {

  return ((PINA ^ BUTTON_ALL_PORTA) & (xMask & BUTTON_ALL_PORTA)) | 
          (((PINB ^ BUTTON_ALL_PORTB) << 5) & (xMask & BUTTON_INIT)) ;
}

/* public variables ========================================================= */

#if defined(BUTTON_MASK_ARRAY_ENABLE)
#define DECLARE_BUTTON_MASK_ARRAY  \
  const xButMask \
    xButMaskArray [BUTTON_QUANTITY] = { \
      BUTTON_DBG,  \
      BUTTON_INIT, \
      BUTTON_NAVC, \
      BUTTON_NAVW, \
      BUTTON_NAVN, \
      BUTTON_NAVE, \
      BUTTON_NAVS  \
    }
#else
#define DECLARE_BUTTON_MASK_ARRAY
#endif

/* ========================================================================== */
#endif /* _AVRIO_BOARD_BUTTON_H_ */
