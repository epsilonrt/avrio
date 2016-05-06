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
 * @file dcm.h
 * @brief Algorithme Direction Cosine Matrix
 *
 * Revision History ------------------------------------------------------------
 *    20130228 - Initial version by epsilonRT
 */
#ifndef _AVRIO_DCM_H_
#define _AVRIO_DCM_H_

#include <avrio/defs.h>
#include <avrio/accelero3d.h>
#include <avrio/compass3d.h>
#include <avrio/gyro3d.h>

__BEGIN_C_DECLS
/* ========================================================================== */

/**
 * @addtogroup nav_group
 * @{
 *
 *  @defgroup dcm_module Algorithme "Direction Cosine Matrix"
 *
 * Le but principal de l’algorithme DCM (contraction de Direct Cosine Matrix) 
 * est d’être capable à tout moment de savoir dans quelle position et 
 * orientation se trouve un objet dans un repère donné. \n
 * Il est couramment utilisé dans la stabilisation d’entité comme une caméra 
 * par exemple, un avion ou encore dans notre cas présent, un hélicoptère. \n
 * En effet grâce à un tel algorithme, on est capable d’estimer avec précision 
 * et donc d’asservir le « pitch » (tangage), le « roll » (roulis) et le « yaw »
 * (lacet) d’un véhicule. \n
 * Ce module utilise les modules accelero3d, gyro3d et compass3d pour 
 * accéder aux capteurs permettant de mesurer les accélérations, les vitesses
 * angulaires et les composantes magnétiques. \n
 * La configuration du module est réalisée dans le fichier avrio-config-dcm.h
 *  @{
 */

/* constants ================================================================ */
/**
 * @brief Valeur renvoyée par iDcmLoop() indiquant que les valeurs d'attitude
 * renvoyées par pxDcmAttitude() ne sont pas encore valides.
 */
#define DCM_NOT_READY 1

/* structures =============================================================== */
/**
 * @struct xDcmAttitude
 * @brief Données renvoyées par le module grâce à pxDcmAttitude()
 */
typedef struct xDcmAttitude {
  /**
   * Angles d'Euler en radians dans l'ordre { Roll, Pitch, Yaw }
   *
   * - Le roulis est exprimé entre -PI et PI radians
   * - Le tangage est exprimé entre -0.5PI et 0.5PI radians
   * - Le lacet est exprimé entre 0 et 2PI radians
   * .
   */
  float fAtt[3];
#define ROLL  0       ///< Index du roulis
#define PITCH 1       ///< Index du tangage
#define YAW   2       ///< Index du lacet
  float fMagHeading;  ///< Cap magnétique entr 0 et 2PI radians
  float fRate[3];     ///< Vitesse angulaires corrigées en radians/s
} xDcmAttitude;

/* internal public functions ================================================ */
/**
 * @brief Initialise le module
 *
 * Cette fonction prend un certain temps de l'ordre de 2 secondes afin 
 * d'effectuer un calage du gyroscope par moyennage.
 * @return 0 en cas de succès, une valeur négative sur erreur.
 */
int iDcmInit (void);

/**
 * @brief Boucle de calcul du filtre DCM
 *
 * Cettte fonction doit être appellée à une cadence corespondant au paramètre
 * CFG_DCM_DT défini dans le fichier avrio-config-dcm.h
 * @return 0 en cas de succès, une valeur négative sur erreur, DCM_NOT_READY
 * tant que le filtre n'a pas convergé (données inexploitables)
 */
int iDcmLoop (void);

/**
 * @brief Renvoie les valeurs actuelles du filtre DCM
 *
 * Ces valeurs ne sont valables que si iDcmLoop() renvoie 0.
 * @return Attitude du mobile
 */
const xDcmAttitude * pxDcmAttitude (void);

/**
 *   @}
 * @}
 */

/* ========================================================================== */
__END_C_DECLS
#endif  /* _AVRIO_DCM_H_ not defined */
