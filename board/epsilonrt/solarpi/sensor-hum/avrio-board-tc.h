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
#define TC_EOL SERIAL_CR
#define TC_RXBUFSIZE 16
#define TC_TXBUFSIZE 16
#define AVRIO_TC_BAUD_USE_X2 1

#define TC_TXEN_ENABLE   
#define TC_RXTX_PULLUP_ENABLE
#define TC_NUMOF_PORT 1
#define AVRIO_TC_FLAVOUR TC_FLAVOUR_IRQ
#define AVRIO_TC_RS485

#define UCSRA           UCSR0A
#define UCSRB           UCSR0B
#define UCSRC           UCSR0C
#define UBRRL           UBRR0L
#define UBRRH           UBRR0H
#define UDR             UDR0
#define TC0_IO { \
  .rxd =  { .port = &PORTD, .pin = 0 }, \
  .txd =  { .port = &PORTD, .pin = 1 }, \
  .txen = { .port = &PORTD, .pin = 2 }, \
 }

/* ========================================================================== */
#endif /* _AVRIO_BOARD_TC_H_ */
