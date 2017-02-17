/**
 * @file hsc.h
 * @brief Capteur de pression Honeywell HSC
 *
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
 */
#ifndef _AVRIO_HSC_H_
#define _AVRIO_HSC_H_
#include <avrio/defs.h>

__BEGIN_C_DECLS
/* ========================================================================== */

/**
 * @addtogroup sensor_group
 * @{
 *
 *  @defgroup hsc_module Capteur de pression Honeywell HSC
 *  Ce module permet de gérer des capteurs de pression Honeywell HSC.
 *  Les capteurs HSC sont des capteurs de pression numériques avec une résolution
 *  de 14 bits. Suivant le modèle choisi, il peut être interfacé au MCU sur
 *  bus SPI ou I2C. \n
 *  - La constante HSC_SPI_ENABLE doit être ajoutée à la configuration d'AvrIO
 *  (AVRIO_CONFIG) pour valider la prise en charge de capteur SPI.
 *  - La constante HSC_TWI_ENABLE doit être ajoutée à la configuration d'AvrIO
 *  (AVRIO_CONFIG) pour valider la prise en charge de capteur I2C.
 *  .
 *  @{
 * @example sensor/hsc/spi/demo_sensor_hsc_spi.c
 * Démonstration utilisation capteur pression HSC (SPI)
 * Effectue des mesures et les affichent sur le terminal série, les valeurs
 * sont affichées de façon tabulaire afin de pouvoir être traitée par un 
 * tableur.
 * 
 * @example sensor/hsc/twi/demo_sensor_hsc_twi.c
 * Démonstration utilisation capteur pression HSC (TWI)
 * Effectue des mesures et les affichent sur le terminal série, les valeurs
 * sont affichées de façon tabulaire afin de pouvoir être traitée par un 
 * tableur.
 */

/* macros =================================================================== */
/**
 * @brief Convertion psi en pascal
 */
#define PSI_TO_PA(_psi) ((double)(_psi)*6894.76f)
#define PSI_TO_HPA(_psi) ((double)(_psi)*68.9476f)

/* constants ================================================================ */
/**
 * @brief Bus utilisé par le capteur
 *
 * Le capteur est soit SPI, soit I2C
 */
typedef enum {
  HSC_SPI,  /**< Interface SPI */
  HSC_TWI   /**< Interface I2C */
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
  HSC_DIAGNOSTIC = -3,
  HSC_TWI_ERROR = -4
} eHscStatus;

/**
 * @brief Adresse I2c par défaut
 */
#define HSC_DEFAULT_TWIADDR (0x28)

/* types ==================================================================== */
/**
 * Type de fonction qui sélectionne un capteur SPI en affirmant sa broche /SS
 */
typedef void (*vHscSpiSelect)(bool);

/**
 * Type de fonction qui initialise la broche /SS d'un capteur SPI
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
    uint8_t ucTwiAddr;
    struct {
      vHscSpiInit vInit; ///< Pointeur sur la fonction d'initialisation capteur
      vHscSpiSelect vSelect; ///< Pointeur sur la fonction de validation du capteur
    };
  };
} xHscSensor;

/* internal public functions ================================================ */
/**
 * @brief Lecture capteur
 *
 * Effectue une mesure avec iHscGetRaw() et la convertit avec
 * vHscRawToValue()
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
 * @brief Initialisation capteur HSC I2C
 *
 * Attention, il est nécessaire d'initialiser le bus I2C séparément.
 *
 * @param pSensor Pointeur sur le capteur à utiliser
 * @param dPressMin Pression minimale du capteur
 * @param dPressMax Pression maximale du capteur
 * @param ucTwiAddr Adresse I2C esclave du capteur
 * @return  HSC_SUCCESS (0) ou un code erreur eHscStatus
 */
int
iHscInitTwiSensor (xHscSensor *pSensor,  double dPressMin, double dPressMax,
                                uint8_t ucTwiAddr);
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
INLINE int
iHscInitTwiSensor (xHscSensor *pSensor,  double dPressMin, double dPressMax,
                                uint8_t ucTwiAddr) {

  pSensor->dPressMin = dPressMin;
  pSensor->dPressMax = dPressMax;
  pSensor->eBus = HSC_TWI;
  pSensor->ucTwiAddr = ucTwiAddr;
  return 0;
}
// -----------------------------------------------------------------------------
#  endif /* __DOXYGEN__ not defined */

/* ========================================================================== */
__END_C_DECLS
#endif  /* _AVRIO_HSC_H_ not defined */
