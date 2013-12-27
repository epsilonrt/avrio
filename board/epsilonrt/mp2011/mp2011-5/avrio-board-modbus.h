/* Copyright _ 2009-2012 epsilonRT. All rights reserved.
 *  $Id$ */
#ifndef _AVRIO_BOARD_MODBUS_H_
#  define _AVRIO_BOARD_MODBUS_H_
/* ========================================================================== */

/* MODBUS =================================================================== */
#  include <avrio/defs.h>
#  include <avr/io.h>

/* constants ================================================================ */
#  define MB_SERIAL 0

#  define MB_SERIAL_TXEN_ENABLE
#  define MB_SERIAL_TXEN_PORT    PORTD
#  define MB_SERIAL_TXEN_DDR     DDRD
#  define MB_SERIAL_TXEN_BIT     3

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
