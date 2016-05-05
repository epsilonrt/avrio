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
//#define AVRIO_SERIAL_PORT 0
#define SERIAL_RXBUFSIZE 8
#define SERIAL_TXBUFSIZE 8

// #define SERIAL_TXEN_ENABLE
#define SERIAL_TXEN_PORT PORTB
#define SERIAL_TXEN_DDR  DDRB
#define SERIAL_TXEN_BIT  4
//#define SERIAL_HALF_DUPLEX

/* macros =================================================================== */
#ifdef USART_TX_vect
  /* 
   * ATmega8535
   * ATtiny2313
   * ATmega328P,
   * ATmega168, ATmega168P,
   * ATmega88, ATmega88P,
   * ATmega48, ATmega48P
   */
#  define USART_TXC_vect  USART_TX_vect
#  define USART_RXC_vect  USART_RX_vect
#endif

#if AVRIO_SERIAL_PORT > 0
#    ifdef UCSR1A
    /* 
     * ATmega162
     */
#      define UCSRA           UCSR1A
#      define UCSRB           UCSR1B
#      define UCSRC           UCSR1C
#      define UBRRL           UBRR1L
#      define UBRRH           UBRR1H
#      define UDR             UDR1
#      define USART_TXC_vect  USART1_TXC_vect
#      define USART_UDRE_vect USART1_UDRE_vect
#      define USART_RXC_vect  USART1_RXC_vect
#    else
#      undef AVRIO_SERIAL_PORT
#      define AVRIO_SERIAL_PORT 0
#    endif
#endif

#if (AVRIO_SERIAL_PORT == 0) && defined(UCSR0A)
/* 
 * ATmega162
 * ATmega328P,
 * ATmega168, ATmega168P,
 * ATmega88, ATmega88P,
 * ATmega48, ATmega48P
 */
#  define UCSRA           UCSR0A
#  define UCSRB           UCSR0B
#  define UCSRC           UCSR0C
#  define UBRRL           UBRR0L
#  define UBRRH           UBRR0H
#  define UDR             UDR0
#    ifdef USART0_TXC_vect
  /* 
   * ATmega162
   */
#      define USART_TXC_vect  USART0_TXC_vect
#      define USART_UDRE_vect USART0_UDRE_vect
#      define USART_RXC_vect  USART0_RXC_vect
#    endif
#endif

/* ========================================================================== */
#endif /* _AVRIO_BOARD_SERIAL_H_ */
