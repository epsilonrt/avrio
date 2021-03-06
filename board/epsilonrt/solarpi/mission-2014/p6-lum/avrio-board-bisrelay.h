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
#ifndef _AVRIO_BOARD_BISRELAY_H_
#define _AVRIO_BOARD_BISRELAY_H_
/* ========================================================================== */

/* REL ====================================================================== */
#include <avrio/defs.h>
#include <avr/io.h>
#include <avr/pgmspace.h>

/* constants ================================================================ */
/*
 * Nombre de relais commandés
 */
#define BISRELAY_QUANTITY   3

/*
 * Temps d'établissement (set/reset) en millisecondes
 */
#define BISRELAY_SET_TIME   7

/*
 * Définition du tableau descriptif des relais
 */
#define BISRELAY_ARRAY { \
  { .pPort = &PORTD, .ucOn = _BV(2), .ucOff = _BV(3) }, \
  { .pPort = &PORTD, .ucOn = _BV(6), .ucOff = _BV(7) }, \
  { .pPort = &PORTD, .ucOn = _BV(4), .ucOff = _BV(5) }}

/* inline public functions ================================================== */

// -----------------------------------------------------------------------------
// Initialisation des broches de commande des relais en sortie é zéro
static inline void
vBisRelayHwInit (void) {

  DDRD  |= 0b11111100;
  PORTD &= ~0b11111100;
}

/* ========================================================================== */
#endif /* _AVRIO_BOARD_BISRELAY_H_ */
