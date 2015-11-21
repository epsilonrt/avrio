/**
 * @file serial_private.h
 * @brief Entête privé liaison série asynchrone
 *
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
 */
#ifndef _AVRIO_TC_IRQ_PRIVATE_H_
#define _AVRIO_TC_IRQ_PRIVATE_H_
/* ========================================================================== */
#include "avrio-config.h"

#ifdef AVRIO_TC_ENABLE
/* ========================================================================== */
#include "tc_private.h"
#include <avrio/queue.h>

/**
 * @brief
 */
typedef struct xTcIrqDcb {
    
  int8_t txen;
  int8_t rxen;
  volatile bool stopped;
  xQueue * rxbuf;
  xQueue * txbuf;
} xTcIrqDcb;

/* interrupt service vectors ================================================ */
#if defined(USART_RX_vect)
/* -----------------------------------------------------------------------------
 * AT90PWM3, AT90PWM2, AT90PWM1, ATmega168P, ATmega3250, ATmega3250P, ATmega328P, 
 * ATmega3290, ATmega3290P, ATmega48P, ATmega6450, ATmega6490, ATmega8535, 
 * ATmega88P, ATmega168, ATmega48, ATmega88, ATtiny2313 
 */
#define TC0_RX_vect  USART_RX_vect
#define TC0_TX_vect  USART_TX_vect
#define TC0_UDRE_vect USART_UDRE_vect

#elif defined(USART_RXC_vect)
/*
 * ATmega16, ATmega32, ATmega323, ATmega8 
 */
#define TC0_RX_vect  USART_RXC_vect
#define TC0_TX_vect  USART_TXC_vect
#define TC0_UDRE_vect USART_UDRE_vect

#elif defined(UART_RX_vect)
/*
 * AT90S2313, AT90S2333, AT90S4414, AT90S4433, AT90S4434, AT90S8515, AT90S8535, 
 * ATmega103, ATmega163, ATmega8515 
 */
#define TC0_RX_vect  UART_RX_vect
#define TC0_TX_vect  UART_TX_vect
#define TC0_UDRE_vect UART_UDRE_vect

#elif defined(USART0_RX_vect)
/*
 * AT90CAN128, AT90CAN32, AT90CAN64, ATmega128, ATmega1284P, ATmega165, 
 * ATmega165P, ATmega169, ATmega169P, ATmega325, ATmega329, ATmega64, ATmega645, 
 * ATmega649, ATmega640, ATmega1280, ATmega1281, ATmega2560, ATmega2561, 
 * ATmega324P, ATmega164P, ATmega644P, ATmega644 
 */
#define TC0_RX_vect  USART0_RX_vect
#define TC0_TX_vect  USART0_TX_vect
#define TC0_UDRE_vect USART0_UDRE_vect

#elif defined(UART0_RXC_vect)
/*
 * ATmega161
 */
#define TC0_RX_vect  UART0_RX_vect
#define TC0_TX_vect  UART0_TX_vect
#define TC0_UDRE_vect UART0_UDRE_vect

#elif defined(USART0_RXC_vect)
/*
 * ATmega162
 */
#define TC0_RX_vect  USART0_RXC_vect
#define TC0_TX_vect  USART0_TXC_vect
#define TC0_UDRE_vect USART0_UDRE_vect

#else 
#error Unable to find the interrupt vectors of first UART
#endif /* TC0_RX_vect not defined ------------------------------------------- */

#if defined(USART1_RX_vect)
/* -----------------------------------------------------------------------------
 * AT90CAN128, AT90CAN32, AT90CAN64, ATmega128, ATmega1284P, ATmega64, 
 * ATmega640, ATmega1280, ATmega1281, ATmega2560, ATmega2561, ATmega324P, 
 * ATmega164P, ATmega644P, ATmega644, AT90USB162, AT90USB82, AT90USB1287, 
 * AT90USB1286, AT90USB647, AT90USB646 
 */
#define TC1_RX_vect  USART1_RX_vect
#define TC1_TX_vect  USART1_TX_vect
#define TC1_UDRE_vect USART1_UDRE_vect

#elif defined(USART1_RXC_vect)
/*
 * ATmega162
 */
#define TC1_RX_vect  USART1_RXC_vect
#define TC1_TX_vect  USART1_TXC_vect
#define TC1_UDRE_vect USART1_UDRE_vect

#elif defined(UART1_RX_vect)
/*
 * ATmega161
 */
#define TC1_RX_vect  UART1_RX_vect
#define TC1_TX_vect  UART1_TX_vect
#define TC1_UDRE_vect UART1_UDRE_vect

#endif /* TC1_RX_vect not defined ------------------------------------------- */

#if defined(USART2_RX_vect)
/* -----------------------------------------------------------------------------
 * ATmega640, ATmega1280, ATmega1281, ATmega2560, ATmega2561  
 */
#define TC2_RX_vect  USART2_RX_vect
#define TC2_TX_vect  USART2_TX_vect
#define TC2_UDRE_vect USART2_UDRE_vect
#endif /* TC2_RX_vect not defined ------------------------------------------- */

#if defined(USART3_RX_vect)
/* -----------------------------------------------------------------------------
 * ATmega640, ATmega1280, ATmega1281, ATmega2560, ATmega2561  
 */
#define TC3_RX_vect  USART3_RX_vect
#define TC3_TX_vect  USART3_TX_vect
#define TC3_UDRE_vect USART3_UDRE_vect
#endif /* TC3_RX_vect not defined ------------------------------------------- */


/* private functions ======================================================== */
// -----------------------------------------------------------------------------
INLINE void
vRxIrqEnable (xTcPort * p) {

  TC_UCSRB |= _BV (RXCIE);
}

// -----------------------------------------------------------------------------
INLINE void
vRxIrqDisable (xTcPort * p) {

  TC_UCSRB &= ~_BV (RXCIE);
}

// -----------------------------------------------------------------------------
INLINE void
vTxUdreIrqEnable (xTcPort * p) {

  TC_UCSRB &= ~_BV (TXCIE);
  TC_UCSRB |= _BV (UDRIE);
}

// -----------------------------------------------------------------------------
INLINE void
vTxTxcIrqEnable (xTcPort * p) {

  TC_UCSRB &= ~_BV (UDRIE);
  TC_UCSRB |= _BV (TXCIE);
}

// -----------------------------------------------------------------------------
INLINE void
vTxIrqDisable (xTcPort * p) {

  TC_UCSRB &= ~ (_BV (TXCIE) | _BV (UDRIE));
}

/* ========================================================================== */
#endif /* AVRIO_TC_ENABLE defined */
#endif /* _AVRIO_TC_IRQ_PRIVATE_H_ */
