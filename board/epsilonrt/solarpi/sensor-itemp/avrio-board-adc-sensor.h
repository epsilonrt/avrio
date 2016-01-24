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
