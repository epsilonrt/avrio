/* Copyright © 2009-2012 epsilonRT. All rights reserved.
 *  $Id$ */
#ifndef _AVRIO_BOARD_SERIAL_H_
#  define _AVRIO_BOARD_SERIAL_H_
/* ========================================================================== */

/* SERIAL =================================================================== */
#  include <avrio/defs.h>
#  include <avr/io.h>

/* constants ================================================================ */
#  define SERIAL_RXBUFSIZE 32
#  define SERIAL_TXBUFSIZE 16

//#  define SERIAL_TXEN_ENABLE
#  define SERIAL_TXEN_PORT PORTC
#  define SERIAL_TXEN_DDR  DDRC
#  define SERIAL_TXEN_BIT  3

#  define SERIAL_RTS_PORT PORTB
#  define SERIAL_RTS_DDR  DDRB
#  define SERIAL_RTS_BIT  7

#  define SERIAL_CTS_PORT PORTB
#  define SERIAL_CTS_PIN  PINB
#  define SERIAL_CTS_DDR  DDRB
#  define SERIAL_CTS_BIT  6

//#  define SERIAL_HALF_DUPLEX
//#  define AVRIO_SERIAL_RTSCTS

/* macros =================================================================== */
#  ifdef USART_TX_vect
  /*
   * ATmega328P,
   * ATmega168, ATmega168P,
   * ATmega88, ATmega88P,
   * ATmega48, ATmega48P
   */
#    define USART_TXC_vect  USART_TX_vect
#    define USART_RXC_vect  USART_RX_vect
#  endif

#  ifdef UCSR0A
/*
 * ATmega328P,
 * ATmega168, ATmega168P,
 * ATmega88, ATmega88P,
 * ATmega48, ATmega48P
 */
#    define UCSRA           UCSR0A
#    define UCSRB           UCSR0B
#    define UCSRC           UCSR0C
#    define UBRRL           UBRR0L
#    define UBRRH           UBRR0H
#    define UDR             UDR0
#  endif

/* ========================================================================== */
#endif /* _AVRIO_BOARD_SERIAL_H_ */
