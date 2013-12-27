/* Copyright _ 2009-2012 epsilonRT. All rights reserved.
 *  $Id$ */
#ifndef _AVRIO_BOARD_SERIAL_H_
#  define _AVRIO_BOARD_SERIAL_H_
/* ========================================================================== */

/* SERIAL =================================================================== */
#  include <avrio/defs.h>
#  include <avr/io.h>

/* constants ================================================================ */
#  define SERIAL_RXBUFSIZE 8
#  define SERIAL_TXBUFSIZE 8

#  define SERIAL_TXEN_ENABLE
#  define SERIAL_TXEN_PORT PORTD
#  define SERIAL_TXEN_DDR  DDRD
#  define SERIAL_TXEN_BIT  3

/* ========================================================================== */
#endif /* _AVRIO_BOARD_SERIAL_H_ */
