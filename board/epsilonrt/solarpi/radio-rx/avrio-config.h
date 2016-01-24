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
//#define AVRIO_CPU_FREQ F_CPU
//#define AVRIO_TASK_ENABLE
#define AVRIO_LED_ENABLE
//#define AVRIO_BUTTON_ENABLE
//#define AVRIO_LCD_ENABLE
//#define AVRIO_LCD_BACKLIGHT_ENABLE
//#define AVRIO_KEYB_ENABLE
//#define AVRIO_MELODY_ENABLE
#define AVRIO_SERIAL_ENABLE
#ifndef AVRIO_SERIAL_FLAVOUR
#define AVRIO_SERIAL_FLAVOUR SERIAL_FLAVOUR_POLL
#endif
//#define AVRIO_MODBUS_ENABLE
//#define AVRIO_PHONE_ENABLE
//#define AVRIO_BDCM_ENABLE
//#define AVRIO_ENCODER_ENABLE
//#define AVRIO_CAN_ENABLE
//#define AVRIO_SPI_ENABLE
//#define AVRIO_TWI_ENABLE
#define AVRIO_ADC_ENABLE
#define AVRIO_TNC_ENABLE
#define AVRIO_SWITCH_ENABLE
//#define AVRIO_WUSB_ENABLE

//#define AVRIO_BATTERY_ENABLE
/* ========================================================================== */
#endif /* _AVRIO_CONFIG_H_ */
