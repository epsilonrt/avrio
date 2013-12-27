/* Copyright © 2009-2012 epsilonRT. All rights reserved.
 *  $Id$ */
#ifndef _AVRIO_BOARD_PHONE_H_
#  define _AVRIO_BOARD_PHONE_H_
/* ========================================================================== */

/* PHONE ==================================================================== */
#  include <avrio/defs.h>
#  include <avr/io.h>

/* constants ================================================================ */
// Flag d'interruption RING
#  define PHONE_RING_IRQ  _BV(INT0)

// Vecteur d'interruption RING
#  define PHONE_RING_VECT INT0_vect

// Flag d'interruption DTMF
#  define PHONE_DTMF_IRQ  _BV(INT1)

// Vecteur d'interruption DTMF
#  define PHONE_DTMF_VECT INT1_vect

// Délai sans commande avant de raccrocher
#  define PHONE_HANGUP_DELAY    SECONDS(35)

// Nombre de sonnerie (par défaut) avant de décrocher (min 4, max 9)
#  define PHONE_HOOKOFF_DELAY    8

// Nombre max de sonnerie permettant d'enclencher le décrochage rapide au
// prochain appel
#  define PHONE_FASTHOOK_RINGS    3

// Délai avant d'enclencher le décrochage rapide après au plus
// PHONE_FASTHOOK_RINGS sonnerie(s)
#  define PHONE_FASTHOOK_DELAY    SECONDS(10)

// Durée d'activation du décrochage rapide (après activation)
#  define PHONE_FASTHOOK_LENGTH  MINUTES(1)

// Taille du buffer de mémorisation de la file de messages
#  define PHONE_BUFSIZE 16

/* inline public functions ================================================== */
#  define RING    PD2
#  define STD     PD3
#  define HOOKOFF PD4

/*
 * Valide une ou plusieurs interruption
 */
static inline void
vPhoneBoardEnable (uint8_t ucIrq) {

  GIFR |= ucIrq;  /* Clear flag */
  GICR |= ucIrq;  /* validation des IT externes... */
}

/*
 * Dévalide une ou plusieurs interruption
 */
static inline void
vPhoneBoardDisable (uint8_t ucIrq) {

  GICR &= ~ucIrq; /* devalidation des IT externes... */
}

/*
 * Initialisation de la partie matérielle
 */
static inline void
vPhoneBoardInit (void) {

  sbi (PORTD, RING);
  cbi (DDRD, RING);

  cbi (PORTD, STD);
  cbi (DDRD, STD);

  cbi (PORTD, HOOKOFF);
  sbi (DDRD, HOOKOFF);

  PORTB &= ~(0x0F);
  DDRB &= ~(0x0F);

  /* DTMF sur front montant */
  MCUCR |= _BV (ISC11) | _BV (ISC10);

  /* Ring sur front descendant */
  sbi (MCUCR, ISC01);
  cbi (MCUCR, ISC00);
}

/*
 * Lecture du code DTMF en sortie du MT8870
 */
static inline uint8_t
ucPhoneBoardGetDtmfCode (void) {

  return PINB & 0x0F;
}

/*
 * Active le relais de décrochage ligne
 */
static inline void
vPhoneBoardSetHookoff (void) {

  sbi (PORTD, HOOKOFF);
}

/*
 * Désactive le relais de décrochage ligne
 */
static inline void
vPhoneBoardClearHookoff (void) {

  cbi (PORTD, HOOKOFF);
}

/* ========================================================================== */
#endif /* _AVRIO_BOARD_PHONE_H_ */
