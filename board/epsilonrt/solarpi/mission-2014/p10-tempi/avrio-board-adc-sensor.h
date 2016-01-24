/*
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
 */
#ifndef _AVRIO_BOARD_ADC_SENSOR_H_
#define _AVRIO_BOARD_ADC_SENSOR_H_

// -----------------------------------------------------------------------------
// NTC
#include "ntc.h"

/* constants ================================================================ */
#define ADC_SENSOR_TYPE    ADC_SENSOR_NLINEAR
#define ADC_SENSOR_CHAN      0

#define IREF 100E-6
#define VREF 4.95
#define ADC_N 1024.0
#define R3 4.87E3
#define R5 3.01E3
#define R6 26.7E3
#define R7 23.7E3

/*
 * NTC Murata NXFT15 10k
 */
#define NTC_NXFT15_10K { \
  9.310296797541951e-04, \
  2.308343095769287e-04, \
  3.001370069362199e-06, \
  5.407975166655454e-08 }

/*
 * NTC AVX NJ28MA0302F - 3k
 */
#define NTC_NJ28_3K {    \
  1.384458976342609e-03, \
  2.393452650459891e-04, \
  4.184121390081160e-07, \
  5.134115012343303e-08 }

/*
 * Coefficients de Steinhart-Hart de la NTC utilis�e
 */
#define NTC_COEFF NTC_NXFT15_10K

/* private functions ======================================================== */
/*
 * Renvoie la temp�rature en fonction de la valeur ADC
 */
static double
dNtcConvert (struct xAdcSensor * pSensor, uint16_t usRaw) {
  double dR;

  dR = ((double)usRaw * VREF * R6) / (ADC_N * IREF * (R6 + R7)) +
        (R3 * R7) / (R6 + R7) - R5;

  return dNtcResToTemp (dR, pSensor->pSetting->xNlin.dCoeff);
}

// -----------------------------------------------------------------------------
#endif  /* _AVRIO_BOARD_ADC_SENSOR_H_ not defined */
