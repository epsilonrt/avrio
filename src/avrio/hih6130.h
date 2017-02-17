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
 * @addtogroup sensor_group
 * @{
 *
 *  @defgroup hih6130_module Capteur d'humidité/température I2C HIH6130
 *
 *  Ce module permet la commande d'un capteur I2C HIH6130 de Honeywell.
 *  @{
 * @example sensor/hih6130/demo_sensor_hih6130.c
 * Démonstration utilisation capteur HIH6130
 * Effectue des mesures et les affichent sur le terminal série, les valeurs
 * sont affichées de façon tabulaire afin de pouvoir être traitée par un 
 * tableur.
 */

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
#endif  /* _AVRIO_HIH6130_H_ defined */
