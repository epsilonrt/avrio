/**
 * Copyright © 2015 Pascal JEAN aka epsilonRT <pascal.jean--AT--btssn.net>
 * All rights reserved.
 * This software is governed by the CeCILL license <http://www.cecill.info>
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
#define LMT87_DEFAULT_COEFF {2275.4E-3, 24., 13.582E-3, 4.33E-6, 5.0 }

// -----------------------------------------------------------------------------
#endif  /* _AVRIO_BOARD_ADC_SENSOR_H_ not defined */
