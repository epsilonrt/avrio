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
