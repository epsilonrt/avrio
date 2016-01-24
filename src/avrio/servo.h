/**
 * Copyright © 2016 Pascal JEAN aka epsilonRT. All rights reserved.
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
 * @file servo.h
 * @brief Servo-moteurs
 */
#include <avrio/defs.h>

__BEGIN_C_DECLS
/* ========================================================================== */
/**
 * @addtogroup robotic_group
 * @{
 *
 *  @defgroup  servo_module Servo-moteurs
 * 
 *  Ce module permet de commander des servomoteurs de radiomodélisme analogique.
 *  @{
 *  @example servo/demo_servo.c
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
__END_C_DECLS
