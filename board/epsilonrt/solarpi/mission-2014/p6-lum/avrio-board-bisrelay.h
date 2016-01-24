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
// Initialisation des broches de commande des relais en sortie à zéro
static inline void
vBisRelayHwInit (void) {

  DDRD  |= 0b11111100;
  PORTD &= ~0b11111100;
}

/* ========================================================================== */
#endif /* _AVRIO_BOARD_BISRELAY_H_ */
