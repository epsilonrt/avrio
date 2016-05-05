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
#ifndef _AVRIO_BOARD_ENCODER_H_
#  define _AVRIO_BOARD_ENCODER_H_
/* ========================================================================== */

/* ENCODER ================================================================== */
#  include <avr/interrupt.h>
#  include <avrio/encoder.h>

/* constants ================================================================ */
#  define ENCODER_MAX 1 /*< Nombre d'encodeurs */

/*
 * Temps maximal de rebond de l'encodeur en millisecondes
 */
#define ENCODER_DEBOUNCE_TIME 4

/*
 * Période de calcul de la vitesse en ms
 */
//#  define ENCODER_SPEED_PERIOD 100

/*
 * Signaux utilisés par les encodeurs
 */
#  define CODA 0  /* Index dans le tableau des encodeurs */

/* Broche d'interruption */
#  define CODA_IRQ_PORT   PORTD
#  define CODA_IRQ_DDR    DDRD
#  define CODA_IRQ_PIN    PIND
#  define CODA_IRQ_BIT    2
#  define CODA_VECT       INT2_vect

/* Broche de direction */
#  define CODA_DIR_PORT   PORTA
#  define CODA_DIR_DDR    DDRA
#  define CODA_DIR_PIN    PINA
#  define CODA_DIR_BIT    2

/* inline public functions ================================================== */
/*
 * Initialise la ressource matérielle utilisée par le module encodeur
 * A modifier en fonction des signaux utilisés.
 */
static inline void
vEncoderBoardInit (void) {

  CODA_IRQ_DDR  &= ~_BV(CODA_IRQ_BIT);
  CODA_IRQ_PORT &= ~_BV(CODA_IRQ_BIT);
  CODA_DIR_DDR  &= ~_BV(CODA_DIR_BIT);
  CODA_DIR_PORT &= ~_BV(CODA_DIR_BIT);

  /* .... IT sur fronts descendants */
  EICRA |= _BV (ISC21)|_BV (ISC20);
}

/*
 * Validation d'un encodeur
 */
static inline void
vEncoderBoardEnable (uint8_t ucEncoder, bool xEnable) {

  if (xEnable) {

    EIFR |= _BV(INTF2); /* Clear flag */
    EIMSK |= _BV(INT2); /* validation des IT externes... */
  } else {

    EIMSK &= ~_BV(INT2); /* devalidation des IT externes... */
  }
}

/*
 * Lecture de l'état de la broche d'interruption
 * 
 * @param ucEncoder Index de l'encodeur
 * @return true si à l'état actif 
 */
static inline bool 
bEncoderIrqPin (uint8_t ucEncoder) {

  return ((CODA_IRQ_PIN & _BV(CODA_IRQ_BIT)) ? true : false);
}

/*
 * Lecture de l'état de la broche d'interruption
 * 
 * @param ucEncoder Index de l'encodeur
 * @return true si à l'état correnspondant au sens horaire 
 */
static inline bool 
bEncoderDirPin (uint8_t ucEncoder) {

  return ((CODA_DIR_PIN & _BV(CODA_DIR_BIT)) ? false : true);
}

/*
 * Routine d'interruption encodeur A
 */
ISR (CODA_VECT) {

  vEncoderTask (CODA);
}

/* ========================================================================== */
#endif /* _AVRIO_BOARD_ENCODER_H_ */
