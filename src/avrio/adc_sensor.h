/**
 * @file adc_sensor.h
 * @brief Capteur linéaire analogique
 * @author Copyright © 2014 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * @version $Id$
 * Revision History ---
 *    20140312 - Initial version by epsilonRT
 */
#ifndef _AVRIO_ADC_SENSOR_H_
#define _AVRIO_ADC_SENSOR_H_
#include <avrio/defs.h>

/* *INDENT-OFF* */
__BEGIN_C_DECLS
/* ========================================================================== */
#include <avrio/adc.h>
/**
 * @addtogroup dev_group
 * @{
 *
 *  @defgroup adc_sensor_module Capteur linéaire sur ADC
 *  Ce module permet de gérer des capteurs analogiques à réponse linéaire.
 *  La tension en sortie du capteur est mesurée grâce au convertisseur
 *  analogique-numérique intégré au MCU.
 *  @{
 *    @example adc_sensor/demo_adc_sensor.c
 *    Exemple qui affiche l'angle correspondant à un potentiomètre monotour
 *    sur la liaison série.
 */

/* macros =================================================================== */
/**
 * @brief Initialisation d'une variable xAdcSensorSetting
 *
 * Permet d'initialiser un repère de calage capteur
 */
#define ADC_SENSOR_SETTING(_vmin,_vmax,_rmin,_rmax) { \
  .dValueMin=(_vmin), \
  .dValueMax=(_vmax), \
  .dRawMin=(_rmin), \
  .dRawMax=(_rmax) }

/* structures =============================================================== */
/**
 * Repère de calage d'un capteur
 */
typedef struct xAdcSensorSetting {

  double dValueMin; ///< Grandeur minimale en sortie du capteur
  double dValueMax; ///< Grandeur maximale en sortie du capteur
  double dRawMin;   ///< Valeur en sortie de l'ADC associée à dValueMin
  double dRawMax;   ///< Valeur en sortie de l'ADC associée à dValueMax
} xAdcSensorSetting;

/**
 * Capteur linéaire connecté à une entrée de l'ADC
 */
typedef struct xAdcSensor {

  xAdcSensorSetting *pSetting;  ///< Repère de calage
  uint8_t ucAdcChan;            ///< Voie ADC du cpateur
  uint8_t ucMeanTerms;          ///< Nombre de mesure pour le moyennage
} xAdcSensor;

/* internal public functions ================================================ */
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
 * Initialisation capteur
 *
 * Attention, il est nécessaire d'initialiser l'ADC séparément.
 *
 * @param pSensor pointeur sur le capteur à utiliser
 * @param pSetting pointeur sur le repère de calage
 * @param ucAdcChan voie ADC utilisée par le capteur
 * @param ucMeanTerms nombre de mesures effectuées pour calculer la valeur moyenne
 */
void vAdcSensorInit (xAdcSensor *pSensor,  xAdcSensorSetting *pSetting,
                               uint8_t ucAdcChan, uint8_t ucMeanTerms);

/**
 * Lecture valeur brute ADC
 *
 * Renvoie la valeur moyennée en sortie de l'ADC
 *
 * @param pSensor pointeur sur le capteur à utiliser
 * @return Valeur moyenne mesurée en sortie de l'ADC
 */
uint16_t usAdcSensorGetRaw (xAdcSensor *pSensor);

/**
 * Lecture grandeur capteur
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
/* internal inline functions ================================================ */

// -----------------------------------------------------------------------------
INLINE void
vAdcSensorInit (xAdcSensor *pSensor,  xAdcSensorSetting *pSetting,
                               uint8_t ucAdcChan, uint8_t ucMeanTerms) {

  pSensor->pSetting = pSetting;
  pSensor->ucAdcChan = ucAdcChan;
  pSensor->ucMeanTerms = ucMeanTerms;
}

// -----------------------------------------------------------------------------
INLINE uint16_t
usAdcSensorGetRaw (xAdcSensor *pSensor) {

  return usAdcReadAverage (pSensor->ucAdcChan, pSensor->ucMeanTerms);
}

// -----------------------------------------------------------------------------
INLINE double
dAdcSensorGetValue (xAdcSensor *pSensor) {

  return dAdcSensorRawToValue (pSensor, usAdcSensorGetRaw (pSensor));
}

#  endif /* __DOXYGEN__ not defined */


/* ========================================================================== */
__END_C_DECLS
/* *INDENT-ON* */
#endif  /* _AVRIO_ADC_SENSOR_H_ not defined */
