/**
 * Copyright © 2011-2015 epsilonRT. All rights reserved.
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
 * @file quaternionf.h
 * @brief Quatfernion
 *
 * Revision History ------------------------------------------------------------
 *    20121218 - Initial version by epsilonRT
 */
#ifndef _AVRIO_QUATERNIONF_H_
#define _AVRIO_QUATERNIONF_H_

#include <avrio/defs.h>

/* *INDENT-OFF* */
__BEGIN_C_DECLS
/* ========================================================================== */

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

__END_C_DECLS
/* *INDENT-ON* */
/* ========================================================================== */
#endif  /* _AVRIO_QUATERNIONF_H_ not defined */