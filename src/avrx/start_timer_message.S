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
 * @file start_timer_message.S
 * @brief vAvrXStartTimerMessage
 * @author Larry Barello, <larry@barello.net>. 
 *         Copyright _ 1998-2002 All rights reserved.
 *
 * Revision History
 ---
 *    20110830 - Initial port to AvrIO by epsilonRT
 */
#include "avrx.inc"

  .section .text.avrx_start_timer_message,"ax",@progbits
/* ========================================================================== */
  .extern vAvrXSendMessage
  .extern prvvTimerCountNotZero

  .text
/* -----------------------------------------------------------------------------
 * void vAvrXStartTimerMessage(xTimerMessage *, uint16_t, xMessageQueue *)
 *
 * PASSED:       R25:R24 = TCB
 *               R23:R22 = Timeout
 *               R20:R21 = xMessageQueue
 * Returns:
 * Uses:
 *-
 */
  .global vAvrXStartTimerMessage
  .func vAvrXStartTimerMessage
// -----------------------------------------------------------------------------
vAvrXStartTimerMessage:
    subi    p2l, lo8(-0)
    sbci    p2h, hi8(-0)
    brne    axstm0
    mov     p2l, p1l
    mov     p2h, p1h
    mov     p1l, tmp2               // tmp3:tmp2 == P3 for GCC
    mov     p1h, tmp3
    rjmp    vAvrXSendMessage        // Count Zero, just queue onto message queue
axstm0:
    mov     Zl, p1l
    mov     Zh, p1h
    std     Z+TcbQueue+NextH, tmp3
    std     Z+TcbQueue+NextL, tmp2       // Stuff the queue
    ldi     tmp2, lo8(TIMERMESSAGE_EV)
    ldi     tmp3, hi8(TIMERMESSAGE_EV)
    std     Z+TcbSemaphore+NextH, tmp3
    std     Z+TcbSemaphore+NextL, tmp2   // Cruft up bogus semaphore
    rjmp    prvvTimerCountNotZero            // This is in avrx_timequeue.s

  .endfunc

/* ========================================================================== */
  .end
