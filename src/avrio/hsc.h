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
 * @file hsc.h
 * @brief Capteur de pression Honeywell HSC
 *
 * Revision History ------------------------------------------------------------
 *    20140312 - Initial version by epsilonRT
 */
#ifndef _AVRIO_HSC_H_
#define _AVRIO_HSC_H_
#include <avrio/defs.h>

/* *INDENT-OFF* */
__BEGIN_C_DECLS
/* ========================================================================== */

/**
 * @addtogroup dev_group
 * @{
 *
 *  @defgroup hsc_module Capteur de pression Honeywell HSC
 *  Ce module permet de gérer des capteurs de pression Honeywell HSC.
 *  Les capteurs HSC sont des capteurs de pression numériques avec une résolution
 *  de 14 bits. Suivant le modèle choisi, il peut être interfacé au MCU sur
 *  bus SPI ou I2C. \n
 *  @warning Seul l'interface SPI est pour l'instant gérée.
 *  @{
 *    @example hsc/demo_hsc.c
 *    Exemple qui affiche la pression et la température sur la liaison série.
 */

/* macros =================================================================== */
/**
 * @brief Convertion psi en pascal
 */
#define PSI_TO_PA(_psi) ((double)(_psi)*6894.76f)

/* constants ================================================================ */
/**
 * @brief Bus utilisé par le capteur
 *
 * Le capteur est soit SPI, soit I2C
 */
typedef enum {
  HSC_SPI,
  HSC_I2C
} eHscBus;

/**
 * @brief Code d'état
 *
 * Valeurs renvoyées par les fonctions du module
 */
typedef enum {
  HSC_SUCCESS = 0,  /**< Pas d'erreur */
  HSC_COMMAND_MODE = -1, /**< Capteur en mode commande (impossible normalement) */
  HSC_STALE_DATA = -2, /**< Données périmées: \n
                        * les données ont déjà été récupérés depuis le dernier
                        * cycle de mesure, ou données extraites avant que la
                        * première mesure ait été complétée
                        */
  HSC_DIAGNOSTIC = -3
} eHscStatus;

/**
 * @brief Adresse I2c par défaut
 */
#define HSC_DEFAULT_I2CADDR (0x28)

/* types ==================================================================== */
/**
 * Type de fonction qui sélectionne un capteur en affirmant sa broche /SS
 */
typedef void (*vHscSpiSelect)(bool);

/**
 * Type de fonction qui initialise la broche /SS d'un capteur
 */
typedef void (*vHscSpiInit)(void);

/* structures =============================================================== */
/**
 * @brief Données brutes lues dans un capteur
 */
typedef struct xHscRaw {

  uint16_t usPress; ///< Pression sur 14 bits alignés à droite (sans status)
  uint16_t usTemp;  ///< Température sur 11 bits alignés à droite
} xHscRaw;

/**
 * @brief Valeurs calculées de la pression et de la température
 */
typedef struct xHscValue {

  double dPress;  ///< Pression exprimée dans l'unité fournie à l'initialiasation
  double dTemp;   ///< Température exprimée en °C
} xHscValue;

/**
 * @brief Capteur HSC
 */
typedef struct xHscSensor {

  double dPressMin; ///< Pression minimale du capteur
  double dPressMax; ///< Pression maximale du capteur
  eHscBus eBus; ///< Type de bus utilisé par le capteur
  union {
    uint8_t ucI2cAddr;
    struct {
      vHscSpiInit vInit; ///< Pointeur sur la fonction d'initialisation capteur
      vHscSpiSelect vSelect; ///< Pointeur sur la fonction de validation du capteur
    };
  };
  // TODO: I2C
} xHscSensor;

/* internal public functions ================================================ */
/**
 * @brief Lecture capteur
 *
 * Effectue une mesure avec usAdcSensorGetRaw() et la convertit avec
 * dAdcSensorRawToValue()
 *
 * @param pSensor Pointeur sur le capteur à utiliser
 * @param pValue Valeur des grandeurs mesurées (pression et température)
 * @return  HSC_SUCCESS (0) ou un code erreur eHscStatus
 */
int iHscGetValue (xHscSensor *pSensor, xHscValue *pValue);

/**
 * @brief Convertit des valeurs brutes en grandeurs capteur
 *
 * @param pSensor Pointeur sur le capteur à utiliser
 * @param pRaw Valeurs brutes lues en sortie du capteur
 * @param pValue Valeur des grandeurs mesurées (pression et température)
 *        associées à pRaw
 */
void vHscRawToValue (xHscSensor *pSensor, const xHscRaw *pRaw, xHscValue *pValue);

/**
 * @brief Lecture valeurs brutes
 *
 * Renvoie les valeurs brutes en sortie du capteur
 *
 * @param pSensor Pointeur sur le capteur à utiliser
 * @param pRaw Valeurs brutes correspodant aux grandeurs mesurées
 * @return  HSC_SUCCESS (0) ou un code erreur eHscStatus
 */
int iHscGetRaw (xHscSensor *pSensor, xHscRaw *pRaw);

#  if defined(__DOXYGEN__)
/*
 * __DOXYGEN__ defined
 * Partie documentation ne devant pas être compilée.
 * =============================================================================
 */

/**
 * @brief Initialisation capteur HSC SPI
 *
 * Attention, il est nécessaire d'initialiser le bus SPI séparément.
 *
 * @param pSensor Pointeur sur le capteur à utiliser
 * @param dPressMin Pression minimale du capteur
 * @param dPressMax Pression maximale du capteur
 * @param vInit Pointeur sur la fonction d'initialisation spécifique au capteur
 *        (init. broche /SS ...). Si inutilisé, mettre 0.
 * @param vSelect Pointeur sur la fonction de validation de la broche /SS du
 *        capteur. Cette fonction est obligatoire.
 * @return  HSC_SUCCESS (0) ou un code erreur eHscStatus
 */
int
iHscInitSpiSensor (xHscSensor *pSensor,  double dPressMin, double dPressMax,
                                vHscSpiInit vInit, vHscSpiSelect vSelect);
/**
 *   @}
 * @}
 */
#  else
/*
 * __DOXYGEN__ not defined
 * Partie ne devant pas être documentée.
 * =============================================================================
 */

/* internal inline functions ================================================ */

// -----------------------------------------------------------------------------
INLINE int
iHscInitSpiSensor (xHscSensor *pSensor,  double dPressMin, double dPressMax,
                                vHscSpiInit vInit, vHscSpiSelect vSelect) {

  pSensor->dPressMin = dPressMin;
  pSensor->dPressMax = dPressMax;
  pSensor->vInit = vInit;
  pSensor->vSelect = vSelect;
  pSensor->eBus = HSC_SPI;

  if (pSensor->vInit)
    pSensor->vInit();
  return 0;
}

// -----------------------------------------------------------------------------
// TODO: I2C

// -----------------------------------------------------------------------------
#  endif /* __DOXYGEN__ not defined */

/* ========================================================================== */
__END_C_DECLS
/* *INDENT-ON* */
#endif  /* _AVRIO_HSC_H_ not defined */
