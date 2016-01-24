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
