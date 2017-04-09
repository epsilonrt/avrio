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
#define BISRELAY_QUANTITY   1

/*
 * Temps d'établissement (set/reset) en millisecondes
 */
#define BISRELAY_SET_TIME   500

/*
 * Définition du tableau descriptif des relais
 */
#define BISRELAY_ARRAY { \
  { .pPort = &PORTA, .ucOn = _BV(0), .ucOff = _BV(1) }}

/* inline public functions ================================================== */

// -----------------------------------------------------------------------------
// Initialisation des broches de commande des relais en sortie à zéro
static inline void
vBisRelayHwInit (void) {

  DDRA  |=  0b00000011;
  PORTA &= ~0b00000011;
}

/* ========================================================================== */
#endif /* _AVRIO_BOARD_BISRELAY_H_ */
