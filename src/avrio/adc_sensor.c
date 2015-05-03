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
 * @file adc_sensor.c
 * @brief Capteur linéaire analogique (Implémentation)
 *
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
