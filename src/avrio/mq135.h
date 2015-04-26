/**
 * @file mq135.h
 * @brief Capteur de qualité de l'air MQ135
 * @author Copyright © 2015 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 */
#ifndef _AVRIO_MQ135_H_
#define _AVRIO_MQ135_H_
#include <math.h>
#include <avrio/defs.h>
#include <avrio/adc_sensor.h>

/* *INDENT-OFF* */
__BEGIN_C_DECLS
/* ========================================================================== */
/**
 * @addtogroup adc_sensor_module
 * @{
 *
 *  @defgroup mq135_module Capteur MQ135
 *  Ce module permet de gérer des capteurs MQ135 permettant de mesurer la
 *  qualité de l'air.
 *  @{
 *    @example adc_sensor/mq135/demo_mq135.c
 *    Exemple qui affiche la teneur en CO² de l'atmosphère sur la liaison série.
 */

/* structures =============================================================== */
/**
 * Repère d'étalonnage d'un capteur MQ135
 */
typedef struct xMq135Calibration {

  double dRLoad; ///< Résistance de charge en sortie en kilo Ohms
  // -------- Paramètres statiques correspondants à l'étalonnage
  // Teneur CO² actualisée sur http://www.esrl.noaa.gov/gmd/ccgg/trends/global.html
  double dRZero; ///< Résistance correspondant à la teneur de CO² standard en kilo Ohms
  double dAtmoCO2; ///< Teneur en CO² standard
  // -------- Paramètres dynamiques variants pendant le fonctionnement
  double dTemp; ///< Température en °C pour la correction
  double dHum; ///< Humidité relative en % pour la correction
} xMq135Calibration;

/* internal public functions ================================================ */

/**
 * @brief Convertit une valeur ADC en résistance Rs
 *
 * Si les paramètres température et humidité ont été réglés avec vMq135SetTemp(),
 * vMq135SetHumidity() ou vMq135SetCorrection() la valeur renvoyée est corrigée.
 * Pour invalider la correction, il faut remettre les 2 paramètres à NAN.
 *
 * @param pSensor pointeur sur le capteur à utiliser
 * @param usRaw valeur de l'ADC correspodant à la grandeur
 * @return Résistance Rs associée à usRaw
 */
double dMq135RawToResistance (xAdcSensor * pSensor, uint16_t usRaw);

/**
 * @brief Convertit une valeur ADC en teneur en gaz en ppm
 *
 * Si les paramètres température et humidité ont été réglés avec vMq135SetTemp(),
 * vMq135SetHumidity() ou vMq135SetCorrection() la valeur renvoyée est corrigée.
 * Pour invalider la correction, il faut remettre les 2 paramètres à NAN.
 *
 * @param pSensor pointeur sur le capteur à utiliser
 * @param usRaw valeur de l'ADC correspodant à la grandeur
 * @return Teneur en ppm associée à usRaw
 */
double dMq135RawToPPM (xAdcSensor * pSensor, uint16_t usRaw);

/**
 * @brief Convertit une valeur ADC en résistance Rs avec correction
 *
 * Cette fonction invalide la correction interne à dMq135RawToPPM() en mettant
 * les paramètres du capteur à NAN et renvoie une valeur corrigée avec les
 * paramètres dTemp et dHum fournis.
 *
 * @param pSensor pointeur sur le capteur à utiliser
 * @param usRaw valeur de l'ADC correspodant à la grandeur
 * @param dTemp température en °C
 * @param dHum humidité relative en %
 * @return Résistance Rs associée à usRaw
 */
double dMq135RawToCorrectedResistance (xAdcSensor * pSensor, uint16_t usRaw, double dTemp, double dHum);

/**
 * @brief Convertit une valeur ADC en teneur en gaz en ppm avec correction
 *
 * Cette fonction invalide la correction interne à dMq135RawToPPM() en mettant
 * les paramètres du capteur à NAN et renvoie une valeur corrigée avec les
 * paramètres dTemp et dHum fournis.
 *
 * @param pSensor pointeur sur le capteur à utiliser
 * @param usRaw valeur de l'ADC correspodant à la grandeur
 * @param dTemp température en °C
 * @param dHum humidité relative en %
 * @return Teneur en ppm associée à usRaw
 */
double dMq135RawToCorrectedPPM (xAdcSensor * pSensor, uint16_t usRaw, double dTemp, double dHum);

/**
 * @brief Mesure de la résistance Ro
 *
 * Cette fonction permet de calculer la résistance d'étalonnage correspondant à
 * la teneur en CO² standard. \n
 * Pour effectuer l'étalonnage:
 * - la teneur en CO² standard doit être actualisée sur
 *   http://www.esrl.noaa.gov/gmd/ccgg/trends/global.html
 * - le capteur doit être démarré depuis au moins 24 heures
 * - le capteur doit mis à l'air libre pendant au moins 1 heure
 *    (température 20°C, humidité 65%).
 * .
 *
 * @param pSensor pointeur sur le capteur à utiliser
 * @return Résistance Ro associée à dAtmoCO2
 */
double dMq135CalibrateRZero (xAdcSensor * pSensor);

/**
 * @brief Calcul du facteur de correction de la résistance
 *
 * @param dTemp température en °C
 * @param dHum humidité relative en %
 * @return Facteur de correction de la résistance du capteur (Rcor=Rs/Cor)
 */
double dMq135CorrectionFactor (double dTemp, double dHum);

#  if defined(__DOXYGEN__)
/*
 * __DOXYGEN__ defined
 * Partie documentation ne devant pas être compilée.
 * =============================================================================
 */

/* macros =================================================================== */
/**
 * @brief Initialisation d'un repère d'étalonnage d'un capteur MQ135
 *
 * @param rload Résistance de charge en sortie en kilo ohms
 * @param rzero Résistance correspondant à la teneur de CO² standard en kilo Ohms
 * @param atmoco2 Teneur en CO² standard
 *        (à actualiser sur http://www.esrl.noaa.gov/gmd/ccgg/trends/global.html)
 */
#define MQ135_CALIBRATION(rload,rzero,atmoco2)

/**
 * @brief Initialise le capteur à partir de l'étalonnage
 *
 * dAdcSensorGetValue() est initialisée pour fournir une valeur de teneur en gaz en ppm
 *
 * @param pSetting pointeur sur le réglage du cpateur
 * @param pCalibration pointeur sur l'étalonnage
 */
static inline void vMq135Init (xAdcSensorSetting *pSetting, xMq135Calibration * pCalibration);

/**
 * @brief Modifie la température utilisée pour la correction interne de la mesure
 *
 * Si une température et une humidité est fournie, la mesure est corrigée avec
 * ces valeurs, sinon aucune correction n'est effctuée.
 *
 * @param pSensor pointeur sur le capteur à utiliser
 * @param dTemp température en °C
 */
static inline void vMq135SetTemp (xAdcSensor *pSensor, double dTemp);

/**
 * @brief Modifie l'humidité utilisée pour la correction interne de la mesure
 *
 * Si une température et une humidité est fournie, la mesure est corrigée avec
 * ces valeurs, sinon aucune correction n'est effctuée.
 *
 * @param pSensor pointeur sur le capteur à utiliser
 * @param dHum humidité relative en %
 */
static inline void vMq135SetHumidity (xAdcSensor *pSensor, double dHum);

/**
 * @brief Modifie la température et l'humidité utilisée pour la correction interne de la mesure
 *
 * Si une température et une humidité est fournie, la mesure est corrigée avec
 * ces valeurs, sinon aucune correction n'est effctuée.
 *
 * @param pSensor pointeur sur le capteur à utiliser
 * @param dTemp température en °C
 * @param dHum humidité relative en %
 */
static inline void vMq135SetCorrection (xAdcSensor *pSensor, double dTemp, double dHum);

/**
 * @brief Lecture de la température utilisée pour la correction interne de la mesure
 *
 * Cette valeur est à NaN par défaut après l'initialisation.
 *
 * @param pSensor pointeur sur le capteur à utiliser
 * @return température en °C
 */
static inline void dMq135Temp (xAdcSensor *pSensor);

/**
 * @brief Lecture de l'humidité utilisée pour la correction interne de la mesure
 *
 * Cette valeur est à NaN par défaut après l'initialisation.
 *
 * @param pSensor pointeur sur le capteur à utiliser
 * @return humidité relative en %
 */
static inline void dMq135Humidity (xAdcSensor *pSensor);

/**
 * @brief Lecture de la résistance d'étalonnage
 *
 * Cette valeur correspond à la résistance calculée lors de l'étalonnage à la
 * teneur de CO² standard
 *
 * @param pSensor pointeur sur le capteur à utiliser
 * @return résistance Ro en kilo Ohms
 */
static inline void dMq135RZero (xAdcSensor *pSensor);

/**
 * @brief Lecture de la résistance de charge
 *
 * Cette valeur correspond à la résistance branchée en sortie du capteur.
 *
 * @param pSensor pointeur sur le capteur à utiliser
 * @return résistance RL en kilo Ohms
 */
static inline void dMq135RLoad (xAdcSensor *pSensor);

/**
 * @brief Lecture de la teneur en CO² d'étalonnage
 *
 * Cette valeur correspond à la teneur définie lors de l'étalonnage
 *
 * @param pSensor pointeur sur le capteur à utiliser
 * @return Teneur en CO² en ppm
 */
static inline void dMq135AtmoCO2 (xAdcSensor *pSensor);

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

#define MQ135_CALIBRATION(_rload,_rzero,_atmoco2) { \
    .dRLoad=(_rload), .dRZero=(_rzero), .dAtmoCO2=(_atmoco2), \
    .dTemp=NAN, .dHum=NAN }

// -----------------------------------------------------------------------------
INLINE void
vMq135Init (xAdcSensorSetting *pSetting,
                      xMq135Calibration * pCalibration) {

  pSetting->xNlin.dRawToValue = dMq135RawToPPM;
  pSetting->xNlin.dCoeff = (double *) pCalibration;
}

// -----------------------------------------------------------------------------
INLINE void
vMq135SetTemp (xAdcSensor *pSensor, double dTemp) {
  xMq135Calibration * pCalibration =
    (xMq135Calibration *) pSensor->pSetting->xNlin.dCoeff;
  pCalibration->dTemp = dTemp;
}

// -----------------------------------------------------------------------------
INLINE void
vMq135SetCorrection (xAdcSensor *pSensor, double dTemp, double dHum) {
  xMq135Calibration * pCalibration =
    (xMq135Calibration *) pSensor->pSetting->xNlin.dCoeff;
  pCalibration->dTemp = dTemp;
  pCalibration->dHum = dHum;
}

// -----------------------------------------------------------------------------
INLINE double
dMq135Temp (xAdcSensor *pSensor) {
  xMq135Calibration * pCalibration =
    (xMq135Calibration *) pSensor->pSetting->xNlin.dCoeff;
  return pCalibration->dTemp;
}

// -----------------------------------------------------------------------------
INLINE void
vMq135SetHumidity (xAdcSensor *pSensor, double dHum) {
  xMq135Calibration * pCalibration =
    (xMq135Calibration *) pSensor->pSetting->xNlin.dCoeff;
  pCalibration->dHum = dHum;
}

// -----------------------------------------------------------------------------
INLINE double
dMq135Humidity (xAdcSensor *pSensor) {
  xMq135Calibration * pCalibration =
    (xMq135Calibration *) pSensor->pSetting->xNlin.dCoeff;
  return pCalibration->dHum;
}

// -----------------------------------------------------------------------------
INLINE double
dMq135RZero (xAdcSensor *pSensor) {
  xMq135Calibration * pCalibration =
    (xMq135Calibration *) pSensor->pSetting->xNlin.dCoeff;
  return pCalibration->dRZero;
}

// -----------------------------------------------------------------------------
INLINE double
dMq135RLoad (xAdcSensor *pSensor) {
  xMq135Calibration * pCalibration =
    (xMq135Calibration *) pSensor->pSetting->xNlin.dCoeff;
  return pCalibration->dRLoad;
}

// -----------------------------------------------------------------------------
INLINE double
dMq135AtmoCO2 (xAdcSensor *pSensor) {
  xMq135Calibration * pCalibration =
    (xMq135Calibration *) pSensor->pSetting->xNlin.dCoeff;
  return pCalibration->dAtmoCO2;
}
#  endif /* __DOXYGEN__ not defined */

/* ========================================================================== */
__END_C_DECLS
/* *INDENT-ON* */
#endif  /* _AVRIO_MQ135_H_ not defined */
