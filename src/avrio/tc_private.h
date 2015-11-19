/**
 * @file tc_private.h
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

/* constants ================================================================ */
#define  TC_BINARY  0
#define  TC_CR      0x0D
#define  TC_LF      0x0A
#define  TC_CRLF    (TC_CR + TC_LF)

/* USART Control and Status Register A */
#if !defined(RXC)
#define    RXC          7
#define    TXC          6
#define    TC_UDRE         5
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
#define    TC_UDRIE        5
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
#define TC_BAUD_X1(b) (AVRIO_CPU_FREQ / (16UL * b) - 1)
#define TC_BAUD_X2(b) (AVRIO_CPU_FREQ / (8UL * b) - 1)

#if TC_NUMOF_PORT > 1
// -----------------------------------------------------------------------------
#define TC_UCSRA (*p->io.csra)
#define TC_UCSRB (*p->io.csrb)
#define TC_UCSRC (*p->io.csrc)
#define TC_UBRRL (*p->io.brrl)
#define TC_UBRRH (*p->io.brrh)
#define TC_UDR   (*p->io.dr)
// -----------------------------------------------------------------------------
#elif TC_NUMOF_PORT == 1
// -----------------------------------------------------------------------------
#define TC_UCSRA UCSRA
#define TC_UCSRB UCSRB
#define TC_UCSRC UCSRC
#define TC_UBRRL UBRRL
#define TC_UBRRH UBRRH
#define TC_UDR   UDR
// -----------------------------------------------------------------------------
#else /* TC_NUMOF_PORT == 1 */
#error TC_NUMOF_PORT bad value !
// -----------------------------------------------------------------------------
#endif /* TC_NUMOF_PORT > 1 */

/* structures =============================================================== */
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
} xTcIo;

typedef struct xTcPort {
  xTcIos ios;
  xTcIo io;
} xTcPort;

/* internal public functions ================================================ */
uint16_t usTcHit (xFileHook * h);
void vTcFlush (xFileHook * h);

/* internal private functions =============================================== */
bool bTcIsRxError (xTcPort * p);
bool xTcReady (xFileHook * h);

void vTcPrivateInit (xFileHook * h);
int iTcPrivatePutChar (char c, xFileHook * h);
int iTcPrivateGetChar (xFileHook * h);
void vTcPrivateTxEn (bool bTxEn, xFileHook * h);
void vTcPrivateRxEn (bool bRxEn, xFileHook * h);

/* inline private functions ================================================= */
// -----------------------------------------------------------------------------
INLINE void
vRxClearError (xTcPort * p) {

  if (bTcIsRxError (p)) {

    (void) TC_UDR; /* clear des flags d'erreur */
  }
}

// -----------------------------------------------------------------------------
INLINE void
vRxEnable (xTcPort * p) {

  TC_UCSRB |= _BV (RXEN);
}

// -----------------------------------------------------------------------------
INLINE void
vRxDisable (xTcPort * p) {

  TC_UCSRB &= ~_BV (RXEN);
}

// -----------------------------------------------------------------------------
INLINE void
vTxEnable (xTcPort * p) {

  TC_UCSRB |= _BV (TXEN);
}

// -----------------------------------------------------------------------------
INLINE void
vTxDisable (xTcPort * p) {

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
vRtsEnable (xFileHook * h) {
  xTcPort * p = (xTcPort *) h->dev;
  
  if ( (p->ios.flowctl == TC_FLOWCTL_RTSCTS) && (h->flag & O_RD)) {

    vDpWrite (&p->io.rts, 0);
  }
}

// -----------------------------------------------------------------------------
INLINE void
vRtsDisable (xFileHook * h) {
  xTcPort * p = (xTcPort *) h->dev;

  if ( (p->ios.flowctl == TC_FLOWCTL_RTSCTS) && (h->flag & O_RD)) {

    vDpWrite (&p->io.rts, 1);
  }
}

// -----------------------------------------------------------------------------
INLINE bool
bCtsIsEnabled (xFileHook * h) {
  xTcPort * p = (xTcPort *) h->dev;

  if ( (p->ios.flowctl == TC_FLOWCTL_RTSCTS) && (h->flag & O_WR)) {

    return bDpRead (&p->io.cts) == 0;
  }
  return true;
}

// -----------------------------------------------------------------------------
INLINE void
vRtsInit (xFileHook * h) {
  xTcPort * p = (xTcPort *) h->dev;
  
  vDpSetMode (&p->io.rts, eModeOutput);
}

// -----------------------------------------------------------------------------
INLINE void
vCtsInit (xFileHook * h) {
  xTcPort * p = (xTcPort *) h->dev;
  
  vDpSetMode (&p->io.rts, eModeInput);
}
#else /* TC_RTSCTS_ENABLE not defined */
// -----------------------------------------------------------------------------
#define vRtsInit(h)
#define vRtsEnable(h)
#define vRtsDisable(h)
#define vCtsInit(h)
#define bCtsIsEnabled(h) (true)
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
vTxEnSet (xTcPort * p) {

  vDpWrite (&p->io.txen, 1);
}

// -----------------------------------------------------------------------------
// TX OFF : Inactive à l'état bas
INLINE void
vTxEnClear (xTcPort * p) {

  vDpWrite (&p->io.txen, 0);
}

// -----------------------------------------------------------------------------
INLINE void
vTxEnInit (xTcPort * p) {

  vDpSetMode (&p->io.txen, eModeOutput);
}

#else /* TC_TXEN_ENABLE not defined */
// -----------------------------------------------------------------------------
void vTxEnInit(p)
void vTxEnClear(p)
void vTxEnSet(p)
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
vRxEnSet (xTcPort * p) {

  vDpWrite (&p->io.rxen, 0);
}

// -----------------------------------------------------------------------------
// RX OFF: Inactive à l'état haut
INLINE void
vRxEnClear (xTcPort * p) {

  vDpWrite (&p->io.txen, 1);
}

// -----------------------------------------------------------------------------
INLINE void
vRxEnInit (xTcPort * p) {

  vDpSetMode (&p->io.txen, eModeOutputHigh);
}
#else /* TC_RXEN_ENABLE not defined */
// -----------------------------------------------------------------------------
void vRxEnInit(p)
void vRxEnClear(p)
void vRxEnSet(p)
// -----------------------------------------------------------------------------
#endif /* TC_RXEN_ENABLE */

/* ========================================================================== */
#endif  /* AVRIO_TC_ENABLE defined */
#endif /* _AVRIO_TC_PRIVATE_H_ */
