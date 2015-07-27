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
 * Etalonnage 1 point avec thermomètre à thermocouple K: T0 = 24 °C pour ADC = 466 donc V0 = 2275.4 mV
 * Valeur datasheet: T0 = 30 °C pour V0 = 2230.8E-3
 */
#define LMT87_DEFAULT_COEFF {2275.4E-3, 24., 13.582E-3, 4.33E-6}

// Index des coefficients du LMT87
#define LMT87_V0  0 // Tension correspondant à T0
#define LMT87_T0  1 // Température
#define LMT87_S   2 // Sensibilité linéaire
#define LMT87_S2  3 // Sensibilté carrée

/* public variables ========================================================= */
extern double dLmt87Coeff[];

/* internal public functions ================================================ */
double dLmt87Convert (struct xAdcSensor * pSensor, uint16_t usRaw);

/* macros =================================================================== */
#define ADC_SENSOR_SETTING ADC_SENSOR_SETTING_NLINEAR(dLmt87Coeff, dLmt87Convert)

// -----------------------------------------------------------------------------
#endif  /* _AVRIO_BOARD_ADC_SENSOR_H_ not defined */
