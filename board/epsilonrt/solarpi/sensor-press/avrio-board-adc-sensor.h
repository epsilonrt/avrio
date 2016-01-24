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

// -----------------------------------------------------------------------------
// MPX4115A
#define ADC_SENSOR_SETTING  ADC_SENSOR_SETTING_LINEAR(150.0, 1150.0, 40.96, 962.56)
#define ADC_SENSOR_TYPE     ADC_SENSOR_LINEAR
#define ADC_SENSOR_CHAN      0
#define ADC_SENSOR_UNIT      "hPa"
// -----------------------------------------------------------------------------
#endif  /* _AVRIO_BOARD_ADC_SENSOR_H_ not defined */
