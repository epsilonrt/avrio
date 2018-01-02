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
 * @file util.h
 * @brief Utilitaires
 *
 * Revision History ------------------------------------------------------------
 *    20111113 - Initial version by epsilonRT
 */
#ifndef _AVRIO_UTIL_H_
#define _AVRIO_UTIL_H_

#include <avrio/defs.h>

#if defined(__cplusplus)
  extern "C" {
#endif
/* ======================================================================== */
/**
 * @addtogroup util_group
 * @{
 */

/* internal public functions ============================================== */

/**
 * @brief Permute les octets 2 à 2
 *
 *  Passage de big à little endian ou l'inverse
 * @param pucBytes pointeur sur les octets
 * @param ucSize nombre d'ocets à permuter (doit être pair)
 */
void vSwapBytes (uint8_t * pucBytes, uint8_t ucSize);

/**
 * @brief Inverse l'ordre des bits d'un octet
 * @param ucByte octet
 * @return octet inversé
 */
uint8_t ucUtilReverseBits (uint8_t ucByte);

/**
 * @brief Génére un nombre pseudo aléatoire 8 bits
 *
 * Générateur à congruence linéaire de période 256
 * Par défaut, la valeur de départ est 255
 */
uint8_t rand8 (void);

/**
 * @brief Modifie la valeur initiale du générateur à congruence linéaire utilisé par \c rand8()
 * Cette valeur ne doit jamais être nulle.
 */
void srand8 (uint8_t seed);


/* public variables ======================================================= */
/* inline public functions ================================================ */
#  if defined(__DOXYGEN__)
/*
 * __DOXYGEN__ defined
 * Partie documentation ne devant pas être compilée.
 * =========================================================================
 */

/**
 * @}
 */
#  else
/*
 * __DOXYGEN__ not defined
 * Partie ne devant pas être documentée.
 * =========================================================================
 */
#  endif /* __DOXYGEN__ not defined */
/* ======================================================================== */
#if defined(__cplusplus)
  }
#endif
#endif  /* _AVRIO_UTIL_H_ defined */

