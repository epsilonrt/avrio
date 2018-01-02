/**
 * Copyright © 2016 epsilonRT. All rights reserved.
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
 * @file servo.h
 * @brief Servo-moteurs
 */
#include <avrio/defs.h>

#if defined(__cplusplus)
  extern "C" {
#endif
/* ========================================================================== */
/**
 * @addtogroup robotic_group
 * @{
 *
 *  @defgroup  servo_module Servo-moteurs
 * 
 *  Ce module permet de commander des servomoteurs de radiomodélisme analogique.
 *  @{
 *  @example output/servo/demo_servo.c
 *  Commande la rotation de servo avec un bouton poussoir. 
 */
/* internal public functions ================================================ */

/**
 * @brief Initialise le module
 * 
 * Au démarrage les voies sont validées.
 */
void vServoInit (void);

/**
 * @brief Valide ou non un servomoteur
 * @param ucChannel voie du servomoteur
 * @param bEnabled true pour valider, false sinon
 */
void vServoEnable (uint8_t ucChannel, bool bEnabled);

/**
 * @brief Nombre de voies
 * @return 
 */
uint8_t ucServoChannels (void);

/**
 * @brief Réglage d'une durée d'impulsion
 * @param ucChannel voie du servomoteur
 * @param usPulseUs largeur d'impulsion en us
 */
void vServoSetPulse (uint8_t ucChannel, uint16_t usPulseUs);

/**
 * @brief Lecture de la durée d'impulsion réglée
 * @param ucChannel  voie du servomoteur
 * @return largeur d'impulsion en us
 */
uint16_t usServoGetPulse (uint8_t ucChannel);

/**
 *  @}
 * @}
 */
 
/* ========================================================================== */
#if defined(__cplusplus)
  }
#endif
