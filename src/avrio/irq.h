/**
 * @file int.h
 * @brief Broches d'interruption
 *
 * Copyright © 2011-2015 Pascal JEAN aka epsilonRT. All rights reserved.
 *
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
#ifndef _AVRIO_IRQ_H_
#define _AVRIO_IRQ_H_
#include <avrio/defs.h>
#include <avr/interrupt.h>

__BEGIN_C_DECLS
/* ========================================================================== */

/**
 * @addtogroup dev_group
 * @{
 *
 *  @defgroup int_module Broches d'interruption
 *
 *  Ce module permet de gérer la communication sur une liaison série asynchrone
 *  à l'aide d'une borche d'interruption externe et d'un timer 8 bits.
 *  La transmission s'effectue sur un fil de port quelconque mais la réception
 *  doit se faire sur une broche d'interruption (INTX) sur front descendant
 *  Il n'est pas possible de faire du FULL DUPLEX !
 *  @{
 *  @example kirq/demo_kirq.c
 *  Renvoie chaque caractère reçu sur la liaison série en basculant la LED1.
 */

/* constants ================================================================ */
/**
 * @brief Type d'évement de déclenchement
 */
typedef enum {
  eIrqLow      = 0, /**< Niveau bas */
  eIrqEdge     = 1, /**< Front montant ou descendant*/
  eIrqFalling  = 2, /**< Front descendant*/
  eIrqRising   = 3, /**< Front montant*/
} eIrqMode;

/* structures =============================================================== */
/**
 * @struct xIrqContext
 * @brief Structure décrivant une interruption externe
 */
typedef struct xIrqContext {
  void (*func) (struct xIrqContext *); /**< pointeur vers le gestionnaire */
  void * udata; /**< pointeur sur paramètre utilisateur (optionnel) */
  uint8_t chan; /**< numéro du canal INTn */
  eIrqMode mode;/**< sens du front de déclenchement */
} xIrqContext;

/* types ==================================================================== */
/** @brief Type pointeur sur une routine d'interruption utilisateur */
typedef void (*vIrqFunc) (xIrqContext *);

/* internal public functions ================================================ */
/**
 * @brief Attache une fonction à une broche d'interruption
 *
 * L'interruption correspondante est validée à l'aide de vIrqEnable()
 *
 * @param i numéro de l'interruption (0=INT0, 1=INT1 ....)
 * @param func fonction gestionnaire de l'interruption
 * @param mode type d'événement déclenchant l'interruption
 */
void vIrqAttach (uint8_t i, vIrqFunc func, eIrqMode mode);

// -----------------------------------------------------------------------------

#if defined(__DOXYGEN__)
/*
 * __DOXYGEN__ defined
 * Partie documentation ne devant pas être compilée.
 * =============================================================================
 */

/**
 * @brief Efface le drapeau de l'interruption
 *
 * @param i numéro de l'interruption (0=INT0, 1=INT1 ....)
 * @note Cette fonction est automatiquement appellée par vIrqEnable() avant validation
 */
inline void vIrqClearFlag (uint8_t i);

/**
 * @brief Valide ou invalide une interruption
 *
 * @param i numéro de l'interruption (0=INT0, 1=INT1 ....)
 *
 * @note Cette fonction est automatiquement appellée par vIrqAttach()
 */
inline void vIrqEnable (uint8_t i);

/**
 * @brief Dévalide une interruption
 *
 * @param i numéro de l'interruption (0=INT0, 1=INT1 ....)
 */
inline void vIrqDisable (uint8_t i);

/**
 * @brief Modifie l'événement déclenchant l'interruption
 *
 * @param i numéro de l'interruption (0=INT0, 1=INT1 ....)
 * @param mode type d'événement déclenchant l'interruption
 */
inline void vIrqSetMode (uint8_t i, eIrqMode mode);

/**
 * @brief Déclenche une interruption
 *
 * La broche d'interruption devra être configurée en sortie par vIrqInitPin()
 * sinon elle ne fait rien. La configuration d'une broche d'interruption en
 * sortie est un moyen d'émuler une interruption logicielle.
 *
 * @param i numéro de l'interruption (0=INT0, 1=INT1 ....)
 */
inline void vIrqRaise (uint8_t i);

/**
 * @brief Renvoie l'état logique de la broche de l'interruption
 *
 * @param i numéro de l'interruption (0=INT0, 1=INT1 ....)
 * @return true pour l'état haut
 */
inline bool bIrqReadPin (uint8_t i);

/**
 * @brief Modifie la variable utilisateur stockée dans le descripteur.
 * @param i numéro de l'interruption (0=INT0, 1=INT1 ....)
 * @return la variable utilisateur à stocker dans le descripteur
 */
inline void vIrqSetUserData (uint8_t i, void * data);

/**
 * @brief Renvoie la variable utilisateur stockée dans le descripteur
 * @param i numéro de l'interruption (0=INT0, 1=INT1 ....)
 * @return la variable utilisateur stockée dans le descripteur
 */
inline void * pvIrqUserData (uint8_t i);

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
#include "avrio-config.h"
#ifdef AVRIO_IRQ_ENABLE
#include "avrio-board-irq.h"

/* public variables ========================================================= */
extern xIrqContext xIrq[];

/* inline public functions ================================================== */

// -----------------------------------------------------------------------------
INLINE void
vIrqSetUserData (uint8_t i, void * data) {
  xIrq[i].udata = data;
}

// -----------------------------------------------------------------------------
INLINE void *
pvIrqUserData (uint8_t i) {
  return xIrq[i].udata;
}
#endif

#endif /* __DOXYGEN__ not defined */

/* ========================================================================== */
__END_C_DECLS
#endif /* _AVRIO_IRQ_H_ */
