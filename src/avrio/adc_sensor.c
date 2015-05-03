/**
 * @file adc_sensor.c
 * @brief Capteur linéaire analogique (Implémentation)
 * @author Copyright © 2014 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * Revision History ------------------------------------------------------------
 *    2014 - Initial version by epsilonRT
 */
#include "adc_sensor.h"

/* internal public functions ================================================ */

// -----------------------------------------------------------------------------
double
dAdcSensorRawToValue (xAdcSensor *pSensor, uint16_t usRaw) {

  if (pSensor->eType == ADC_SENSOR_LINEAR) {

    // y = (x-x1)*((y2-y1)/(x2-x1))+y1
    return ((double)usRaw - pSensor->pSetting->xLin.dRawMin) *
      ((pSensor->pSetting->xLin.dValueMax - pSensor->pSetting->xLin.dValueMin) /
        (pSensor->pSetting->xLin.dRawMax - pSensor->pSetting->xLin.dRawMin)) +
          pSensor->pSetting->xLin.dValueMin;
  }
  else {

    return pSensor->pSetting->xNlin.dRawToValue (pSensor, usRaw);
  }
}

// -----------------------------------------------------------------------------
uint16_t
usAdcSensorGetRaw (xAdcSensor *pSensor) {

  uint16_t usRaw = usAdcReadFilter (pSensor->ucAdcChan, pSensor->ucTerms, pSensor->eFilter);
  pSensor->ucAdcScale = ucAdcGetScale (pSensor->ucAdcChan);
  return usRaw;
}

/* ========================================================================== */
