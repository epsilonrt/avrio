/* Copyright _ 2009-2012 epsilonRT. All rights reserved.
 *  $Id$ */
#ifndef _AVRIO_BOARD_SERIAL_H_
#define _AVRIO_BOARD_SERIAL_H_
/* ========================================================================== */

/* SERIAL =================================================================== */
#include <avrio/defs.h>
#include <avr/io.h>

/* constants ================================================================ */
#define SERIAL_EOL SERIAL_CR
//#define AVRIO_SERIAL_PORT 0
#define SERIAL_RXBUFSIZE 8
#define SERIAL_TXBUFSIZE 8

// #define SERIAL_TXEN_ENABLE
#define SERIAL_TXEN_PORT PORTD
#define SERIAL_TXEN_DDR  DDRD
#define SERIAL_TXEN_BIT  4
//#define SERIAL_HALF_DUPLEX

#define SERIAL_RTS_PORT PORTG
#define SERIAL_RTS_DDR  DDRG
#define SERIAL_RTS_BIT  0

#define SERIAL_CTS_PORT PORTG
#define SERIAL_CTS_PIN  PING
#define SERIAL_CTS_DDR  DDRG
#define SERIAL_CTS_BIT  1

/* macros =================================================================== */
#if AVRIO_SERIAL_PORT == 0
#  define UCSRA           UCSR0A
#  define UCSRB           UCSR0B
#  define UCSRC           UCSR0C
#  define UBRRL           UBRR0L
#  define UBRRH           UBRR0H
#  define UDR             UDR0
#  define USART_TXC_vect  USART0_TX_vect
#  define USART_UDRE_vect USART0_UDRE_vect
#  define USART_RXC_vect  USART0_RX_vect

#else
      /* SERIAL > 0 */
#  define UCSRA           UCSR1A
#  define UCSRB           UCSR1B
#  define UCSRC           UCSR1C
#  define UBRRL           UBRR1L
#  define UBRRH           UBRR1H
#  define UDR             UDR1
#  define USART_TXC_vect  USART1_TX_vect
#  define USART_UDRE_vect USART1_UDRE_vect
#  define USART_RXC_vect  USART1_RX_vect

#endif
       /* SERIAL > 0 */

/* ========================================================================== */
#endif /* _AVRIO_BOARD_SERIAL_H_ */
