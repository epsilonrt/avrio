/**
 * @file mbmconfig.h
 * @brief MBMaster Setup File
 * @author Copyright _ 2008 Christian Walter <cwalter@embedded-solutions.at>. 
 *         All rights reserved.
 * @author Copyright _ 2012 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * @version $Id$
 * Revision History ---
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
