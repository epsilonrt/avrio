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

#ifndef _AVRIO_BOARD_MODBUS_H_
#define _AVRIO_BOARD_MODBUS_H_
/* ========================================================================== */

/* MODBUS =================================================================== */
#include <avrio/defs.h>
#include <avr/io.h>

/* constants ================================================================ */
#define MB_LED         LED_LED1
#define MB_LED_DELAY   100
/*
 * LEDCOM s'allume à chaque réception d'un octet sur la liaison série même si
 * le traffic ne nous concerne pas.
 */
//#define MB_SERIAL_LEDCOM         LED_LED1
//#define MB_SERIAL_LEDCOM_DELAY   100

/* ========================================================================== */
#endif /* _AVRIO_BOARD_MODBUS_H_ */
