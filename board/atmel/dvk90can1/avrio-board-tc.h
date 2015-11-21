/**
 * Copyright © 2011-2015 Pascal JEAN aka epsilonRT. All rights reserved.
 *
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
 *
 * @file tc.h
 * @brief Liaison série
 */
#ifndef _AVRIO_BOARD_TC_H_
#define _AVRIO_BOARD_TC_H_
/* ========================================================================== */
#include <avrio/defs.h>
#include <avr/io.h>

/* constants ================================================================ */
#define TC_EOL TC_CR
#define TC_RXBUFSIZE 8
#define TC_TXBUFSIZE 8
//#define TC_TXEN_ENABLE   
//#define TC_RXEN_ENABLE
#define TC_RTSCTS_ENABLE
#define TC_RXTX_PULLUP_ENABLE
#define TC_NUMOF_PORT 2
//#define AVRIO_TC_FLAVOUR TC_FLAVOUR_POLL
//#define AVRIO_TC_FLAVOUR TC_FLAVOUR_IRQ
//#define AVRIO_TC_FLAVOUR TC_FLAVOUR_RS485


#ifdef TC_RTSCTS_ENABLE
#define TC0_IO { \
  .dr   = &UDR0,   \
  .csra = &UCSR0A, \
  .csrb = &UCSR0B, \
  .csrc = &UCSR0C, \
  .brrl = &UBRR0L, \
  .brrh = &UBRR0H, \
  .rxd = { .port = &PORTE, .pin = 0 }, \
  .txd = { .port = &PORTE, .pin = 1 }, \
  .rts = { .port = &PORTG, .pin = 0 }, \
  .cts = { .port = &PORTG, .pin = 1 }, \
 }
/*
#define TC1_IO { \
  .dr   = &UDR1,   \
  .csra = &UCSR1A, \
  .csrb = &UCSR1B, \
  .csrc = &UCSR1C, \
  .brrl = &UBRR1L, \
  .brrh = &UBRR1H, \
  .rxd = { .port = &PORTE, .pin = 0 }, \
  .txd = { .port = &PORTE, .pin = 1 }, \
  .rts = { .port = &PORTD, .pin = 4 }, \
  .cts = { .port = &PORTD, .pin = 5 }, \
  }
*/
#define TC1_IO { \
  .dr   = &UDR1,   \
  .csra = &UCSR1A, \
  .csrb = &UCSR1B, \
  .csrc = &UCSR1C, \
  .brrl = &UBRR1L, \
  .brrh = &UBRR1H, \
  .rxd = { .port = &PORTD, .pin = 2 }, \
  .txd = { .port = &PORTD, .pin = 3 }, \
  .rts = { .port = &PORTG, .pin = 3 }, \
  .cts = { .port = &PORTG, .pin = 4 }, \
  }
#else
#define TC0_IO { \
  .dr   = &UDR0,   \
  .csra = &UCSR0A, \
  .csrb = &UCSR0B, \
  .csrc = &UCSR0C, \
  .brrl = &UBRR0L, \
  .brrh = &UBRR0H, \
  .rxd = { .port = &PORTE, .pin = 0 }, \
  .txd = { .port = &PORTE, .pin = 1 }, \
 }
#define TC1_IO { \
  .dr   = &UDR1,   \
  .csra = &UCSR1A, \
  .csrb = &UCSR1B, \
  .csrc = &UCSR1C, \
  .brrl = &UBRR1L, \
  .brrh = &UBRR1H, \
  .rxd = { .port = &PORTD, .pin = 2 }, \
  .txd = { .port = &PORTD, .pin = 3 }, \
  }
#endif


/* ========================================================================== */
#endif /* _AVRIO_BOARD_TC_H_ */
