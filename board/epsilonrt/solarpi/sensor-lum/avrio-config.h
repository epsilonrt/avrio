/**
 * Copyright © 2015 Pascal JEAN aka epsilonRT <pascal.jean--AT--btssn.net>
 * All rights reserved.
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
//#define AVRIO_KTIMER_ENABLE
#define AVRIO_LED_ENABLE
#define AVRIO_BUTTON_ENABLE
#define AVRIO_SERIAL_ENABLE
#define AVRIO_SERIAL_FLAVOUR SERIAL_FLAVOUR_RS485
//#define AVRIO_IRQ_ENABLE
//#define AVRIO_TSL230_ENABLE
//#define AVRIO_MODBUS_ENABLE
//#define AVRIO_TWI_ENABLE
//#define AVRIO_SPI_ENABLE
//#define AVRIO_ADC_ENABLE
/* ========================================================================== */
#endif /* _AVRIO_CONFIG_H_ */
