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
 * @file cancel_timer.S
 * @brief pxAvrXCancelTimer()
 * @author Larry Barello, <larry@barello.net>. 
 *         Copyright _ 1998-2002 All rights reserved.
 *
 * Revision History
 ---
 *    20110830 - Initial port to AvrIO by epsilonRT
 */
#include "avrx.inc"

  .section .text.avrx_cancel_timer,"ax",@progbits
/* ========================================================================== */
  .extern vAvrXIntProlog
  .extern vAvrXIntSetObjectSemaphore

  .extern prvvSemRemoveObject
  .extern vAvrXIntEpilog

  .text
/* -----------------------------------------------------------------------------
 * xTimer *
 * pxAvrXCancelTimer(xTimer *)
 *
 * Passed:       R25:R24 = TCB
 * Returns:      Pointer to removed timer, or 0 if failure
 * Uses:
 * Stack:        
 * Note: walk list looking for TCB.  If found, and not at
 *       end of list, then add count to Next tcb.
 */
  .global pxAvrXCancelTimer
  .func pxAvrXCancelTimer
// -----------------------------------------------------------------------------
pxAvrXCancelTimer:
    AVRX_Prolog
    rcall   vAvrXIntSetObjectSemaphore

    ldd     p2l, Y+_p1l
    ldd     p2h, Y+_p1h
    ldi     Zl, lo8(pxTimerQueue)
    ldi     Zh, hi8(pxTimerQueue)

    BeginCritical
    rcall   prvvSemRemoveObject   // Z = next item, R23:R22 = obj.
    subi    tmp0, lo8(0)
    sbci    tmp1, hi8(0)    // Test if in timer queue
    breq    act00           // No, just return

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
act00:
    std     Y+_p1l, tmp0      // Return 0, failure.
    std     Y+_p1h, tmp1
    rjmp    vAvrXIntEpilog

  .endfunc

/* ========================================================================== */
  .end

