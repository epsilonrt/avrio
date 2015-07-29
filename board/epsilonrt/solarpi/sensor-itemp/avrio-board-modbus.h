/**
 * Copyright © 2015 Pascal JEAN aka epsilonRT <pascal.jean--AT--btssn.net>
 * All rights reserved.
 * This software is governed by the CeCILL license <http://www.cecill.info>
 */

#ifndef _AVRIO_BOARD_MODBUS_H_
#define _AVRIO_BOARD_MODBUS_H_
/* ========================================================================== */

/* MODBUS =================================================================== */
#include <avrio/defs.h>
#include <avr/io.h>

/* constants ================================================================ */
#define MB_LED                   LED_LED1
#define MB_LED_DELAY             100
/*
 * LEDCOM s'allume à chaque réception d'un octet sur la liaison série même si
 * le traffic ne nous concerne pas.
 */
//#define MB_SERIAL_LEDCOM         LED_LED1
//#define MB_SERIAL_LEDCOM_DELAY   100

/* ========================================================================== */
#endif /* _AVRIO_BOARD_MODBUS_H_ */
