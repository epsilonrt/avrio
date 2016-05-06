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
 * @file accelero3d.h
 * @brief Acceleromètre 3 axes
 *
 * Revision History ------------------------------------------------------------
 *    20121218 - Initial version by epsilonRT
 */
#ifndef _AVRIO_ACCELERO3D_H_
#define _AVRIO_ACCELERO3D_H_

#include <avrio/defs.h>

__BEGIN_C_DECLS
/* ========================================================================== */
#include <avrio/vector3f.h>
#include <avrio/vector3i.h>

/**
 * @addtogroup nav_group
 * @{
 *
 *  @defgroup acc3d_module Accéléromètre 3 axes

 *  Ce module permet de gérer un accéléromètre 3 axes. \n
 * -------- Convention des axes utilisée -------- \n
 * La convention utilisée est celle des aéronefs : \n
 * - X axe longitudinal (dans le sens de la marche), c'est l'axe du roulis (roll). 
 *   Positif lorsque l'aile droite descend (virage droite)
 * - Y axe transversal, c'est l'axe du tangage ou piqué (pitch). 
 *   Positif à monter.
 * - Z axe normal au plan XY (vertical), c'est l'axe de lacel (yaw)
 *   Positif vers la droite (dans le sens horaire).
 * .
 * En ce qui converne l'accélaration, la convention est que tout axe aligné sur
 * la gravité terrestre vers le bas aura une valeur de +1g. Ainsi les 
 * accélérations en g seront : \n
 * - à l'horizontal {0,0,1}
 * - en piqué à descendre {1,0,0}
 * - en virage sur l'aile droite {0,1,0)
 * .
 *  @{
 */
/* constants ================================================================ */

/**
 * @brief Indique que le gyroscope n'a pas été calibré
 */
#define ACCELERO3D_NOT_CALIBRATED (1)

/* internal public functions ================================================ */
/**
 *
 * Le circuit est initialisé et la valeur du zéro est chargée de l'EEPROM. Si
 * la valeur stockée en EEPROM est corrompue, la valeur du zéro est remise
 * à zéro et stockée en EEPROM et la fonction retourne ACCELERO3D_NOT_CALIBRATED.
 *
 * @return 0 en cas de succès, ACCELERO3D_NOT_CALIBRATED si une calibration est 
 * nécessaire, une valeur négative sur erreur.
 */
int  iAccelero3dInit (void);

/**
 * @brief Calibration de l'accelero
 *
 * Calcul la valeur du zéro par moyennage d'un certains nombre de mesures et
 * stocke le résultat en EEPROM
 *
 * @param usNumOfSamples Nombre d'échantillons de mesure utilisés pour le calcul
 * @return 0 en cas de succès, une valeur négative sur erreur.
 */
int iAccelero3dCalibrate (uint16_t usNumOfSamples);

/**
 * @brief Lecture des accelérations d'un Accelero
 *
 * @param[out] fAcc Pointeur sur le vecteur dans lequel sont stockées 
 * les accelérations (en g)
 * @return 0 en cas de succès, une valeur négative sur erreur.
 */
int iAccelero3dRead (float * fAcc);

/**
 * @brief Lecture des accelérations d'un Accelero
 *
 * @param[out] iAcc Pointeur sur le vecteur dans lequel seront stockées 
 * les accelérations (en LSB)
 * @return 0 en cas de succès, une valeur négative sur erreur.
 */
int iAccelero3dReadRaw (int * iAcc);

/**
 * @brief Lecture du calage du zéro
 *
 * @param[out] iZero Pointeur sur le vecteur dans lequel sont stockées 
 * les accelérations calculées lors du calibrage(en LSB)
 */
void vAccelero3dZero (int * iZero);

/**
 * @brief Modification du calage du zéro
 *
 * La valeur est aussi stockée en EEPROM afin d'être rechargée lors de 
 * l'initialisation.
 *
 * @param[in] iZero Pointeur sur le vecteur de zéro
 */
void vAccelero3dSetZero (const int * iZero);

/**
 * @brief Calcul des angles de piqué (x) et de roulis (y) par rapport à la 
 * verticale à partir des accélérations.
 *
 * @param[in] fAcc Pointeur sur le vecteur dans lequel sont stockées 
 * les accelérations (en g)
 * @param[in,out] fAtt Pointeur sur le vecteur dans lequel seront stockées les 
 * angles (x: roulis, y: piqué ou tangage). 
 * @param fAlpha La composante z (\f$\alpha\f$) de ce vecteur permet de passer à 
 * la fonction le  coefficient du filtre numérique passe-bas intégré à la 
 * fonction:
 * \f[y_{t} = \alpha \cdot x_{t} - (1 - \alpha) \cdot y_{t - 1}\f]
 * - \f$y_{t}\f$ sortie du filtre (l'accélaration filtrée)
 * - \f$x_{t}\f$ entrée du filtre (l'accélaration mesurée)
 * - \f$y_{t - 1}\f$ sortie du filtre au calcul précédent
 * - \f$\alpha\f$  coefficient du filtre. Plus la valeur de ce paramètre est faible 
 *   est plus le filtrage sera important. \n
 *   La valeur doit être dans l'intervalle ]0 ; 1[ car 1 correspond à une 
 *   absence de filtrage (\f$y_{t} = x_{t}\f$) et 0 à un filtrage complet du signal
 *   d'entrée. En conséquence, les valeurs suivantes de \f$\alpha\f$ seront 
 *   interprétées de la façon suivante :
 *   - \f$\alpha=1\f$ : pas de filtrage
 *   - \f$\alpha=0\f$ : remise à zéro du filtre (\f$y_{t} = 0\f$)
 *   - \f$\alpha\in]0,1[\f$ : filtrage 
 *   .
 * .
 */
void vAccelero3dAttitude (float * fAtt, const float * fAcc, float fAlpha);

/**
 *   @}
 * @}
 */

__END_C_DECLS
/* ========================================================================== */
#endif  /* _AVRIO_ACCELERO3D_H_ not defined */

