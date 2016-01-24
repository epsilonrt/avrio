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
#define AVRIO_ENCODER_ENABLE

/* ========================================================================== */
#endif /* _AVRIO_CONFIG_H_ */