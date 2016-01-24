#ifndef _CONFIG_H_
#define _CONFIG_H_
#include <avrio/defs.h>

/* *INDENT-OFF* */
__BEGIN_C_DECLS
/* ========================================================================== */
#include "avrio-board-adc-sensor.h"

/* constants ================================================================ */

/*
 * Etalonnage par défaut du capteur TEMT
 * Eléments calculés à partir des données d'étalonnage fournies par le
 * fabricant
 */
#define CFG_SENSOR_DEFAULT_SETTING  { TEMT_LUM_SETTINGS, TEMT_TEMP_SETTING }

/*
 * Voies ADC
 */
#define CFG_SENSOR_LUM_CHAN 0
#define CFG_SENSOR_SOL_CHAN 1
#define CFG_SENSOR_BAT_CHAN 2
#define CFG_SENSOR_TEMP_CHAN 8

/*
 * Nombre de termes de moyennage capteur TEMT
 */
#define CFG_TEMT_AVERAGE_TERMS 64

/*
 * Gradient de dL/dT du capteur TEMT en %
 */
#define CFG_TEMT_TKIPCE 1.18

/*
 * Utilisation du capteur de température interne au MCU
 */
#define CFG_TEMT_TEMP_INTERNAL

/* ========================================================================== */
__END_C_DECLS
/* *INDENT-ON* */
#endif  /* _CONFIG_H_ not defined */
