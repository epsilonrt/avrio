/**
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
 *
 * @file terminate.S
 * @brief Terminate
 * @author Larry Barello, <larry@barello.net>. 
 *         Copyright _ 1998-2002 All rights reserved.
 *
 * Revision History
 ---
 *    20110830 - Initial port to AvrIO by epsilonRT
 *    20020604 - Modified to use xKernelData structure
 */
#include "avrx.inc"

  .section .text.avrx_terminate,"ax",@progbits
/* ========================================================================== */
  .extern vAvrXIntProlog
  .extern vAvrXIntSetObjectSemaphore
  .extern prvvSemRemoveObject
  .extern vAvrXIntEpilog

  .text
/* -----------------------------------------------------------------------------
 * vAvrXTaskExit
 * vAvrXTerminate
 *
 * This is where tasks that "return" come to.  Set the IDLE bit so
 * it can not be queued again.  Then attempt to Dequeue from run queue.
 * Can not do anything more than that if it is queued on a semaphore...
 *
 * PASSED:       R25:R24 = Pid to terminate
 * RETURNS:
 * USES:
 * CALLS:
 * ASSUMES:
 * NOTES:
 */

#ifdef  AVRX_TASKEXIT_ENABLE
  .global vAvrXTaskExit
  .func vAvrXTaskExit
// -----------------------------------------------------------------------------
vAvrXTaskExit:    // Entry point for killing self
    AVRX_Prolog   // Entire state of dying task is on stack
    lds     p1h, xKernelData+Running+NextH
    lds     p1l, xKernelData+Running+NextL
    rjmp    at00

  .endfunc
#endif /* AVRX_TASKEXIT_ENABLE */

  .global vAvrXTerminate
  .func vAvrXTerminate
// -----------------------------------------------------------------------------
vAvrXTerminate:  // Entry point for killing other tasks.
    AVRX_Prolog
at00:
    // rcall   vAvrXIntSetObjectSemaphore       // Later: Signal parent task
    // subi    p1l, lo8(-2)
    // sbci    p1h, hi8(-2)
    mov     Zh, p1h
    mov     Zl, p1l
    ldi     Xl, _BV(IdleBit) // Mark task dead
    std     Z+PidState, Xl
    mov     p2h, p1h
    mov     p2l, p1l
    ldi     Zh, hi8(xKernelData+RunQueue)
    ldi     Zl, lo8(xKernelData+RunQueue)
    BeginCritical
    rcall   prvvSemRemoveObject  // Attempt to remove from run queue
    rjmp    vAvrXIntEpilog
  .endfunc

/* ========================================================================== */
  .end
