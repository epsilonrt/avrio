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
#ifndef _AVRIO_BOARD_SERIAL_H_
#  define _AVRIO_BOARD_SERIAL_H_
/* ========================================================================== */

/* SERIAL =================================================================== */
#  include <avrio/defs.h>
#  include <avr/io.h>

/* constants ================================================================ */
#  define SERIAL_RXBUFSIZE 8
#  define SERIAL_TXBUFSIZE 8

// #define SERIAL_TXEN_ENABLE
#  define SERIAL_TXEN_PORT PORTB
#  define SERIAL_TXEN_DDR  DDRB
#  define SERIAL_TXEN_BIT  4
//#  define SERIAL_HALF_DUPLEX

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
