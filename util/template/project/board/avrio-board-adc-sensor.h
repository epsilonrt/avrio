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

/* constants ================================================================ */
#define ADC_SENSOR_TYPE    ADC_SENSOR_NLINEAR
#define ADC_SENSOR_CHAN    0
#define ADC_SENSOR_UNIT    "°C"

/*
 * coefficients par défaut du LMT87
 * Etalonnage 1 point avec thermomètre à thermocouple K: 
 * T0 = 24 °C pour ADC = 466 donc V0 = 2275.4 mV
 * Valeur datasheet: T0 = 30 °C pour V0 = 2230.8E-3
 */
//#define LMT87_DEFAULT_COEFF {2275.4E-3, 24., 13.582E-3, 4.33E-6, ADC0_FULLSCALE }
#define LMT87_DEFAULT_COEFF {2323E-3, 22., 13.582E-3, 4.33E-6, ADC0_FULLSCALE }

// -----------------------------------------------------------------------------
#endif  /* _AVRIO_BOARD_ADC_SENSOR_H_ not defined */
