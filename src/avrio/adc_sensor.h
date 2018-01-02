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
 * @file adc_sensor.h
 * @brief Capteurs analogiques
 *
 * Revision History ------------------------------------------------------------
 *    20140312 - Initial version by epsilonRT
 */
#ifndef _AVRIO_ADC_SENSOR_H_
#define _AVRIO_ADC_SENSOR_H_
#include <avrio/defs.h>

#if defined(__cplusplus)
  extern "C" {
#endif
/* ========================================================================== */
#include <avrio/adc.h>
/**
 * @addtogroup sensor_group
 * @{
 *
 *  @defgroup adc_sensor_module Capteurs analogiques
 *  Ce module permet de gérer des capteurs analogiques à réponse linéaire et
 *  non-linéaire. \n
 *  La tension en sortie du capteur est mesurée grâce au convertisseur
 *  analogique-numérique intégré au MCU.
 *  @{
 * @example sensor/hih4000/demo_sensor_hih4000.c
 * Demo capteur analogique d'humidité (HIH4030)
 * Ce programme affiche la valeur brute en entrée ADC0 (en LSB de 0 à 1023) et
 * l'humidité correspondant à cette valeur brute sur la liaison série (UART0).
 * 
 * @example sensor/lmt87/demo_sensor_lmt87.c
 * Demo capteur analogique de température (LMT87)
 * Ce programme affiche la valeur brute en entrée ADC0 (en LSB de 0 à 1023) et
 * la température correspondant à cette valeur brute sur la liaison série (UART0).
 * 
 * @example sensor/mpx/demo_sensor_mpx.c
 * Demo capteur analogique de pression (MPX4115AP)
 * Ce programme affiche la valeur brute en entrée ADC0 (en LSB de 0 à 1023) et
 * la pression correspondant à cette valeur brute sur la liaison série (UART0).
 * 
 * @example sensor/pot/demo_sensor_pot.c
 * Demo capteur analogique angulaire (potentiomère)
 * Ce programme affiche la valeur brute en entrée ADC0 (en LSB de 0 à 1023) et
 * l'angle correspondant à cette valeur brute sur la liaison série (UART0).
 * 
 * @example sensor/rtd/demo_sensor_rtd.c
 * Demo capteur analogique de température (PT100)
 * Ce programme affiche la valeur brute en entrée ADC0 (en LSB de 0 à 1023) et
 * la température correspondant à cette valeur brute sur la liaison série (UART0).
 */

/* macros =================================================================== */
/**
 * @brief Initialisation d'un repère linéaire
 *
 * Permet d'initialiser un repère d'étalonnage de capteur linéaire
 */
#define ADC_SENSOR_SETTING_LINEAR(_vmin,_vmax,_rmin,_rmax) { \
  .xLin.dValueMin=(_vmin), \
  .xLin.dValueMax=(_vmax), \
  .xLin.dRawMin=(_rmin), \
  .xLin.dRawMax=(_rmax) }

/**
 * @brief Initialisation d'un repère non linéaire
 *
 * Permet d'initialiser un repère d'étalonnage de capteur non linéaire
 */
#define ADC_SENSOR_SETTING_NLINEAR(_coeff, _f) { \
  .xNlin.dRawToValue=(_f), \
  .xNlin.dCoeff = _coeff }

/* types ==================================================================== */
struct xAdcSensor;

/**
 * Type de capteur
 */
typedef enum {

  ADC_SENSOR_LINEAR = 0,  ///< Capteur linéaire
  ADC_SENSOR_NLINEAR = 1  ///< Capteur non linéaire
} eAdcSensorType;

/**
 * Fonction de conversion d'une valeur brute ADC en grandeur de sortie
 */
typedef double (*dAdcSensorConvert) (struct xAdcSensor *, uint16_t);

/* structures =============================================================== */
/**
 * Repère d'étalonnage d'un capteur linéaire
 */
typedef struct xAdcSensorLinearSetting {

  double dValueMin; ///< Grandeur minimale en sortie du capteur
  double dValueMax; ///< Grandeur maximale en sortie du capteur
  double dRawMin;   ///< Valeur en sortie de l'ADC associée à dValueMin
  double dRawMax;   ///< Valeur en sortie de l'ADC associée à dValueMax
} xAdcSensorLinearSetting;

/**
 * Repère d'étalonnage d'un capteur non linéaire
 */
typedef struct xAdcSensorNlinearSetting {

  double * dCoeff; ///< Tableau de coefficients pour la conversion
  dAdcSensorConvert dRawToValue;   ///< Fonction de conversion non linéaire
} xAdcSensorNlinearSetting;

/**
 * Repère d'étalonnage d'un capteur
 */
typedef union xAdcSensorSetting {

  xAdcSensorLinearSetting xLin;
  xAdcSensorNlinearSetting xNlin;
} xAdcSensorSetting;

/**
 * @brief Capteur
 *
 * Capteur connecté à une entrée de l'ADC
 */
typedef struct xAdcSensor {

  xAdcSensorSetting *pSetting;  ///< Repère d'étalonnage
  uint8_t ucAdcChan;            ///< Voie ADC du capteur
  uint8_t ucAdcScale;           ///< Echelle de la dernière mesure
  uint8_t ucTerms;              ///< Nombre de mesure pour le filtrage
  eAdcSensorType eType;         ///< Type de capteur
  eAdcFilter eFilter;           ///< Type de filtrage (Moyennage par défaut)
} xAdcSensor;

/* internal public functions ================================================ */
/**
 * @brief Lecture valeur brute ADC
 *
 * Renvoie la valeur filtrée de l'ADC, le champ ucAdcScale de pSensor
 * est mis à jour.
 *
 * @param pSensor pointeur sur le capteur à utiliser
 * @return Valeur filtrée de l'ADC
 */
uint16_t usAdcSensorGetRaw (xAdcSensor *pSensor);

/**
 * @brief Convertit une valeur ADC en grandeur capteur
 *
 * @param pSensor pointeur sur le capteur à utiliser
 * @param usRaw valeur de l'ADC correspodant à la grandeur
 * @return Grandeur capteur assoicée à usRaw
 */
double dAdcSensorRawToValue (xAdcSensor *pSensor, uint16_t usRaw);

#  if defined(__DOXYGEN__)
/*
 * __DOXYGEN__ defined
 * Partie documentation ne devant pas être compilée.
 * =============================================================================
 */
/**
 * @brief Initialisation capteur
 *
 * Attention, il est nécessaire d'initialiser l'ADC séparément.
 *
 * @param pSensor pointeur sur le capteur à utiliser
 * @param pSetting pointeur sur le repère d'étalonnage
 * @param eType Type de capteur
 * @param ucAdcChan voie ADC utilisée par le capteur
 * @param ucTerms nombre de mesures effectuées pour le filtrage, si > 1 une
 *        moyenne est effectuée, sinon c'est une mesure brute. Le champ eFilter
 *        de xAdcSensor peut être modifié pour changer de filtrage.
 */
void vAdcSensorInit (xAdcSensor *pSensor,  xAdcSensorSetting *pSetting,
                      eAdcSensorType eType,
                      uint8_t ucAdcChan, uint8_t ucTerms);

/**
 * @brief Lecture grandeur capteur
 *
 * Effectue une mesure avec usAdcSensorGetRaw() et la convertit avec
 * dAdcSensorRawToValue()
 *
 * @param pSensor pointeur sur le capteur à utiliser
 * @return Valeur moyenne de la grandeur mesurée en sortie du capteur
 */
double dAdcSensorGetValue (xAdcSensor *pSensor);

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
#include "avrio-board-adc-sensor.h"
/* internal inline functions ================================================ */

// -----------------------------------------------------------------------------
INLINE void
vAdcSensorInit (xAdcSensor *pSensor,  xAdcSensorSetting *pSetting,
                eAdcSensorType eType, uint8_t ucAdcChan, uint8_t ucTerms) {

  pSensor->pSetting = pSetting;
  pSensor->ucAdcChan = ucAdcChan;
  pSensor->eType = eType;
  pSensor->ucTerms = ucTerms;
  pSensor->eFilter = (ucTerms > 1 ? eAdcAverage : eAdcRaw);
}

// -----------------------------------------------------------------------------
INLINE double
dAdcSensorGetValue (xAdcSensor *pSensor) {

  return dAdcSensorRawToValue (pSensor, usAdcSensorGetRaw (pSensor));
}

#  endif /* __DOXYGEN__ not defined */


/* ========================================================================== */
#if defined(__cplusplus)
  }
#endif
#endif  /* _AVRIO_ADC_SENSOR_H_ not defined */
