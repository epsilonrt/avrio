/**
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
 *
 * @file file.h
 * @brief Extension du système de fichiers de la libc
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
 *
 *  @{
 *  @example dpin/demo_dpin.c
 */

/* constants ================================================================ */
typedef enum {

  eModeInput = 0, /**< Entrée sans résistance de pullup */
  eModeOutput = 1, /**< Sortie à 0 à l'initialisation */
  eModeInputPullUp = 2, /**< Entrée avec résistance de pullup */
  eModeOutputHigh = 3, /**< Sortie à 1 à l'initialisation */
  eModeUnknown = -1
} eDpMode;

/* structures =============================================================== */
/**
 * @brief Broche numérique
 */
typedef struct xDPin {
  volatile uint8_t * port; /**< Adresse du registre PORT */
  uint8_t pin; /**< Index de la broche (0 à 7) */
} xDPin;

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
 * @brief Lecture de l'état d'une broche (entrée ou sortie)
 * @param p pointeur sur la broche
 * @return l'état false ou true
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
  vDpSetMode (p, eMode);
}
#endif

/* ========================================================================== */
__END_C_DECLS
#endif /* _AVRIO_DPIN_H_ */
