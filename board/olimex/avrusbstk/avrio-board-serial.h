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
#define SERIAL_TXEN_PORT PORTD
#define SERIAL_TXEN_DDR  DDRD
#define SERIAL_TXEN_BIT  4

#define UCSRA           UCSR1A
#define UCSRB           UCSR1B
#define UCSRC           UCSR1C
#define UBRRL           UBRR1L
#define UBRRH           UBRR1H
#define UDR             UDR1

#define USART_TXC_vect  USART1_TX_vect
#define USART_UDRE_vect USART1_UDRE_vect
#define USART_RXC_vect  USART1_RX_vect

/* ========================================================================== */
#endif /* _AVRIO_BOARD_SERIAL_H_ */
