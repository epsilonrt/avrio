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

