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
 * @file dpin.h
 * @brief Broche numérique
 */
#ifndef _AVRIO_DPIN_H_
#define _AVRIO_DPIN_H_

#include <avrio/defs.h>
__BEGIN_C_DECLS
/* ========================================================================== */
#include <avr/io.h>

/**
 * @addtogroup dev_group
 * @{
 *
 *  @defgroup dpin_module Broche numérique
 *  Ce module permet de gérer les broches numériques (binaires). Ce module est
 *  compatible avec la plupart des microcontroleurs y compris ceux disposant
 *  de registres PUE pour la validation des résistances de pull-up
 *  (AtTiny20/40/441/841/1634).
 *  @{
 *  @example dpin/demo_dpin.c
 */

/* constants ================================================================ */
/**
 * @enum eDpMode
 * @brief Type de broche
 */
typedef enum {

  eModeInput = 0, /**< Entrée sans résistance de pullup, active à l'état haut */
  eModeOutput = 1, /**< Sortie active à l'état haut (état bas à l'initialisation) */
  eModeInputPullUp = 2, /**< Entrée avec résistance de pullup, active à l'état bas */
  eModeOutputHigh = 3, /**< Sortie active à l'état bas (état haut à l'initialisation) */
  eModeUnknown = -1
} eDpMode;

/**
 * @enum eDpEdge
 * @brief Front d'entrée
 */
typedef enum {
  eEdgeNone = 0,                          /**< aucun front */
  eEdgeFalling = 1,                       /**< front descendant */
  eEdgeRising  = 2,                       /**< front montant */
  eEdgeBoth = eEdgeFalling + eEdgeRising, /**< front descendant et montant */
} eDpEdge;

/* structures =============================================================== */
/**
 * @brief Broche numérique
 */
struct xDPin {
  volatile uint8_t * port; /**< Adresse du registre PORT */
#if defined(__AVR_ATtiny441__ ) || defined(__AVR_ATtiny841__)
  volatile uint8_t * pue; /**< Adresse du registre PUE */
#define DPIN_PUE 1
#define DPIN_PUE_FIELD 1
#endif
  struct {

    uint8_t pin:  3; /**< Index de la broche (0 à 7) */
    int8_t mode: 3; /**< Type de broche eDpMode (0 à 3) */
    uint8_t edge: 2; /**< Front de déclenchement eDpEdge du callback (0 à 3) */
  };
};
typedef struct xDPin xDPin;

/* internal public functions ================================================ */
/**
 * @brief Modifie le mode de la broche
 * @param p pointeur sur la broche
 * @param eMode mode
 */
void vDpSetMode (xDPin * p, eDpMode eMode);

/**
 * @brief Modifie l'état de la sortie ou la résistance de pullup pour une entrée
 * @param p pointeur sur la broche
 * @param bValue valeur
 */
void vDpWrite (xDPin * p, bool bValue);

/**
 * @brief Bascule l'état de la sortie ou la résistance de pullup pour une entrée
 * @param p pointeur sur la broche
 */
void vDpToggle (xDPin * p);

/**
 * @brief Lecture de l'état d'une broche (entrée ou sortie)
 *
 * @param p pointeur sur la broche
 * @return false ou true, si le mode de la broche est eModeInputPullUp ou
 * eModeOutputHigh la valeur booléenne renvoyée est l'inverse de l'état électrique.
 */
bool bDpRead (const xDPin * p);

#  if defined(__DOXYGEN__)
/*
 * __DOXYGEN__ defined
 * Partie documentation ne devant pas être compilée.
 * =============================================================================
 */
/**
 * @brief Initialisation d'une broche
 * @param p pointeur sur la broche
 * @param pucPort Adresse du registre PORT
 * @param ucPin Index de la broche (0 à 7)
 * @param eMode mode
 */
static inline void vDpInit (xDPin * p, volatile uint8_t * pucPort, uint8_t ucPin, eDpMode eMode);

/**
 *   @}
 * @}
 */

#  else
/*
 * __DOXYGEN__ not defined
 * Partie ne devant pas être documentée.
 * =============================================================================
 */
// -----------------------------------------------------------------------------
INLINE void
vDpInit (xDPin * p, volatile uint8_t * pucPort, uint8_t ucPin, eDpMode eMode) {
  p->port = pucPort;
  p->pin = ucPin;
  
#ifdef DPIN_PUE_FIELD
  if (pucPort == &PORTA) {
    p->pue = &PUEA;
  }
#ifdef PUEB
  else if (pucPort == &PORTB) {
    p->pue = &PUEB;
  }
#endif
#ifdef PUEC
else if (pucPort == &PORTC) {
  p->pue = &PUEC;
}
#endif
#endif

  vDpSetMode (p, eMode);
}
#endif

/* ========================================================================== */
__END_C_DECLS
#endif /* _AVRIO_DPIN_H_ */
