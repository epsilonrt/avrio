/**
 * Copyright © 2011-2015 epsilonRT. All rights reserved.
 *
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
 *
 * @file bisrelay.h
 * @brief Relais bistables
 *
 * Revision History ------------------------------------------------------------
 *    20140312 - Initial version by epsilonRT
 */
#ifndef _AVRIO_BISRELAY_H_
#define _AVRIO_BISRELAY_H_
#include <avrio/defs.h>

__BEGIN_C_DECLS
/* ========================================================================== */
/**
 * @addtogroup dev_group
 * @{
 *
 *  @defgroup bisrelay_module Relais bistables
 *  Ce module permet de gérer des relais bistables. Les relais bistables sont
 *  des relais qui ne consomment pas de courant à l'état de repos. Ils peuvent
 *  disposer d'une ou deux bobines. \n
    - Dans le cas de deux bobines, une des bobines effectue la mise à l'état On,
 *    une autre, la mise à l'état Off. Une broche commune étant reliée à la
 *    masse ou le + de l'alimentation.
 *  - Dans le cas d'une seule bobine, un fil de la bobine effectue la mise à
 *    l'état On, un autre la mise à l'état Off. Quand un fil est actif, l'autre
 *    doit être inactif.
 *  .
 *  Ce module gère pour l'instant les relais bistable à une bobine. Le nombre
 *  maximal de relais géré est de 8.
 *  @{
 *    @example bisrelay/demo_bisrelay.c
 *    Exemple qui bascule chaque relais à l'état On puis à l'état Off.
 */

/* constants ================================================================ */
#define BISRELAY_ALL      0xFF

/* structures =============================================================== */
/**
 * Relais bistable
 */
typedef struct xBisRelay {
  uint8_t ucOn;   ///< Masque de la broche Set
  uint8_t ucOff;   ///< Masque de la broche Reset
  volatile uint8_t *pPort;  ///< Port des broches du relais
} xBisRelay;

/* internal public functions ================================================ */
/**
 * @brief Initialise le module
 *
 * Tous les relais sont mis à l'arrêt.
 */
void vBisRelayInit (void);

/**
 * @brief Mise en marche d'un ou plusieurs relais
 *
 * @param ucMask masque de bits des relais à modifier. Les relais dont le bit
 * est à '1' sont modifiés, les autres restent en l'état.
 */
void vBisRelaySet (uint8_t ucMask);

/**
 * @brief Arrêt d'un relais d'un ou plusieurs relais
 *
 * @param ucMask masque de bits des relais à modifier. Les relais dont le bit
 * est à '1' sont modifiés, les autres restent en l'état.
 */
void vBisRelayClear (uint8_t ucMask);

/**
 * @brief Basculement d'un ou plusieurs relais
 *
 * @param ucMask masque de bits des relais à modifier. Les relais dont le bit
 * est à '1' sont modifiés, les autres restent en l'état.
 */
void vBisRelayToggle (uint8_t ucMask);

/**
 * @brief Modification de l'état des relais
 *
 * @param ucMask masque de bits des relais, les relais dont le bit est à '1'
 * sont mis en marche, ceux dont le bit est à '0' sont arréter.
 */
void vBisRelaySetAll (uint8_t ucMask);

// -----------------------------------------------------------------------------
#  if  defined(__DOXYGEN__)
/*
 * __DOXYGEN__ defined
 * Partie documentation ne devant pas être compilée.
 * =============================================================================
 */

/**
 * @brief Lecture de l'état des relais
 *
 * @return  masque de bits des relais, les relais dont le bit est à '1'
 * sont mis en marche, ceux dont le bit est à '0' sont arréter.
 */
uint8_t ucBisRelayGetAll (void);

/**
 *   @}
 * @}
 */

#else
/*
 * __DOXYGEN__ not defined
 * Partie ne devant pas être documentée.
 * =============================================================================
 */
#include "avrio-board-bisrelay.h"
extern uint8_t ucBisRelayCurrent;

INLINE uint8_t
ucBisRelayGetAll (void) {

  return ucBisRelayCurrent;
}
#  endif /* __DOXYGEN__ not defined */

/* ========================================================================== */
__END_C_DECLS
#endif  /* _AVRIO_BISRELAY_H_ not defined */
