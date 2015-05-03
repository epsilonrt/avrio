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
 * @file mbmconfig.h
 * @brief MBMaster Setup File
 * Copyright _ 2008 Christian Walter <cwalter@embedded-solutions.at>. 
 *         All rights reserved.
 *
 *
 * Revision History ------------------------------------------------------------
 *    20120406 - Initial version by epsilonRT
 */

/* ========================================================================== */

#ifndef _MBM_CONFIG_H
#define _MBM_CONFIG_H

#include <avrio/defs.h>
/* *INDENT-OFF* */
__BEGIN_C_DECLS
/* ========================================================================== */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avrio/led.h>

/* constants ================================================================ */

/* ---------------------------- Setup MODBUS ---------------------------------*/
#define MBM_ASCII_ENABLED                       ( 1 )
#define MBM_RTU_ENABLED                         ( 1 )
#define MBM_TCP_ENABLED                         ( 0 )
#define MBM_DEFAULT_RESPONSE_TIMEOUT            ( 500 )
#define MBM_SERIAL_RTU_MAX_INSTANCES            ( 1 )
#define MBM_SERIAL_ASCII_MAX_INSTANCES          ( 1 )
#define MBM_TCP_MAX_INSTANCES                   ( 0 )
#define MBM_RTU_WAITAFTERSEND_ENABLED			      ( 1 )

/* ---------------------------- Setup Serial ---------------------------------*/
//#define MBM_USART_BAUDRATE_MIN		( F_CPU / (16UL * (0xFFFUL + 1UL)) )
//#define MBM_USART_BAUDRATE_MAX		( F_CPU / (16UL) )

#define MBM_USART0_ENABLED        ( 1 )   /*< Set this to 1 to enable USART0 */
#define MBM_USART0_RS485_ENABLED  ( 1 )
#define MBM_USART0_RS485_ENABLE_PORT	PORTC
#define MBM_USART0_RS485_ENABLE_DDR		DDRC
#define MBM_USART0_RS485_ENABLE_PIN		PC3

/* Only for MCU that have 2 USART */
#define MBM_USART1_ENABLED        ( 0 )   /*< Set this to 1 to enable USART1 */
#define MBM_USART1_RS485_ENABLED  ( 0 )
#define MBM_USART1_RS485_ENABLE_PORT	PORTC
#define MBM_USART1_RS485_ENABLE_DDR		DDRC
#define MBM_USART1_RS485_ENABLE_PIN		PC2

/* ----------------------------- Setup Debug ---------------------------------*/
#define MBM_DEBUG_LED_ERROR     ( LED_LED1 )
#define MBM_DEBUG_LED_WORKING   ( LED_LED2 )

/* private functions ======================================================== */
#if (MBM_USART0_ENABLED != 0) && (MBM_USART0_RS485_ENABLED != 0)
// -----------------------------------------------------------------------------
static inline void
MBM_USART0_RS485_INIT (void) {

  MBM_USART0_RS485_ENABLE_DDR |= _BV(MBM_USART0_RS485_ENABLE_PIN);
}

// -----------------------------------------------------------------------------
static inline void
MBM_USART0_RS485_ENABLE_TX (void) {

  MBM_USART0_RS485_ENABLE_PORT |= _BV(MBM_USART0_RS485_ENABLE_PIN);
}

// -----------------------------------------------------------------------------
static inline void
MBM_USART0_RS485_DISABLE_TX (void) {

  MBM_USART0_RS485_ENABLE_PORT &= ~_BV(MBM_USART0_RS485_ENABLE_PIN);
}
#else
# define MBM_USART0_RS485_INIT(  )
# define MBM_USART0_RS485_ENABLE_TX(  )
# define MBM_USART0_RS485_DISABLE_TX(  )
#endif

#if (MBM_USART1_ENABLED != 0) && (MBM_USART1_RS485_ENABLED != 0)
// -----------------------------------------------------------------------------
static inline void
MBM_USART1_RS485_INIT (void) {

  MBM_USART1_RS485_ENABLE_DDR |= _BV(MBM_USART1_RS485_ENABLE_PIN);
}

// -----------------------------------------------------------------------------
static inline void
MBM_USART1_RS485_ENABLE_TX (void) {

  MBM_USART1_RS485_ENABLE_PORT |= _BV(MBM_USART1_RS485_ENABLE_PIN);
}

// -----------------------------------------------------------------------------
static inline void
MBM_USART1_RS485_DISABLE_TX (void) {

  MBM_USART1_RS485_ENABLE_PORT &= ~_BV(MBM_USART1_RS485_ENABLE_PIN);
}

#else
#define MBM_USART1_RS485_INIT(  )
#define MBM_USART1_RS485_ENABLE_TX(  )
#define MBM_USART1_RS485_DISABLE_TX(  )
#endif

/* ========================================================================== */
__END_C_DECLS
/* *INDENT-ON* */
#endif
