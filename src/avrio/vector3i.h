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
 * @file vector3i.h
 * @brief Vecteur tridimensionnel
 *
 * Revision History ------------------------------------------------------------
 *    20121218 - Initial version by epsilonRT
 */
#ifndef _AVRIO_VECTOR3I_H_
#define _AVRIO_VECTOR3I_H_

#include <avrio/defs.h>

/* *INDENT-OFF* */
__BEGIN_C_DECLS
/* ========================================================================== */

/* internal public functions ================================================ */
/**
 * @brief Produit vectoriel de 2 vecteurs 3D
 * @param xIn1 pointeur sur le premier vecteur
 * @param xIn2 pointeur sur le deuxième vecteur
 * @param xOut pointeur sur le vecteur résultat
 */
void vVector3iCross (int * xOut, const int * xIn1, const int * xIn2);

/**
 * @brief Produit scalaire de 2 vecteurs 3D
 * @param xIn1 pointeur sur le premier vecteur
 * @param xIn2 pointeur sur le deuxième vecteur
 * @return Le produit sclaire
 */
int iVector3iDot (const int * xIn1, const int * xIn2);

/**
 * @brief Remise à zéro d'un vecteur 
 * @param pointeur sur le vecteur à effacer et en sortie, le vecteur normalisé
 */
void vVector3iClear (int * xIn1);

/**
 * @brief Egalité de 2 vecteurs
 * @param xIn1 pointeur sur le premier vecteur
 * @param xIn2 pointeur sur le deuxième vecteur
 * @return true si les 2 vecteurs sont égaux
 */
bool bVector3iEq (const int * xIn1, const int * xIn2);

/**
 * @brief Copie d'un vecteur dans un autre
 * @param xDst pointeur sur le vecteur destination
 * @param xSrc pointeur sur le vecteur source
 */
void vVector3iCopy (int * xDst, const int * xSrc);

/**
 * @brief Copie d'un vecteur dans un autre (la source est un vecteur long)
 * @param xDst pointeur sur le vecteur destination
 * @param xSrc pointeur sur le vecteur source
 */
void vVector3iCopyLong (int * xDst, const long * xSrc);

/**
 * @brief Addition de 2 vecteurs 3D
 * @param xIn1 pointeur sur le premier vecteur
 * @param xIn2 pointeur sur le deuxième vecteur
 * @param xOut pointeur sur le vecteur résultat xOut = xIn1 + xIn2
 */
void vVector3iAdd (int * xOut, const int * xIn1, const int * xIn2);

/**
 * @brief Soustraction de 2 vecteurs 3D
 * @param xIn1 pointeur sur le premier vecteur
 * @param xIn2 pointeur sur le deuxième vecteur
 * @param xOut pointeur sur le vecteur résultat xOut = xIn1 - xIn2
 */
void vVector3iSub (int * xOut, const int * xIn1, const int * xIn2);

/**
 * @brief Multiplie un vecteur par une constante
 * @param xVect pointeur sur le vecteur source et destination
 * @param iConst constante
 */
void vVector3iScale (int * xOut, const int * xIn1, int iConst);

/**
 * @brief Multiplie un vecteur par une constante
 * @param xVect pointeur sur le vecteur source et destination
 * @param iConst constante
 */
void vVector3iMulK (int * xVect, int iConst);

/**
 * @brief Divise un vecteur par une constante
 * @param xVect pointeur sur le vecteur source et destination
 * @param iConst constante
 */
void vVector3iDivK (int * xVect, int iConst);

__END_C_DECLS
/* *INDENT-ON* */
/* ========================================================================== */
#endif  /* _AVRIO_VECTOR3I_H_ not defined */