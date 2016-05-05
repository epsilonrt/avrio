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
#ifndef _AVRIO_BOARD_SERIAL_H_
#define _AVRIO_BOARD_SERIAL_H_
/* ========================================================================== */

/* SERIAL =================================================================== */
#include <avrio/defs.h>
#include <avr/io.h>

/* constants ================================================================ */
#define SERIAL_EOL SERIAL_CR
//#define AVRIO_SERIAL_PORT 1
#define SERIAL_RXBUFSIZE 8
#define SERIAL_TXBUFSIZE 8

// #define SERIAL_TXEN_ENABLE
#define SERIAL_TXEN_PORT PORTD
#define SERIAL_TXEN_DDR  DDRD
#define SERIAL_TXEN_BIT  4
//#define SERIAL_HALF_DUPLEX

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
