/**
 * Copyright © 2014 Pascal JEAN aka epsilonRT <pascal.jean--AT--btssn.net>
 * All rights reserved.
 * This software is governed by the CeCILL license <http://www.cecill.info>
 */
#ifndef _AVRIO_BOARD_SERIAL_H_
#define _AVRIO_BOARD_SERIAL_H_
/* ========================================================================== */

/* SERIAL =================================================================== */
#include <avrio/defs.h>
#include <avr/io.h>

/* configuration ============================================================ */
#define SERIAL_RXBUFSIZE 64
#define SERIAL_TXBUFSIZE 96
//#define AVRIO_SERIAL_BAUD_USE_X2 1
#ifndef SERIAL_EOL
#define SERIAL_EOL SERIAL_CR
#endif

#define AVRIO_SERIAL_FLAVOUR SERIAL_FLAVOUR_RS485

#define SERIAL_TXEN_ENABLE
#define SERIAL_TXEN_PORT PORTD
#define SERIAL_TXEN_DDR  DDRD
#define SERIAL_TXEN_BIT  2

/* macros =================================================================== */
#ifdef USART_TX_vect
  /* 
   * ATmega328P,
   * ATmega168, ATmega168P,
   * ATmega88, ATmega88P,
   * ATmega48, ATmega48P
   */
#define USART_TXC_vect  USART_TX_vect
#define USART_RXC_vect  USART_RX_vect
#endif

#ifdef UCSR0A
/* 
 * ATmega328P,
 * ATmega168, ATmega168P,
 * ATmega88, ATmega88P,
 * ATmega48, ATmega48P
 */
#define UCSRA           UCSR0A
#define UCSRB           UCSR0B
#define UCSRC           UCSR0C
#define UBRRL           UBRR0L
#define UBRRH           UBRR0H
#define UDR             UDR0
#endif

/* ========================================================================== */
#endif /* _AVRIO_BOARD_SERIAL_H_ */
