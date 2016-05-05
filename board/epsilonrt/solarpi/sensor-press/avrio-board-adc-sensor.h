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

// -----------------------------------------------------------------------------
// MPX4115A
#define ADC_SENSOR_SETTING  ADC_SENSOR_SETTING_LINEAR(150.0, 1150.0, 40.96, 962.56)
#define ADC_SENSOR_TYPE     ADC_SENSOR_LINEAR
#define ADC_SENSOR_CHAN      0
#define ADC_SENSOR_UNIT      "hPa"
// -----------------------------------------------------------------------------
#endif  /* _AVRIO_BOARD_ADC_SENSOR_H_ not defined */
