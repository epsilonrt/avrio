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
 * @file temt.c
 * @brief Capteur de luminosité Vishay TEMT6000 (Implémentation)
 * 
 * Revision History ---
 *    2014 - Initial version by epsilonRT
 */
#include "temt.h"
#include "avrio-board-temt.h"

/* internal public functions ================================================ */

// -----------------------------------------------------------------------------
void
vTemtInit ( xTemtSensor *pSensor,
            uint8_t ucAdcLumChan,  xTemtSetting *pLumSettings,
            uint8_t ucAdcTempChan, xTemtSetting *pTempSetting) {

  pSensor->pLumSettings = pLumSettings;
  vAdcSensorInit (&pSensor->xLumSensor, &pLumSettings[0], ADC_SENSOR_LINEAR,
                  ucAdcLumChan, CFG_TEMT_AVERAGE_TERMS);
  if (pTempSetting)
    vAdcSensorInit (&pSensor->xTempSensor, pTempSetting, ADC_SENSOR_LINEAR,
                    ucAdcTempChan, CFG_TEMT_AVERAGE_TERMS);
}

// -----------------------------------------------------------------------------
void
vTemtGetRaw (xTemtSensor *pSensor, xTemtRaw *pRaw) {

  pRaw->usLum = usAdcSensorGetRaw (&pSensor->xLumSensor);
  if (pSensor->xTempSensor.pSetting) {
#ifdef CFG_TEMT_TEMP_INTERNAL
    eAdcRef eRef = eAdcGetRef();

    if (eRef != eAdcInternal) {

      vAdcSetRef (eAdcInternal);
      pRaw->usTemp = usAdcSensorGetRaw (&pSensor->xTempSensor);
      vAdcSetRef (eRef);
    }
    else
      pRaw->usTemp = usAdcSensorGetRaw (&pSensor->xTempSensor);
#else
    pRaw->usTemp = usAdcSensorGetRaw (&pSensor->xTempSensor);
#endif
  }
}

// -----------------------------------------------------------------------------
void
vTemtRawToValue (xTemtSensor *pSensor, const xTemtRaw *pRaw, xTemtValue *pValue) {

  // Sélectionne le repère d'étalonnage correspondant à l'échelle utilisée pour
  // la mesure brute
  pSensor->xLumSensor.pSetting = &pSensor->pLumSettings[pSensor->xLumSensor.ucAdcScale];

  pValue->dLum  = dAdcSensorRawToValue (&pSensor->xLumSensor, pRaw->usLum);
  if (pSensor->xTempSensor.pSetting) {
    double dT, dL;

    pValue->dTemp = dAdcSensorRawToValue (&pSensor->xTempSensor, pRaw->usTemp);
    // Correction de l'éclairement en fonction de la température
    dT = 25.0 - pValue->dTemp;
    dL = pValue->dLum * dT * CFG_TEMT_TKIPCE / 100.0;
    pValue->dLum += dL;
  }
}

// -----------------------------------------------------------------------------
void
vTemtGetValue (xTemtSensor *pSensor, xTemtValue *pValue) {
  xTemtRaw xRaw;

  vTemtGetRaw (pSensor, &xRaw);
  vTemtRawToValue (pSensor, &xRaw, pValue);
}

/* ========================================================================== */
