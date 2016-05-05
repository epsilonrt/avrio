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
