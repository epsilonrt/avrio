/* Copyright � 2009-2012 epsilonRT. All rights reserved.
 *  $Id$ */
#ifndef _AVRIO_BOARD_BISRELAY_H_
#define _AVRIO_BOARD_BISRELAY_H_
/* ========================================================================== */

/* REL ====================================================================== */
#include <avrio/defs.h>
#include <avr/io.h>
#include <avr/pgmspace.h>

/* constants ================================================================ */
/*
 * Nombre de relais command�s
 */
#define BISRELAY_QUANTITY   3

/*
 * Temps d'�tablissement (set/reset) en millisecondes
 */
#define BISRELAY_SET_TIME   7

/*
 * D�finition du tableau descriptif des relais
 */
#define BISRELAY_ARRAY { \
  { .pPort = &PORTD, .ucOn = _BV(2), .ucOff = _BV(3) }, \
  { .pPort = &PORTD, .ucOn = _BV(6), .ucOff = _BV(7) }, \
  { .pPort = &PORTD, .ucOn = _BV(4), .ucOff = _BV(5) }}

/* inline public functions ================================================== */

// -----------------------------------------------------------------------------
// Initialisation des broches de commande des relais en sortie � z�ro
static inline void
vBisRelayHwInit (void) {

  DDRD  |= 0b11111100;
  PORTD &= ~0b11111100;
}

/* ========================================================================== */
#endif /* _AVRIO_BOARD_BISRELAY_H_ */
