/**
 * Copyright © 2015 Pascal JEAN aka epsilonRT <pascal.jean--AT--btssn.net>
 * All rights reserved.
 */

#ifndef _AVRIO_BOARD_ADC_SENSOR_H_
#define _AVRIO_BOARD_ADC_SENSOR_H_

/* constants ================================================================ */

// -----------------------------------------------------------------------------
// RTD
#define ADC_SENSOR_SETTING ADC_SENSOR_SETTING_LINEAR(-60.0, 60.0, 606.63, 971.07)
#define ADC_SENSOR_TYPE    ADC_SENSOR_LINEAR
#define ADC_SENSOR_CHAN    0
#define ADC_SENSOR_UNIT    "°C"

// -----------------------------------------------------------------------------
#endif  /* _AVRIO_BOARD_ADC_SENSOR_H_ not defined */
