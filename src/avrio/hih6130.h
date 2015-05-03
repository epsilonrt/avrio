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
 * @file hih6130.h
 * @brief Capteur I2c HIH6130 de Honeywell
 *
 * Revision History ------------------------------------------------------------
 *    20120426 - Initial version by epsilonRT
 */
#ifndef _AVRIO_HIH6130_H_
#define _AVRIO_HIH6130_H_

#include <avrio/twi.h>

/**
 * @addtogroup twi_devices_group
 * @{
 *
 *  @defgroup hih6130_module Capteur d'humidité/température I2C HIH6130
 *
 *  Ce module permet la commande d'un capteur I2C HIH6130 de Honeywell.
 *  @{
 */
/* macros =================================================================== */

/* constants ================================================================ */

/**
 * @brief Adresse I2c par défaut du circuit HIH6130
 * @warning Le valeur par défaut indiquée dans la documentation (0x27) est
 * exprimée en hexadécimal alignée à droite. Or L'adresse, sur le bus et dans
 * les fonctions d'AvrIO sont alignée à gauche. C'est la raison pour laquelle
 * la valeur de la documentation est décalée d'un bit vers la gauche.
 */
#define HIH6130_ADDR  (0x27<<1)

/**
 * @enum eHih6130Error
 * @brief Codes d'erreur renvoyés par les fonctions du module.
 */
typedef enum {
  HIH6130_BUSY    =  1, /**< Capteur occupé, la mesure est en cours */
  HIH6130_SUCCESS =  0, /**< Données valides renvoyées */
  HIH6130_TWIERR  = -1, /**< Erreur sur le bus I2C, eHih613LastTwiError() donne alors la cause */
  HIH6130_NODATA  = -2  /**< Pas de données valides disponible */
} eHih6130Error;

/**
 * @brief Données de mesure fournies par un capteur HIH6130
 */
typedef struct xHih6130Data {
  int16_t iHum;   /**< Humidité relative en dixième de % */
  int16_t iTemp;  /**< Température corrigée en dixième de oC */
} xHih6130Data;

/* internal public functions ================================================ */

/**
 * @brief Initialise le circuit HIH6130
 * @param ucConfig Configuration du HIH6130 (inutilisé pour l'instant)
 * @return 0 en cas de succès, une valeur négative en cas d'erreur.
 * @warning Le contrôleur de bus I2C doit être initialisé avant.
 */
eHih6130Error eHih6130Init (uint8_t ucConfig);

/**
 * @brief Démarrer la conversion
 * Le temps de conversion est de 36.65 ms d'après la documentation (vérifié par
 * la mesure).
 * @return 0 en cas de succès, une valeur négative en cas d'erreur.
 */
eHih6130Error eHih6130Start (void);

/**
 * @brief Lecture des derniers résultats de conversion
 * @param pxData Pointeur sur une variable qui contiendra les derniers résultats
 * @return 0 en cas de succès, HIH6130_BUSY tant que la mesure n'est pas
 * disponible, une valeur négative en cas d'erreur.
 */
eHih6130Error eHih6130Read (xHih6130Data * pxData);


#if defined(__DOXYGEN__)
/*
 * __DOXYGEN__ defined
 * Partie documentation ne devant pas être compilée.
 * =============================================================================
 */
/**
 * @brief Lecture de la dernière erreur I2C
 * @return TWI_SUCCESS si la trame a pu être transmise, le code erreur sinon.
 */
inline eTwiStatus eHih613LastTwiError (void);

/**
 *   @}
 * @}
 */
#else
/*
 * __DOXYGEN__ not defined
 * Partie ne devant pas être documentée.
 * =============================================================================
 */

/* public variables ========================================================= */
extern eTwiStatus eHih6130LastTwiErrorValue;

/* macros =================================================================== */

/* inline public functions ================================================== */
// -----------------------------------------------------------------------------
__STATIC_ALWAYS_INLINE (eTwiStatus
eHih6130LastTwiError (void)) {

  return eHih6130LastTwiErrorValue;
}

#endif /* __DOXYGEN__ not defined */
/* ========================================================================== */
__END_C_DECLS
/* *INDENT-ON* */
#endif  /* _AVRIO_HIH6130_H_ defined */
