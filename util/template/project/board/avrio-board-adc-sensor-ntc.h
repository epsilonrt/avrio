/*
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
 * Coefficients de Steinhart-Hart de la NTC utilisée
 */
#define NTC_COEFF NTC_NXFT15_10K

/* private functions ======================================================== */
/*
 * Renvoie la température en fonction de la valeur ADC
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
