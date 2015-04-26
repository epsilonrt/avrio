/**
 * @file mq135.c
 * @brief Capteur de qualité de l'air MQ135 (Implémentation)
 * @author Copyright © 2014 G.Krocker (Mad Frog Labs). All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 */
#include "mq135.h"

/* constants ================================================================ */
#ifndef ADC_VREF
#warning ADC_VREF has been set to the default value ! (5.0 V)
#define ADC_VREF 5.0
#endif

// Parameters for calculating ppm of CO2 from sensor resistance
#ifndef MQ135_PARA
#define MQ135_PARA 116.6020682
#endif
#ifndef MQ135_PARB
#define MQ135_PARB 2.769034857
#endif

// Parameters to model temperature and humidity dependence
#ifndef MQ135_CORA
#define MQ135_CORA 0.00035
#endif
#ifndef MQ135_CORB
#define MQ135_CORB 0.02718
#endif
#ifndef MQ135_CORC
#define MQ135_CORC 1.39538
#endif
#ifndef MQ135_CORD
#define MQ135_CORD 0.0018
#endif

/* internal public functions ================================================ */

/* -----------------------------------------------------------------------------
  @brief  Get the ppm of CO2 sensed (assuming only CO2 in the air)
  @return The ppm of CO2 in the air
*/
double
dMq135RawToPPM (xAdcSensor *pSensor, uint16_t usRaw) {

  return MQ135_PARA *
         pow ( (dMq135RawToResistance (pSensor, usRaw) / dMq135RZero (pSensor)), -MQ135_PARB);
}

/* -----------------------------------------------------------------------------
  @brief  Get the resistance of the sensor, ie. the measurement value
  @return The sensor resistance in kOhm
*/
double
dMq135RawToResistance (xAdcSensor *pSensor, uint16_t usRaw) {
  double dRes;

  dRes = ( (1023. / (double) usRaw) * ADC_VREF - 1.) * dMq135RLoad (pSensor);
  if (!isnan (dMq135Temp (pSensor)) && !isnan (dMq135Humidity (pSensor))) {
    xMq135Calibration * pCalibration =
      (xMq135Calibration *) pSensor->pSetting->xNlin.dCoeff;

    dRes /= dMq135CorrectionFactor (pCalibration->dTemp, pCalibration->dHum);
  }
  return dRes;
}

// -----------------------------------------------------------------------------
double
dMq135RawToCorrectedResistance (xAdcSensor * pSensor, uint16_t usRaw, double dTemp, double dHum) {

  vMq135SetCorrection (pSensor, NAN, NAN);
  return dMq135RawToResistance (pSensor, usRaw) / dMq135CorrectionFactor (dTemp, dHum);
}

// -----------------------------------------------------------------------------
double
dMq135RawToCorrectedPPM (xAdcSensor *pSensor, uint16_t usRaw, double dTemp, double dHum) {

  return MQ135_PARA *
         pow ( (dMq135RawToCorrectedResistance (pSensor, usRaw, dTemp, dHum) / dMq135RZero (pSensor)), -MQ135_PARB);
}

// -----------------------------------------------------------------------------
double
dMq135CalibrateRZero (xAdcSensor *pSensor) {
  uint16_t usRaw = usAdcSensorGetRaw (pSensor);

  return dMq135RawToResistance (pSensor, usRaw) *
         pow ( (dMq135AtmoCO2 (pSensor) / MQ135_PARA), (1. / MQ135_PARB));
}

/* -----------------------------------------------------------------------------
  @brief  Get the correction factor to correct for temperature and humidity
  @param[in] t  The ambient air temperature
  @param[in] h  The relative humidity
  @return The calculated correction factor
*/
double
dMq135CorrectionFactor (double dTemp, double dHum) {

  return MQ135_CORA * dTemp * dTemp -
         MQ135_CORB * dTemp +
         MQ135_CORC -
         MQ135_CORD * (dHum - 33.);
}

/* ========================================================================== */
