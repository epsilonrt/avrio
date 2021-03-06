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
 * @file vector3f.h
 * @brief Vecteur tridimensionnel
 *
 * Revision History ------------------------------------------------------------
 *    20121218 - Initial version by epsilonRT
 */
#ifndef _AVRIO_VECTOR3F_H_
#define _AVRIO_VECTOR3F_H_

#include <avrio/defs.h>

__BEGIN_C_DECLS
/* ========================================================================== */

/**
 * @addtogroup math_group
 * @{
 *
 *  @defgroup vector3f_module Vecteurs tridimensionnels de décimaux
 *  Ce module permet de gérer les vecteurs tridimensionnels de nombres décimaux.
 *  @{
 */

/* constants ================================================================ */
#define roll 0
#define pitch 1
#define yaw 2

/* internal public functions ================================================ */
/**
 * @brief Produit vectoriel de 2 vecteurs
 * @param xIn1 pointeur sur le premier vecteur
 * @param xIn2 pointeur sur le deuxième vecteur
 * @param xOut pointeur sur le vecteur résultat
 */
void vVector3fCross (float * xOut, const float * xIn1, const float * xIn2);

/**
 * @brief Produit scalaire de 2 vecteurs
 * @param xIn1 pointeur sur le premier vecteur
 * @param xIn2 pointeur sur le deuxième vecteur
 * @return Le produit sclaire
 */
float fVector3fDot (const float * xIn1, const float * xIn2);

/**
 * @brief Norme d'un vecteur
 * @param xIn1 pointeur sur le vecteur
 * @return La norme
 */
float fVector3fAbs (const float * xIn1);

/**
 * @brief Normalisation d'un vecteur 
 * @param pointeur sur le vecteur à normaliser et en sortie, le vecteur normalisé
 */
void vVector3fNormalize (float * xIn1);

/**
 * @brief Remise à zéro d'un vecteur 
 * @param pointeur sur le vecteur à effacer et en sortie, le vecteur effacé
 */
void vVector3fClear (float * xIn1);

/**
 * @brief Test si un vecteur est nul
 * @param pointeur sur le vecteur à tester
 */
bool bVector3fIsCleared (const float * xIn1);

/**
 * @brief Egalité de 2 vecteurs
 * @param xIn1 pointeur sur le premier vecteur
 * @param xIn2 pointeur sur le deuxième vecteur
 * @return true si les 2 vecteurs sont égaux
 */
bool bVector3fEq (const float * xIn1, const float * xIn2);

/**
 * @brief Copie d'un vecteur dans un autre
 * @param xDst pointeur sur le vecteur destination
 * @param xSrc pointeur sur le vecteur source
 */
void vVector3fCopy (float * xDst, const float * xSrc);

/**
 * @brief Addition de 2 vecteurs
 * @param xIn1 pointeur sur le premier vecteur
 * @param xIn2 pointeur sur le deuxième vecteur
 * @param xOut pointeur sur le vecteur résultat xOut = xIn1 + xIn2
 */
void vVector3fAdd (float * xOut, const float * xIn1, const float * xIn2);

/**
 * @brief Soustraction de 2 vecteurs
 * @param xIn1 pointeur sur le premier vecteur
 * @param xIn2 pointeur sur le deuxième vecteur
 * @param xOut pointeur sur le vecteur résultat xOut = xIn1 - xIn2
 */
void vVector3fSub (float * xOut, const float * xIn1, const float * xIn2);

/**
 * @brief Multiplie un vecteur par une constante
 * @param xOut pointeur sur le vecteur résultat
 * @param xIn1 pointeur sur le vecteur opérande
 * @param fConst constante
 */
void vVector3fScale (float * xOut, const float * xIn1, float fConst);

/**
 * @brief Multiplie un vecteur par une constante
 * @param xVect pointeur sur le vecteur source et destination
 * @param fConst constante
 */
void vVector3fMulK (float * xVect, float fConst);

/**
 * @brief Divise un vecteur par une constante
 * @param xVect pointeur sur le vecteur source et destination
 * @param fConst constante
 */
void vVector3fDivK (float * xVect, float fConst);

/**
 *   @}
 * @}
 */

__END_C_DECLS
/* ========================================================================== */
#endif  /* _AVRIO_VECTOR3F_H_ not defined */
