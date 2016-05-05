/**
 * Copyright © 2011-2015 epsilonRT. All rights reserved.
 *
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
 *
 * @file debugio.S
 * @author Larry Barello, <larry@barello.net>. 
 *         Copyright © 1998-2002 All rights reserved.
 *
 * Revision History ---
 *    20110905 - Initial version by epsilonRT
 *    20050918 - This file removed from avrx library (it is too confusing, 
 *               to limited).
 *    20020907 - Delete all header file references.  Fix this in "avrx.inc"
 *             - Added _SFR_IO_ADDR() macro to all i/o access (3.2 compat)
 *    20020802 - Changed "io.h" to "io-avr.h" as the former includes some C 
 *               only header files.
 *    20020604 - Changed Rx and UDRE interrupt routines to use the #defined
 *               names to conform with the new GCC interrupt vector table 
 *               handling mechanism (this breaks IAR support in general for the 
 *               mega stuff).
 *             - fixed alias support for UCSRB and UCSRA for bigger mega chips.
 *    20010613 - Added aliases for UCSRB and UCSRA as the mega series
 *               changed the names for some reason...
 *    20010131 - Changed vDebugInit parameter register to conform to new
 *               GCC calling conventions (R25 -> R24)
 *    20020530 - Fixed re-defines of various USART registers.  What a mess.  
 *               Added code to initialize mega128 upper baud rate register.
 * ----
 * @brief Liaison Série sous interruption
 *
 * This code is might be C compatible. I have not tested it or gone over it
 * carefully to insure compatibility.   In general the Monitor and the
 * serial routines should run in their own task space and not be called by
 * any C code.
 */
#include "avrio-config.h"

#ifdef AVRIO_SERIAL_ENABLE
/* ========================================================================== */
#include <avrx/avrx.inc>
#include <avrio/serial_private.h>

/* external functions ======================================================= */
  .extern vAvrXIntEpilog
  .extern vAvrXIntProlog
  .extern vAvrXWaitSemaphore
  .extern vAvrXIntSetSemaphore

/* constants ================================================================ */
#define SERIAL_CRLF_PUTCHAR_ENABLE
#define CHARPUSHED 7

/* macros =================================================================== */

/* private variables ======================================================== */
STATIC_DATA(cRxByte, 1)
STATIC_DATA(ucSioFlags, 1)
STATIC_DATA(xRxcSem, SemSz)
STATIC_DATA(xTxcSem, SemSz)
  
/* internal public functions ================================================ */

// -----------------------------------------------------------------------------
// vDebugInit
// Serial Init with 8 data bits, 1 stop bit, no parity
// History: 
//    20110905 - Initial version by epsilonRT
//    1/31/01 l.b. parameter used to be passed in R25  
//	  5/30/02 l.b. Changed to uint16_t for mega chips.
//  
// PASSED:  p1h:p1l = UBBR
// RETURNS: -
// USES:    tmp0
// STACK:
//-
FUNCTION(vDebugInit)

    sts     UBRRL, p1l
    sts     UBRRH, p1h

    ldi     tmp0, lo8(UCSRC_SEL | _BV(UCSZ1) | _BV(UCSZ0))
    sts     UCSRC, tmp0

    ldi     tmp0, lo8(_BV(TXEN) | _BV(RXEN) | _BV(RXCIE))
    sts     UCSRB, tmp0
    
    lds     tmp0, UDR
    ret

// -----------------------------------------------------------------------------
// vDebugPutChar
// Stuffs "TxByte" into usart.  The usart is double buffered.
// when the data register is full we wait on the semaphore
// until the Usart interrupts indicating the register is ready.
//
// PASSED:  p1l = tx char
// RETURNS: -
// USES:    p1l, p1h
// STACK:   1
//-
FUNCTION(vDebugPutChar)

    lds     tmp0, UCSRA
    andi    tmp0, _BV(UDRE)
    brne    .LPCNOWAIT
    
.LPCWAIT:
    push    p1l
    lds     tmp0, UCSRB
    ori     tmp0, _BV(UDRIE)
    sts     UCSRB, tmp0

    ldi     p1l, lo8(xTxcSem)
    ldi     p1h, hi8(xTxcSem)
    rcall   vAvrXWaitSemaphore// Wait until data register is ready
    pop     p1l

.LPCNOWAIT:
#ifdef SERIAL_CRLF_PUTCHAR_ENABLE
  // if LF or CR, send CR+LF
    cpi     p1l, '\n'
    brne    .LPCPUT
    ldi     p1l, '\r'
    rcall   vDebugPutChar
    ldi     p1l, '\n'
#endif

.LPCPUT:
    sts     UDR, p1l
    ret


// -----------------------------------------------------------------------------
// cDebugGetChar
//
// Need to make this draw from a buffer so that multiple
// chars can be pushed... and so different input streams
// can be attached to the buffer.
//
// PASSED:  -
// RETURNS: r1l = rx char
// USES:    tmp0, p1l, p1h
// STACK:   -
//-
FUNCTION(cDebugGetChar)

    lds     tmp0, ucSioFlags
    sbrs    tmp0, CHARPUSHED
    rjmp    .LGCWAIT
    cbr     tmp0, 1<<CHARPUSHED
    sts     ucSioFlags, tmp0
    rjmp    .LGCNOWAIT
    
.LGCWAIT:
    ldi     p1h, hi8(xRxcSem)
    ldi     p1l, lo8(xRxcSem)
    rcall   vAvrXWaitSemaphore       // Wait for a charactor, or flag from System
    
.LGCNOWAIT:
    lds     r1l, cRxByte
    ret

// -----------------------------------------------------------------------------
// vDebugPushChar
// Set a flag so that the next call to cDebugGetChar just returns.
// in effect pushing the last returned charactor to be retrieved
// by the next routine
//
// PASSED:  -
// RETURNS: -
// USES:    -
// STACK:   1
//-
FUNCTION(vDebugPushChar)

    push    Xl
    lds     Xl, ucSioFlags
    sbr     Xl, 1<<CHARPUSHED
    sts     ucSioFlags, Xl
    pop     Xl
    ret

GLOBAL_SECTION(debugio.vectors)
// -----------------------------------------------------------------------------
// UDRE_Interrupt
//
// Disables the interrupt (UDR is empty) signal waiting
// process to stuff the UDR
//-
  .global USART_UDRE_vect
USART_UDRE_vect:
    lds     tmp0, UCSRB
    andi    tmp0, ~_BV(UDRIE)
    sts     UCSRB, tmp0      // Disable the interrupt

    rcall   vAvrXIntProlog

    ldi     p1l, lo8(xTxcSem)
    ldi     p1h, hi8(xTxcSem)
    rcall   vAvrXIntSetSemaphore     // Signal we are ready for more.

    rjmp    vAvrXIntEpilog

// -----------------------------------------------------------------------------
// RxC_Interrupt
//
// Reads the USART and flags UserTask, Char In semaphore
//-
  .global USART_RXC_vect
USART_RXC_vect:
    lds     tmp0, UCSRB
    andi    tmp0, ~_BV(RXCIE)
    sts     UCSRB, tmp0      // Disable the interrupt

    rcall   vAvrXIntProlog      // Switch to kernel stack

    lds      Xl, UDR
    sts     cRxByte, Xl

    ldi     p1h, hi8(xRxcSem)
    ldi     p1l, lo8(xRxcSem)
    rcall   vAvrXIntSetSemaphore

    lds     tmp0, UCSRB
    ori     tmp0, _BV(RXCIE)
    sts     UCSRB, tmp0      // Re-enable interrupts
    rjmp    vAvrXIntEpilog

#endif /* AVRIO_SERIAL_ENABLE defined */
/* ========================================================================== */
  .end

