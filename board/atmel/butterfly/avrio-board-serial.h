/* Copyright _ 2009-2012 epsilonRT. All rights reserved.
 *  $Id$ */
#ifndef _AVRIO_BOARD_SERIAL_H_
#define _AVRIO_BOARD_SERIAL_H_
/* ========================================================================== */

/* SERIAL =================================================================== */
#include <avrio/defs.h>
#include <avr/io.h>

/* configuration ============================================================ */
#define SERIAL_EOL SERIAL_CR
#define SERIAL_RXBUFSIZE 8
#define SERIAL_TXBUFSIZE 8

// #define SERIAL_TXEN_ENABLE
#define SERIAL_TXEN_PORT PORTB
#define SERIAL_TXEN_DDR  DDRB
#define SERIAL_TXEN_BIT  4
//#define SERIAL_HALF_DUPLEX

/* macros =================================================================== */

#define USART_TXC_vect  USART0_TX_vect
#define USART_RXC_vect  USART0_RX_vect
#define USART_UDRE_vect USART0_UDRE_vect

/* ========================================================================== */
#endif /* _AVRIO_BOARD_SERIAL_H_ */
