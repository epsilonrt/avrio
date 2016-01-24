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

#define TEMT_LUM_SETTING0 ADC_SENSOR_SETTING_LINEAR(20.0, 200.0, 102.4, 1024.0)
#define TEMT_LUM_SETTING1 ADC_SENSOR_SETTING_LINEAR(40.0, 400.0, 102.4, 1024.0)
#define TEMT_LUM_SETTING2 ADC_SENSOR_SETTING_LINEAR(400.0, 4000.0, 102.4, 1024.0)
#define TEMT_LUM_SETTING3 ADC_SENSOR_SETTING_LINEAR(4000.0, 40000.0, 102.4, 1024.0)
#define TEMT_LUM_SETTINGS {TEMT_LUM_SETTING0, TEMT_LUM_SETTING1, TEMT_LUM_SETTING2, TEMT_LUM_SETTING3}

#define TEMT_TEMP_SETTING ADC_SENSOR_SETTING_LINEAR(-39.0, 85.0, 282.76, 411.23)
//#define TEMT_TEMP_SETTING ADC_SENSOR_SETTING_LINEAR(-45.0, 85.0, 225.28, 353.75)

#define ADC_SENSOR_SETTING TEMT_LUM_SETTING0  // Pour test seuleument
#define ADC_SENSOR_CHAN    0 // Pour test seuleument: 0 lum, 8 temp
#define ADC_SENSOR_TYPE    ADC_SENSOR_LINEAR

// -----------------------------------------------------------------------------
#endif  /* _AVRIO_BOARD_ADC_SENSOR_H_ not defined */
