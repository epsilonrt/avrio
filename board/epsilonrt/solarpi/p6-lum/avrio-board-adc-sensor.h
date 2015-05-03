/*
 * @file avrio-board-adc-sensor.h
 * @brief Configuration ADC Sensor
 * @author Copyright � 2014 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * Revision History ------------------------------------------------------------
 *    20120519 - Initial version by epsilonRT
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
