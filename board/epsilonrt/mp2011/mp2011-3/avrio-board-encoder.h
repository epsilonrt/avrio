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
#  define ENCODER_MAX 2
                      /*< Nombre d'encodeurs */

/*
 * Période de calcul de la vitesse en ms
 */
#  define ENCODER_SPEED_PERIOD 100

/*
 * Signaux utilisés par les encodeurs
 */
#  define CODA  0
#  define CODA_IRQ_PORT   PORTD
#  define CODA_IRQ_DDR    DDRD
#  define CODA_IRQ_PIN    PIND
#  define CODA_IRQ_BIT        2
#  define CODA_VECT       INT0_vect
#  define CODA_DIR_PORT   PORTC
#  define CODA_DIR_DDR    DDRC
#  define CODA_DIR_PIN    PINC
#  define CODA_DIR_BIT    0

#  define CODB  1
#  define CODB_IRQ_PORT   PORTD
#  define CODB_IRQ_DDR    DDRD
#  define CODB_IRQ_PIN    PIND
#  define CODB_IRQ_BIT        3
#  define CODB_VECT       INT1_vect
#  define CODB_DIR_PORT   PORTC
#  define CODB_DIR_DDR    DDRC
#  define CODB_DIR_PIN    PINC
#  define CODB_DIR_BIT    1

/* inline public functions ================================================== */
/*
 * Initialise la ressource matérielle utilisée par le module encodeur
 * A modifier en fonction des signaux utilisés.
 */
static inline void
vEncoderBoardInit (void) {

  cbi (CODA_IRQ_DDR, CODA_IRQ_BIT);
  cbi (CODA_IRQ_PORT, CODA_IRQ_BIT);
  cbi (CODA_DIR_DDR, CODA_DIR_BIT);
  cbi (CODA_DIR_PORT, CODA_DIR_BIT);

  cbi (CODB_IRQ_DDR, CODB_IRQ_BIT);
  cbi (CODB_IRQ_PORT, CODB_IRQ_BIT);
  cbi (CODB_DIR_DDR, CODB_DIR_BIT);
  cbi (CODB_DIR_PORT, CODB_DIR_BIT);

  /* .... IT sur fronts montants */
  MCUCR |= _BV (ISC11) | _BV (ISC10) | _BV (ISC01) | _BV (ISC00);
}

/*
 * Validation d'un encodeur
 */
static inline void
vEncoderBoardEnable (uint8_t ucEncoder, bool xEnable) {
  uint8_t ucBitIndex = INT0 + ucEncoder;

  if (xEnable) {

    sbi (GIFR, ucBitIndex); /* Clear flag */
    sbi (GICR, ucBitIndex); /* validation des IT externes... */
  } else {

    cbi (GICR, ucBitIndex); /* devalidation des IT externes... */
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
  if (ucEncoder)
    return bit_is_set (CODB_IRQ_PIN, CODB_IRQ_BIT);
  else
    return bit_is_set (CODA_IRQ_PIN, CODA_IRQ_BIT);
}

/*
 * Lecture de l'état de la broche d'interruption
 * 
 * @param ucEncoder Index de l'encodeur
 * @return true si à l'état correnspondant au sens horaire 
 */
static inline bool 
bEncoderDirPin (uint8_t ucEncoder) {

  if (ucEncoder)
    return bit_is_set (CODB_DIR_PIN, CODB_DIR_BIT);
  else
    return bit_is_set (CODA_DIR_PIN, CODA_DIR_BIT);
}

/*
 * Routine d'interruption encodeur A
 */
ISR (CODA_VECT) {

  vEncoderTask (CODA);
}

/*
 * Routine d'interruption encodeur B
 */
ISR (CODB_VECT) {

  vEncoderTask (CODB);
}

/* ========================================================================== */
#endif /* _AVRIO_BOARD_ENCODER_H_ */
