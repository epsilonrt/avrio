/**
 * @file adc_sensor.c
 * @brief Capteur linéaire analogique (Implémentation)
 * @author Copyright © 2014 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * @version $Id$
 * Revision History ---
 *    2014 - Initial version by epsilonRT
 */
#include "adc_sensor.h"

/* internal public functions ================================================ */

// -----------------------------------------------------------------------------
double
dAdcSensorRawToValue (xAdcSensor *pSensor, uint16_t usRaw) {
  // y = (x-x1)*((y2-y1)/(x2-x1))+y1
  return ((double)usRaw - pSensor->pSetting->dRawMin) *
              ((pSensor->pSetting->dValueMax - pSensor->pSetting->dValueMin) /
                (pSensor->pSetting->dRawMax - pSensor->pSetting->dRawMin)) +
                  pSensor->pSetting->dValueMin;
}

/* ========================================================================== */
