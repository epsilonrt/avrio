/**
 * @file irq.h
 * @brief Broches d'interruption
 *
 * Copyright © 2015 epsilonRT. All rights reserved.
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
 *  @defgroup irq_module Broches d'interruption
 *
 *  Ce module permet de gérer les broches d'interruption
 *  @{
 *  @example irq/demo_irq.c
 * description
 */

/* constants ================================================================ */
/**
 * @brief Type d'évement de déclenchement
 */
typedef enum {
  eIrqLowLevel = 0, /**< Niveau bas */
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
  void (*func) (int8_t); /**< pointeur vers le gestionnaire */
  void * udata; /**< pointeur sur paramètre utilisateur (optionnel) */
  eIrqMode mode;/**< sens du front de déclenchement */
} xIrqContext;

/* types ==================================================================== */
/** @brief Désigne le numéro d'une interruption */
typedef int8_t xIrqHandle;

/** @brief Type pointeur sur une routine d'interruption utilisateur */
typedef void (*vIrqFunc) (xIrqHandle i);

/* internal public functions ================================================ */
/**
 * @brief Attache une fonction à une broche d'interruption
 *
 * L'interruption correspondante est validée à l'aide de vIrqEnable()
 *
 * @param i numéro de l'interruption (INT0, INT1 ....)
 * @param func fonction gestionnaire de l'interruption
 * @param mode type d'événement déclenchant l'interruption
 */
void vIrqAttach (xIrqHandle i, vIrqFunc func, eIrqMode mode);

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
 * @param i numéro de l'interruption (INT0, INT1 ....)
 * @note Cette fonction est automatiquement appellée par vIrqEnable() avant validation
 */
inline void vIrqClearFlag (xIrqHandle i);

/**
 * @brief Valide ou invalide une interruption
 *
 * @param i numéro de l'interruption (INT0, INT1 ....)
 *
 * @note Cette fonction est automatiquement appellée par vIrqAttach()
 */
inline void vIrqEnable (xIrqHandle i);

/**
 * @brief Dévalide une interruption
 *
 * @param i numéro de l'interruption (INT0, INT1 ....)
 */
inline void vIrqDisable (xIrqHandle i);

/**
 * @brief Renvoie l'état logique de la broche de l'interruption
 *
 * @param i numéro de l'interruption (INT0, INT1 ....)
 * @return true pour l'état haut
 */
inline bool bIrqReadPin (xIrqHandle i);

/**
 * @brief Modifie l'événement déclenchant l'interruption
 *
 * @param i numéro de l'interruption (INT0, INT1 ....)
 * @param mode type d'événement déclenchant l'interruption
 *
 * @note Cette fonction est automatiquement appellée par vIrqAttach()
 */
inline void vIrqSetMode (xIrqHandle i, eIrqMode mode);

/**
 * @brief Renvoie le mode de déclenchement de l'interruption
 *
 * @param i numéro de l'interruption (INT0, INT1 ....)
 * @return le mode de déclenchement
 */
inline eIrqMode eIrqGetMode (xIrqHandle i);

/**
 * @brief Modifie la variable utilisateur de l'interruption
 *
 * @param i numéro de l'interruption (INT0, INT1 ....)
 * @param data pointeur sur la variable utilisateur
 */
inline void vIrqSetUserData (xIrqHandle i, void * data);

/**
 * @brief Renvoie la variable utilisateur de l'interruption
 *
 * @param i numéro de l'interruption (INT0, INT1 ....)
 * @return la variable utilisateur de l'interruption
 */
inline void * pvIrqGetUserData (xIrqHandle i);


/**
 * @brief Déclenche une interruption
 *
 * La broche d'interruption devra être configurée en sortie par vIrqInitPin()
 * sinon elle ne fait rien. La configuration d'une broche d'interruption en
 * sortie est un moyen d'émuler une interruption logicielle.
 *
 * @param i numéro de l'interruption (INT0, INT1 ....)
 */
inline void vIrqRaise (xIrqHandle i);

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

/* public variables ========================================================= */
extern xIrqContext xIrq[];
#include "avrio-board-irq.h"

/* inline public functions ================================================== */

// -----------------------------------------------------------------------------
INLINE void
vIrqSetUserData (xIrqHandle i, void * data) {
  xIrq[i].udata = data;
}

// -----------------------------------------------------------------------------
INLINE void *
pvIrqGetUserData (xIrqHandle i) {
  return xIrq[i].udata;
}

// -----------------------------------------------------------------------------
INLINE eIrqMode
eIrqGetMode (xIrqHandle i) {
  return xIrq[i].mode;
}
#endif

#endif /* __DOXYGEN__ not defined */

/* ========================================================================== */
__END_C_DECLS
#endif /* _AVRIO_IRQ_H_ */
