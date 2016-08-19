/*
 * Copyright © 2016 epsilonRT, All rights reserved.
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
#ifndef _AVRIO_CONFIG_H_
#define _AVRIO_CONFIG_H_
/* ========================================================================== */

/* 
 * Validation des modules
 * Pour dévalider un module, mettre le define correspondant en commentaire.
 * Si le module 'moduleX' est validé, un fichier avrio-board-moduleX.h doit être
 * présent dans le même répertoire que ce fichier de configuration.
 */
#define AVRIO_ADC_ENABLE
#define AVRIO_LED_ENABLE
#define AVRIO_GPIO_ENABLE
#define AVRIO_LCD_ENABLE
#define AVRIO_LCD_BACKLIGHT_ENABLE
#define AVRIO_TWI_ENABLE
#define AVRIO_SPI_ENABLE
#define AVRIO_TSL230_ENABLE

/* ========================================================================== */
#endif /* _AVRIO_CONFIG_H_ */
