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
 * @file quaternionf.h
 * @brief Quatfernion
 *
 * Revision History ------------------------------------------------------------
 *    20121218 - Initial version by epsilonRT
 */
#ifndef _AVRIO_QUATERNIONF_H_
#define _AVRIO_QUATERNIONF_H_

#include <avrio/defs.h>

__BEGIN_C_DECLS
/* ========================================================================== */

/**
 * @addtogroup math_group
 * @{
 *
 *  @defgroup quaternionf_module Quaternions de décimaux
 *  Ce module permet de gérer les Quaternions de nombres décimaux.
 *  @{
 */

/* structures =============================================================== */
typedef struct xQuatf {
  double a, b, c, d;
} xQuatf;

/* internal public functions ================================================ */
/**
 * @brief Norme d'un quaternionf
 *
 * @param xQ pointeur sur le quaternionf
 * @return La norme
 */
double fQuatfNorm (const xQuatf * xQ);

/**
 * @brief Normalisation d'un quaternionf 
 *
 * @param pointeur sur le quaternionf à normaliser et en sortie, le quaternionf normalisé
 */
void vQuatfNormalize (xQuatf * xQ);

/**
 * @brief Egalité de 2 quaternionfs
 *
 * @param xQ1 pointeur sur le premier quaternionf
 * @param xQ2 pointeur sur le deuxième quaternionf
 * @return true si les 2 quaternionfs sont égaux
 */
bool bQuatfEq (const xQuatf * xQ1, const xQuatf *xQ2);

/**
 * @brief Copie d'un quaternionf dans un autre
 *
 * @param xDst pointeur sur le quaternionf destination
 * @param xSrc pointeur sur le quaternionf source
 */
void vQuatfCopy (xQuatf * xDst, const xQuatf *xSrc);

/**
 * @brief Addition de 2 quaternionfs
 * @param xQ1 pointeur sur le premier quaternionf
 * @param xQ2 pointeur sur le deuxième quaternionf
 * @param xQr pointeur sur le quaternionf résultat xQr = xQ1 + xQ2
 */
void vQuatfAdd (const xQuatf *xQ1, const xQuatf *xQ2, xQuatf *xQr);

/**
 * @brief Soustraction de 2 quaternionfs
 * @param xQ1 pointeur sur le premier quaternionf
 * @param xQ2 pointeur sur le deuxième quaternionf
 * @param xQr pointeur sur le quaternionf résultat xQr = xQ1 - xQ2
 */
void vQuatfSub (const xQuatf *xQ1, const xQuatf *xQ2, xQuatf *xQr);

/**
 * @brief Multiplie un quaternionf par une constante
 * @param xQ pointeur sur le quaternionf source et destination
 * @param fConst constante
 */
void vQuatfMulK (xQuatf * xQ, double fConst);

/**
 * @brief Divise un quaternionf par une constante
 * @param xQ pointeur sur le quaternionf source et destination
 * @param fConst constante
 */
void vQuatfDivK (xQuatf * xQ, double fConst);

/**
 *   @}
 * @}
 */

__END_C_DECLS
/* ========================================================================== */
#endif  /* _AVRIO_QUATERNIONF_H_ not defined */
