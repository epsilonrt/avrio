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
 * @file cancel_timer_message.S
 * @brief pxAvrXCancelTimerMessage
 * @author Larry Barello, <larry@barello.net>. 
 *         Copyright _ 1998-2002 All rights reserved.
 *
 * Revision History
 ---
 *    20110830 - Initial port to AvrIO by epsilonRT
 */
#include "avrx.inc"

  .section .text.avrx_cancel_timer_message,"ax",@progbits
/* ========================================================================== */
  .extern vAvrXIntProlog
  .extern vAvrXIntEpilog
  .extern prvvSemRemoveObject

  .text
/* -----------------------------------------------------------------------------
 * xTimer *
 * pxAvrXCancelTimerMessage(xTimer *, xMessageQueue *) CONST
 *
 * Passed:       R25:R24 = xTimer *
 *               R23:R22 = xMessageQueue *
 * Returns:      R25:R24 = Pointer to removed timer, or 0 if failure
 * Uses:
 * Stack:        
 * Note: walk list looking for TCB.  If found, and not at
 *       end of list, then add count to Next tcb. Else, tries
 *       to remove from message queue.
 */
  .global pxAvrXCancelTimerMessage
  .func pxAvrXCancelTimerMessage
// -----------------------------------------------------------------------------
pxAvrXCancelTimerMessage:
    AVRX_Prolog
    mov     p2l, p1l
    mov     p2h, p1h
    ldi     Zl, lo8(pxTimerQueue)
    ldi     Zh, hi8(pxTimerQueue)

    BeginCritical
    rcall   prvvSemRemoveObject   // Z = next item, R23:R22 = obj.
    subi    tmp0, lo8(0)
    sbci    tmp1, hi8(0)    // Test if in timer queue
    breq    actm00          // No, remove from message queue

    mov     Yl, p2l         // Y is restored in vAvrXIntEpilog
    mov     Yh, p2h
    ldd     p2l, Y+TcbCount+NextL
    ldd     p2h, Y+TcbCount+NextH
    ldd     r1h, Z+TcbCount+NextH // Next
    ldd     r1l, Z+TcbCount+NextL
    add     r1l, p2l
    adc     r1h, p2h
    std     Z+TcbCount+NextL, r1l
    std     Z+TcbCount+NextH, r1h
    rjmp    vAvrXIntEpilog
    //
    // Timer expired, try to remove from messge queue
actm00:
    ldd     Zl, Y+_p2l
    ldd     Zh, Y+_p2h      // Recover pointer to queue
    rcall   prvvSemRemoveObject   // p2 still point to TimerMessage object
    subi    tmp0, lo8(0)
    sbci    tmp1, hi8(0)    // Test if in message queue
    brne    actm01
    std     Y+_r1l, Xl      // If not found, stuff 0 into return registers
    std     Y+_r1h, Xh
actm01:
    rjmp    vAvrXIntEpilog

  .endfunc

/* ========================================================================== */
  .end
