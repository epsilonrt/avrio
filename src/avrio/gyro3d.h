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
 * @file gyro3d.h
 * @brief Gyroscope 3 axes
 *
 * Revision History ------------------------------------------------------------
 *    20121218 - Initial version by epsilonRT
 */
#ifndef _AVRIO_GYRO3D_H_
#define _AVRIO_GYRO3D_H_

#include <avrio/defs.h>

/* *INDENT-OFF* */
__BEGIN_C_DECLS
/* ========================================================================== */
#include <avrio/vector3f.h>
#include <avrio/vector3i.h>

/**
 * @addtogroup robotic_group
 * @{
 *
 *  @defgroup gyro3d_module Gyroscope 3 axes

 *  Ce module permet de gérer un gyroscope 3 axes. \n
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
#define GYRO3D_NOT_CALIBRATED (1)

/* internal public functions ================================================ */
/**
 * @brief Initialise le module Gyro
 *
 * Le circuit est initialisé et la valeur du zéro est chargée de l'EEPROM. Si
 * la valeur stockée en EEPROM est corrompue, la valeur du zéro est remise
 * à zéro et stockée en EEPROM et la fonction retourne GYRO3D_NOT_CALIBRATED.
 *
 * @return 0 en cas de succès, GYRO3D_NOT_CALIBRATED si une calibration est 
 * nécessaire, une valeur négative sur erreur.
 */
int  iGyro3dInit (void);

/**
 * @brief Calibration du gyro
 *
 * Calcul la valeur du zéro par moyennage d'un certains nombre de mesures et
 * stocke le résultat en EEPROM
 *
 * @param usNumOfSamples Nombre d'échantillons de mesure utilisés pour le calcul
 * @return 0 en cas de succès, une valeur négative sur erreur.
 */
int iGyro3dCalibrate (uint16_t usNumOfSamples);

/**
 * @brief Lecture des vitesses angulaires d'un Gyro
 *
 * @param[out] xRate Pointeur sur le vecteur dans lequel seront stockées 
 * les vitesses d'angle (en °/s)
 * @return 0 en cas de succès, une valeur négative sur erreur.
 */
int iGyro3dRead (float * xRate);

/**
 * @brief Lecture des vitesses angulaires brutes d'un Gyro
 *
 * @param[out] xRate Pointeur sur le vecteur dans lequel seront stockées 
 * les vitesses d'angle en LSB
 * @return 0 en cas de succès, une valeur négative sur erreur.
 */
int iGyro3dReadRaw (int * xRate);

/**
 * @brief Lecture du calage du zéro
 *
 * @param[out] xZero Pointeur sur le vecteur dans lequel sont stockées 
 * les vitesses calculées lors du calibrage (en LSB)
 */
void vGyro3dZero (int * xZero);

/**
 * @brief Modification du calage du zéro
 *
 * La valeur est aussu stockée en EEPROM afin d'être rechargée lors de 
 * l'initialisation.
 *
 * @param[in] xZero Pointeur sur le vecteur de zéro
 */
void vGyro3dSetZero (const int * xZero);

/**
 * @brief Sauvegarde du calage du zéro courant en EEPROM
 */
void vGyro3dSaveZero (void);

/**
 * @brief Calcul des angles à partir des vitesses angulaires
 *
 * Requiert le module task et donc un timer kernel (AVRIO_TASK_ENABLE).
 *
 * @param[in] xRate Pointeur sur le vecteur dans lequel sont stockées 
 * les vitesses angulaires (en g)
 * @param[in,out] xAtt Pointeur sur le vecteur dans lequel seront stockées les 
 * angles (x: roulis, y: piqué ou tangage, z: lacet).
 * @param
 * @param fAlpha La composante z (\f$\alpha\f$) de ce vecteur permet de passer à 
 * la fonction le  coefficient du filtre numérique passe-bas intégré à la 
 * fonction:
 * \f[y_{t} = \alpha \cdot x_{t} - (1 - \alpha) \cdot y_{t - 1}\f]
 * - \f$y_{t}\f$ sortie du filtre (la vitesse angulaire filtrée)
 * - \f$x_{t}\f$ entrée du filtre (la vitesse angulaire mesurée)
 * - \f$y_{t - 1}\f$ sortie du filtre au calcul précédent
 * - \f$\alpha\f$  coefficient du filtre. Plus la valeur de ce paramètre est faible 
 *   est plus le filtrage sera important. \n
 *   La valeur doit être dans l'intervalle ]0 ; 1[ car 1 correspond à une 
 *   absence de filtrage (\f$y_{t} = x_{t}\f$) et 0 à un filtrage complet du signal
 *   d'entrée. En conséquence, les valeurs suivantes de \f$\alpha\f$ seront 
 *   interprétées de la façon suivante :
 *   - \f$\alpha=1\f$ : pas de filtrage
 *   - \f$\alpha\in]0,1[\f$ : filtrage 
 *   .
 * .
 * @return 0 en cas de succès, négatif en cas d'erreur 
 */
int iGyro3dAttitude (float * xAtt, const float * xRate, ticks_t xRateTime, float fA);

/**
 * @brief Réinitialise le calcul d'attitude
 *
 * Réinitialise le vecteur passé en arguments et les filtres passe-bas et 
 * d'intégration de la fonction iGyro3dAttitude()
 * @param[in,out] xAtt Pointeur sur le vecteur dans lequel seront stockées les 
 * angles (x: roulis, y: piqué ou tangage, z: lacet).
 */
void vGyro3dAttidudeClear (float * xAtt);

/**
 *   @}
 * @}
 */

__END_C_DECLS
/* *INDENT-ON* */
/* ========================================================================== */
#endif  /* _AVRIO_GYRO3D_H_ not defined */

