/**
 * @file tc_private.h
 * @brief Entête privé liaison série asynchrone
 *
 * Copyright © 2011-2015 epsilonRT. All rights reserved.
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
#ifndef _AVRIO_TC_PRIVATE_H_
#define _AVRIO_TC_PRIVATE_H_
/* ========================================================================== */
#include "avrio-config.h"

#ifdef AVRIO_TC_ENABLE
/* ========================================================================== */
#include "tc.h"
#include "dpin.h"
#include "file_private.h"
#include "avrio-board-tc.h"

#ifndef AVRIO_TC_FLAVOUR
#warning AVRIO_TC_FLAVOUR not defined, was defined to TC_FLAVOUR_POLL
#define AVRIO_TC_FLAVOUR TC_FLAVOUR_POLL
# endif

#if !defined(U2X) && !defined(U2X0)
#error Too old MCU, this UART is not supported.
#endif

/* structures =============================================================== */
/**
 * @brief
 */
typedef struct xTcIo {

#if TC_NUMOF_PORT > 1
  volatile uint8_t * dr;
  volatile uint8_t * csra;
  volatile uint8_t * csrb;
  volatile uint8_t * csrc;
  volatile uint8_t * brrl;
  volatile uint8_t * brrh;
#endif /* TC_NUMOF_PORT > 1 */

#ifdef TC_RTSCTS_ENABLE
  xDPin rts;
  xDPin cts;
#endif

#ifdef TC_TXEN_ENABLE
  xDPin txen;
#endif

#ifdef TC_RXEN_ENABLE
  xDPin rxen;
#endif
#ifdef TC_RXTX_PULLUP_ENABLE
  xDPin txd;
  xDPin rxd;
#endif
} xTcIo;

/**
 * @brief
 */
typedef struct xTcPort {
  xSerialIos ios; /**< configuration du port */
  void * dcb; /**< device control block */
  xFileHook *hook; /**< pointeur vers le niveau supérieur */
  int8_t inode; /**< index du port dans la table des ports (indirection) */
  int8_t uart; /**< index de l'uart utilisé [0..N] */
  xTcIo io; /**< accès aux registres */
} xTcPort;

/* io defines =============================================================== */
#if TC_NUMOF_PORT > 1
// -----------------------------------------------------------------------------
#define TC_UCSRA (*p->io.csra)
#define TC_UCSRB (*p->io.csrb)
#define TC_UCSRC (*p->io.csrc)
#define TC_UBRRL (*p->io.brrl)
#define TC_UBRRH (*p->io.brrh)
#define TC_UDR   (*p->io.dr)
#define TC_UART  (p->uart)
// -----------------------------------------------------------------------------
#elif TC_NUMOF_PORT == 1
// -----------------------------------------------------------------------------
#define TC_UCSRA UCSRA
#define TC_UCSRB UCSRB
#define TC_UCSRC UCSRC
#define TC_UBRRL UBRRL
#define TC_UBRRH UBRRH
#define TC_UDR   UDR
#define TC_UART  (0)
// -----------------------------------------------------------------------------
#else /* TC_NUMOF_PORT == 1 */
#error TC_NUMOF_PORT bad value !
// -----------------------------------------------------------------------------
#endif /* TC_NUMOF_PORT > 1 */

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

/* USART Control and Status Register A */
#if !defined(RXC)
#define    RXC          7
#define    TXC          6
#define    UDRE         5
#define    FE           4
#define    DOR          3
#define    PE           2
#define    U2X          1

#elif !defined(PE)
#define    PE           2
#endif

/* USART Control and Status Register B */
#ifndef RXCIE
#define    RXCIE        7
#define    TXCIE        6
#define    UDRIE        5
#define    RXEN         4
#define    TXEN         3
#define    UCSZ2        2
#define    RXB8         1
#define    TXB8         0
#endif

/* USART Control and Status Register C */
#ifndef UPM1
#define    UPM1         5
#define    UPM0         4
#define    USBS         3
#define    UCSZ1        2
#define    UCSZ0        1
#endif

#ifdef URSEL
#define UCSRC_SEL _BV(URSEL)
#else
#define UCSRC_SEL 0
#endif

/* macros =================================================================== */

/* internal public functions ================================================ */
int iTcRxError (xTcPort * p);

/* internal private functions =============================================== */
void vTcPrivInit (xTcPort * p);
int iTcPrivPutChar (char c, xTcPort * p);
int iTcPrivGetChar (xTcPort * p);
void vTcPrivTxEn (bool bTxEn, xTcPort * p);
void vTcPrivRxEn (bool bRxEn, xTcPort * p);
bool xTcPrivReady (xTcPort * p);
uint16_t usTcPrivDataAvailable (xTcPort * p);
void vTcPrivFlush (xTcPort * p);

/* inline private functions ================================================= */
// -----------------------------------------------------------------------------
INLINE void
vUartClearRxError (xTcPort * p) {

  if (iTcRxError (p)) {

    (void) TC_UDR; /* clear des flags d'erreur */
  }
}

// -----------------------------------------------------------------------------
INLINE void
vUartEnableRx (xTcPort * p) {

  TC_UCSRB |= _BV (RXEN);
}

// -----------------------------------------------------------------------------
INLINE void
vUartDisableRx (xTcPort * p) {

  TC_UCSRB &= ~_BV (RXEN);
}

// -----------------------------------------------------------------------------
INLINE void
vUartEnableTx (xTcPort * p) {

  TC_UCSRB |= _BV (TXEN);
}

// -----------------------------------------------------------------------------
INLINE void
vUartDisableTx (xTcPort * p) {

  TC_UCSRB &= ~_BV (TXEN);
}

/* -----------------------------------------------------------------------------
 *
 *                      Contrôle de flux RTS/CTS
 *
 * ---------------------------------------------------------------------------*/

#ifdef TC_RTSCTS_ENABLE
// -----------------------------------------------------------------------------
INLINE void
vRtsEnable (xTcPort * p) {

  if ( (p->ios.flow == SERIAL_FLOW_RTSCTS) && (p->hook->flag & O_RD)) {

    vDpWrite (&p->io.rts, 0);
  }
}

// -----------------------------------------------------------------------------
INLINE void
vRtsDisable (xTcPort * p) {

  if ( (p->ios.flow == SERIAL_FLOW_RTSCTS) && (p->hook->flag & O_RD)) {

    vDpWrite (&p->io.rts, 1);
  }
}

// -----------------------------------------------------------------------------
INLINE bool
bCtsIsEnabled (xTcPort * p) {

  if ( (p->ios.flow == SERIAL_FLOW_RTSCTS) && (p->hook->flag & O_WR)) {

    return bDpRead (&p->io.cts) == 0;
  }
  return true;
}

// -----------------------------------------------------------------------------
INLINE void
vRtsInit (xTcPort * p) {

  vDpSetMode (&p->io.rts, eModeOutput); // active le RTS à 0
}

// -----------------------------------------------------------------------------
INLINE void
vCtsInit (xTcPort * p) {

  vDpSetMode (&p->io.cts, eModeInput);
}
#else /* TC_RTSCTS_ENABLE not defined */
// -----------------------------------------------------------------------------
#define vRtsInit(p)
#define vRtsEnable(p)
#define vRtsDisable(p)
#define vCtsInit(p)
#define bCtsIsEnabled(p) (true)
// -----------------------------------------------------------------------------
#endif /* TC_RTSCTS_ENABLE */

/* -----------------------------------------------------------------------------
 *
 *                   Broche de validation Transmission
 *
 * ---------------------------------------------------------------------------*/
#ifdef TC_TXEN_ENABLE

// -----------------------------------------------------------------------------
// TX ON: Active à l'état haut
INLINE void
vTxenSet (xTcPort * p) {

  vDpWrite (&p->io.txen, 1);
}

// -----------------------------------------------------------------------------
// TX OFF : Inactive à l'état bas
INLINE void
vTxenClear (xTcPort * p) {

  vDpWrite (&p->io.txen, 0);
}

// -----------------------------------------------------------------------------
INLINE void
vTxenInit (xTcPort * p) {

  vDpSetMode (&p->io.txen, eModeOutput);
}

#else /* TC_TXEN_ENABLE not defined */
// -----------------------------------------------------------------------------
#define vTxenInit(p)
#define vTxenClear(p)
#define vTxenSet(p)
// -----------------------------------------------------------------------------
#endif /* TC_TXEN_ENABLE */

/* -----------------------------------------------------------------------------
 *
 *                   Broche de validation Réception
 *
 * ---------------------------------------------------------------------------*/
#ifdef TC_RXEN_ENABLE
// -----------------------------------------------------------------------------
// RX ON: Active à l'état bas
INLINE void
vRxenSet (xTcPort * p) {

  vDpWrite (&p->io.rxen, 0);
}

// -----------------------------------------------------------------------------
// RX OFF: Inactive à l'état haut
INLINE void
vRxenClear (xTcPort * p) {

  vDpWrite (&p->io.txen, 1);
}

// -----------------------------------------------------------------------------
INLINE void
vRxenInit (xTcPort * p) {

  vDpSetMode (&p->io.rxen, eModeOutputHigh);
}
#else /* TC_RXEN_ENABLE not defined */
// -----------------------------------------------------------------------------
#define vRxenInit(p)
#define vRxenClear(p)
#define vRxenSet(p)
// -----------------------------------------------------------------------------
#endif /* TC_RXEN_ENABLE */

/* ========================================================================== */
#endif  /* AVRIO_TC_ENABLE defined */
#endif /* _AVRIO_TC_PRIVATE_H_ */
