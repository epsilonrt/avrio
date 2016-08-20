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
#ifndef _AVRIO_BOARD_TC_H_
#define _AVRIO_BOARD_TC_H_
/* ========================================================================== */
#include <avrio/defs.h>
#include <avr/io.h>

/* constants ================================================================ */
#define TC_EOL SERIAL_CR
#define TC_RXBUFSIZE 8
#define TC_TXBUFSIZE 8
//#define TC_TXEN_ENABLE   
//#define TC_RXEN_ENABLE
#define TC_RTSCTS_ENABLE
#define TC_RXTX_PULLUP_ENABLE
#define TC_NUMOF_PORT 2
//#define AVRIO_TC_FLAVOUR TC_FLAVOUR_POLL
//#define AVRIO_TC_FLAVOUR TC_FLAVOUR_IRQ


#ifdef TC_RTSCTS_ENABLE
#define TC0_IO { \
  .dr   = &UDR0,   \
  .csra = &UCSR0A, \
  .csrb = &UCSR0B, \
  .csrc = &UCSR0C, \
  .brrl = &UBRR0L, \
  .brrh = &UBRR0H, \
  .rxd = { .port = &PORTE, { .pin = 0 }}, \
  .txd = { .port = &PORTE, { .pin = 1 }}, \
  .rts = { .port = &PORTG, { .pin = 0 }}, \
  .cts = { .port = &PORTG, { .pin = 1 }}, \
 }

#define TC1_IO { \
  .dr   = &UDR1,   \
  .csra = &UCSR1A, \
  .csrb = &UCSR1B, \
  .csrc = &UCSR1C, \
  .brrl = &UBRR1L, \
  .brrh = &UBRR1H, \
  .rxd = { .port = &PORTD, { .pin = 2 }}, \
  .txd = { .port = &PORTD, { .pin = 3 }}, \
  .rts = { .port = &PORTG, { .pin = 3 }}, \
  .cts = { .port = &PORTG, { .pin = 4 }}, \
  }
#else /* TC_RTSCTS_ENABLE not defined */

#define TC0_IO { \
  .dr   = &UDR0,   \
  .csra = &UCSR0A, \
  .csrb = &UCSR0B, \
  .csrc = &UCSR0C, \
  .brrl = &UBRR0L, \
  .brrh = &UBRR0H, \
  .rxd = { .port = &PORTE, { .pin = 0 }}, \
  .txd = { .port = &PORTE, { .pin = 1 }}, \
 }

#define TC1_IO { \
  .dr   = &UDR1,   \
  .csra = &UCSR1A, \
  .csrb = &UCSR1B, \
  .csrc = &UCSR1C, \
  .brrl = &UBRR1L, \
  .brrh = &UBRR1H, \
  .rxd = { .port = &PORTD, { .pin = 2 }}, \
  .txd = { .port = &PORTD, { .pin = 3 }}, \
  }
#endif /* TC_RTSCTS_ENABLE not defined */

/* ========================================================================== */
#endif /* _AVRIO_BOARD_TC_H_ */
