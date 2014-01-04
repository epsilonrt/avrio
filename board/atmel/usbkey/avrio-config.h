/* Copyright © 2009-2012 epsilonRT. All rights reserved.
 *  $Id$ */
#ifndef _AVRIO_CONFIG_H_
#define _AVRIO_CONFIG_H_
/* ========================================================================== */

/* 
 * Validation des modules
 * Pour dévalider un module, mettre le define correspondant en commentaire.
 * Si le module 'moduleX' est validé, un fichier avrio-board-moduleX.h doit être
 * présent dans le même répertoire que ce fichier de configuration.
 */
//#define AVRIO_TASK_ENABLE
#define AVRIO_LED_ENABLE
#define AVRIO_BUTTON_ENABLE
#define AVRIO_LCD_ENABLE
#define AVRIO_TWI_ENABLE
#define AVRIO_ADC_ENABLE
#define AVRIO_ACCELERO3D_ENABLE
#define AVRIO_GYRO3D_ENABLE
#define AVRIO_COMPASS3D_ENABLE
//#define AVRIO_ENCODER_ENABLE

/* ========================================================================== */
#endif /* _AVRIO_CONFIG_H_ */
