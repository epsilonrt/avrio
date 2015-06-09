/*
 * @file avrio-board-adc-sensor.h
 * @brief Configuration ADC Sensor
 * @author Copyright © 2014 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * @version $Id$
 * Revision History ---
 *    20120519 - Initial version by epsilonRT
 */

#ifndef _AVRIO_BOARD_ADC_SENSOR_H_
#define _AVRIO_BOARD_ADC_SENSOR_H_

/* constants ================================================================ */
// -----------------------------------------------------------------------------
// HIH4030
#define ADC_SENSOR_SETTING   ADC_SENSOR_SETTING_LINEAR(0.0, 75.3, 173.2, 650.49)
#define ADC_SENSOR_TYPE      ADC_SENSOR_LINEAR
#define ADC_SENSOR_CHAN      0
#define ADC_SENSOR_UNIT      "%"
// -----------------------------------------------------------------------------
#endif  /* _AVRIO_BOARD_ADC_SENSOR_H_ not defined */
