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
 * @file matrixsqf.h
 * @brief Matrice carrée flottante
 *
 * Revision History ------------------------------------------------------------
 *    20121218 - Initial version by epsilonRT
 */
#ifndef _AVRIO_MATRIXSQF_H_
#define _AVRIO_MATRIXSQF_H_

#include <avrio/defs.h>

__BEGIN_C_DECLS
/* ========================================================================== */

/**
 * @addtogroup math_group
 * @{
 *
 *  @defgroup matrixsqf_module Matrices carrées de décimaux
 *  Ce module permet de gérer les matrices carrées de nombres décimaux.
 *  @{
 */

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

/**
 *   @}
 * @}
 */

__END_C_DECLS
/* ========================================================================== */
#endif  /* _AVRIO_MATRIXSQF_H_ not defined */
