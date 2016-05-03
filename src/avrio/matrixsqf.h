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
 * @file matrixsqf.h
 * @brief Matrice carrée flottante
 *
 * Revision History ------------------------------------------------------------
 *    20121218 - Initial version by epsilonRT
 */
#ifndef _AVRIO_MATRIXSQF_H_
#define _AVRIO_MATRIXSQF_H_

#include <avrio/defs.h>

/* *INDENT-OFF* */
__BEGIN_C_DECLS
/* ========================================================================== */

#define MSQ(_mat) _mat[0]

/* internal public functions ================================================ */

/**
 * @brief Addition de 2 matrices carrée
 * @param xIn1 pointeur sur la première matrice
 * @param xIn2 pointeur sur la deuxième matrice
 * @param xOut pointeur sur la matrice résultat xOut = xIn1 + xIn2
 */
void vMatrixSqfAdd (float * xOut, const float * xIn1, const float * xIn2,  uint8_t ucOrder);

/**
 * @brief Soustraction de 2 matrices carrée
 * @param xIn1 pointeur sur la première matrice
 * @param xIn2 pointeur sur la deuxième matrice
 * @param xOut pointeur sur la matrice résultat xOut = xIn1 - xIn2
 */
void vMatrixSqfSub (float * xOut, const float * xIn1, const float * xIn2,  uint8_t ucOrder);

/**
 * @brief Multiplication de 2 matrices carrée
 * @param xIn1 pointeur sur la première matrice
 * @param xIn2 pointeur sur la deuxième matrice
 * @param xOut pointeur sur la matrice résultat xOut = xIn1 * xIn2
 */
void vMatrixSqfMul (float * xOut, const float * xIn1, const float * xIn2,  uint8_t ucOrder);

/**
 * @brief Multiplie un matrice par une constante
 * @param xMat pointeur sur la matrice source et destination
 * @param fConst constante
 */
void vMatrixSqfScale (float * xOut, const float * xIn, float fConst, uint8_t ucOrder);

/**
 * @brief Multiplie un matrice par une constante
 * @param xMat pointeur sur la matrice source et destination
 * @param fConst constante
 */
void vMatrixSqfMulK (float * xMat, float fConst, uint8_t ucOrder);

/**
 * @brief Divise un matrice par une constante
 * @param xMat pointeur sur la matrice source et destination
 * @param fConst constante
 */
void vMatrixSqfDivK (float * xMat, float fConst, uint8_t ucOrder);

/**
 * @brief Remise à zéro d'un matrice 
 * @param pointeur sur la matrice à effacer et en sortie, la matrice effacé
 */
void vMatrixSqfClear (float * xIn1, uint8_t ucOrder);

/**
 * @brief Test si un matrice est nul
 * @param pointeur sur la matrice à tester
 */
bool bMatrixSqfIsCleared (const float * xIn1, uint8_t ucOrder);

/**
 * @brief Egalité de 2 matrices
 * @param xIn1 pointeur sur la première matrice
 * @param xIn2 pointeur sur la deuxième matrice
 * @return true si les 2 matrices sont égales
 */
bool bMatrixSqfEq (const float * xIn1, const float * xIn2, uint8_t ucOrder);

/**
 * @brief Copie d'un matrice dans un autre
 * @param xDst pointeur sur la matrice destination
 * @param xSrc pointeur sur la matrice source
 */
void vMatrixSqfCopy (float * xDst, const float * xSrc, uint8_t ucOrder);

__END_C_DECLS
/* *INDENT-ON* */
/* ========================================================================== */
#endif  /* _AVRIO_MATRIXSQF_H_ not defined */