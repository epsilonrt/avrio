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
