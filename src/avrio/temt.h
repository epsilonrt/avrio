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
 * @file temt.h
 * @brief Capteur de luminosité Honeywell TEMT
 * 
 * Revision History ---
 *    20140312 - Initial version by epsilonRT
 */
#ifndef _AVRIO_TEMT_H_
#define _AVRIO_TEMT_H_
#include <avrio/adc_sensor.h>

__BEGIN_C_DECLS
/* ========================================================================== */

/**
 * @addtogroup sensor_group
 * @{
 *
 *  @defgroup temt_module Capteur de luminosité TEMT6000
 *  Ce module permet de gérer des capteurs de luminosité Vishay TEMT6000.
 *  Les capteurs TEMT sont des phototransistors ayant un courant proportionnel
 *  à l'éclairement lumineux en lux. \n
 *  Ce module utilise l'ADC pour effectuer la mesure et compense la mesure de la
 *  température.
 *  @warning Seul les MCU disposants d'un capteur de température permettent une
 *  correction de la mesure.
 *  @{
 *    @example temt/demo_temt.c
 *    Exemple qui affiche la luminosité et la température sur la liaison série.
 */

/* types ==================================================================== */
typedef xAdcSensorSetting xTemtSetting;

/* structures =============================================================== */
/**
 * @brief Données brutes lues dans un capteur
 */
typedef struct xTemtRaw {

  uint16_t usLum;  ///< Valeur brute ADC de l'éclairement lumineux
  uint16_t usTemp; ///< Valeur brute ADC de la température
} xTemtRaw;

/**
 * @brief Valeurs calculées de la luminosité et de la température
 */
typedef struct xTemtValue {

  double dLum;  ///< Eclairement lumineux en lux
  double dTemp; ///< Température exprimée en °C
} xTemtValue;


/**
 * @brief Capteur TEMT
 */
typedef struct xTemtSensor {

  xAdcSensor xLumSensor;
  xTemtSetting *pLumSettings;
  xAdcSensor xTempSensor;
} xTemtSensor;

/* internal public functions ================================================ */
/**
 * @brief Initialisation capteur
 *
 * Attention, il est nécessaire d'initialiser l'ADC avant.
 *
 * @param pSensor pointeur sur le capteur à utiliser
 * @param pSetting pointeur sur le repère d'étalonnage
 * @param ucAdcChan voie ADC utilisée par le capteur
 */
void vTemtInit ( xTemtSensor *pSensor,
                  uint8_t ucAdcLumChan,  xTemtSetting *pLumSettings,
                  uint8_t ucAdcTempChan, xTemtSetting *pTempSetting);

/**
 * @brief Lecture valeurs brutes
 *
 * Renvoie les valeurs brutes en sortie du capteur
 *
 * @param pSensor Pointeur sur le capteur à utiliser
 * @param pRaw Valeurs brutes correspodant aux grandeurs mesurées
 */
void vTemtGetRaw (xTemtSensor *pSensor, xTemtRaw *pRaw);

/**
 * @brief Convertit des valeurs brutes en grandeurs capteur
 *
 * @param pSensor Pointeur sur le capteur à utiliser
 * @param pRaw Valeurs brutes lues en sortie du capteur
 * @param pValue Valeur des grandeurs mesurées (luminosité et température)
 *        associées à pRaw
 */
void vTemtRawToValue (xTemtSensor *pSensor, const xTemtRaw *pRaw, xTemtValue *pValue);

/**
 * @brief Lecture capteur
 *
 * Effectue une mesure avec usAdcSensorGetRaw() et la convertit avec
 * dAdcSensorRawToValue()
 *
 * @param pSensor Pointeur sur le capteur à utiliser
 * @param pValue Valeur des grandeurs mesurées (luminosité et température)
 */
void vTemtGetValue (xTemtSensor *pSensor, xTemtValue *pValue);

#  if defined(__DOXYGEN__)
/*
 * __DOXYGEN__ defined
 * Partie documentation ne devant pas être compilée.
 * =============================================================================
 */

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
#  endif /* __DOXYGEN__ not defined */

/* ========================================================================== */
__END_C_DECLS
#endif  /* _AVRIO_TEMT_H_ not defined */
