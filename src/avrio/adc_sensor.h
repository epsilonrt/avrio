/**
 * @file adc_sensor.h
 * @brief Capteurs analogiques
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
 *  @defgroup adc_sensor_module Capteurs analogiques
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
  uint8_t ucMeanTerms;          ///< Nombre de mesure pour le moyennage
  eAdcSensorType eType;         ///< Type de capteur
} xAdcSensor;

/* internal public functions ================================================ */
/**
 * @brief Lecture valeur brute ADC
 *
 * Renvoie la valeur moyennée en sortie de l'ADC, le champ ucAdcScale de pSensor
 * est mis à jour.
 *
 * @param pSensor pointeur sur le capteur à utiliser
 * @return Valeur moyenne mesurée en sortie de l'ADC
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
 * @param ucMeanTerms nombre de mesures effectuées pour calculer la valeur moyenne
 */
void vAdcSensorInit (xAdcSensor *pSensor,  xAdcSensorSetting *pSetting,
                      eAdcSensorType eType,
                      uint8_t ucAdcChan, uint8_t ucMeanTerms);

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
/* internal inline functions ================================================ */

// -----------------------------------------------------------------------------
INLINE void
vAdcSensorInit (xAdcSensor *pSensor,  xAdcSensorSetting *pSetting,
                eAdcSensorType eType, uint8_t ucAdcChan, uint8_t ucMeanTerms) {

  pSensor->pSetting = pSetting;
  pSensor->ucAdcChan = ucAdcChan;
  pSensor->ucMeanTerms = ucMeanTerms;
  pSensor->eType = eType;
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
