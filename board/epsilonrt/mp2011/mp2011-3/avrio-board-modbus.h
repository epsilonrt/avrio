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
