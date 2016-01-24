/*
 * This file is part of AvrIO.
 *
 * AvrIO is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * AvrIO is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with AvrIO.  If not, see <http://www.gnu.org/licenses/lgpl.html>
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
