/**
 * @file util.h
 * @brief Utilitaires
 * @author Copyright © 2011-2012 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * Revision History ------------------------------------------------------------
 *    20111113 - Initial version by epsilonRT
 */
#ifndef _AVRIO_UTIL_H_
#define _AVRIO_UTIL_H_

#include <avrio/defs.h>

/* *INDENT-OFF* */
__BEGIN_C_DECLS
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
__END_C_DECLS
/* *INDENT-ON* */
#endif  /* _AVRIO_UTIL_H_ defined */

