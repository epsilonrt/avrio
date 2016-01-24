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
#  define _AVRIO_BOARD_MODBUS_H_
/* ========================================================================== */

/* MODBUS =================================================================== */
#  include <avrio/defs.h>
#  include <avr/io.h>

/* constants ================================================================ */
#  define MB_SERIAL 0

#  define MB_SERIAL_TXEN_ENABLE
#  define MB_SERIAL_TXEN_PORT    PORTB
#  define MB_SERIAL_TXEN_DDR     DDRB
#  define MB_SERIAL_TXEN_BIT     4

#  define MB_SERIAL_LEDCOM_ENABLE
#  define MB_SERIAL_LEDCOM_PORT    PORTD
#  define MB_SERIAL_LEDCOM_DDR     DDRD
#  define MB_SERIAL_LEDCOM_BIT     7
#  define MB_SERIAL_LEDCOM_DELAY   100

#  define MB_SERIAL_LEDERR_ENABLE
#  define MB_SERIAL_LEDERR_PORT    PORTD
#  define MB_SERIAL_LEDERR_DDR     DDRD
#  define MB_SERIAL_LEDERR_BIT     6

/* ========================================================================== */
#endif /* _AVRIO_BOARD_MODBUS_H_ */
