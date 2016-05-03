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
 * @file single_step.S
 * @brief Debug Next Step
 * @author Larry Barello, <larry@barello.net>. 
 *         Copyright _ 1998-2002 All rights reserved.
 *
 * Revision History
 ---
 *    20110830 - Initial port to AvrIO by epsilonRT
 */
#include "avrx.inc"

  .section .text.avrx_single_step,"ax",@progbits
/* ========================================================================== */
  .extern vAvrXIntProlog
  .extern vTaskQueuePid
  .extern vAvrXIntEpilog

  .text
/* -----------------------------------------------------------------------------
 * uint8_t xAvrXSingleStepNext(xPid)
 *
 * Marks a suspended task for single step support
 * Jams it on the front of the run queue.  Returns error
 * if task is not suspended.
 *
 * PASSED:       R25:R24 = Pid to single step
 * RETURN:       R24 = 1 if task not suspended (error)
 * USES:         FLAGS (except T)
 *
 * 20020604 - Changed to use xKernelData structure.
 */
  .global xAvrXSingleStepNext
  .func xAvrXSingleStepNext
// -----------------------------------------------------------------------------
xAvrXSingleStepNext:
    AVRX_Prolog

    mov     Zh, p1h
    mov     Zl, p1l
    ldd     Xl, Z+PidState
    sbrs    Xl, SuspendedBit
    rjmp    AssnXErr

    sbr     Xl, _BV(SingleStep) // Mark for single stepping
    std     Z+PidState, Xl

    ldi	Zl, lo8(xKernelData)
    ldi	Zh, hi8(xKernelData)
    ldd     Xh, Z+RunQueue+NextH   // Put task in front of run queue
    ldd     Xl, Z+RunQueue+NextL
    std     Z+RunQueue+NextH, p1h
    std     Z+RunQueue+NextL, p1l
    mov	Zh, p1h
    mov	Zl, p1l
    std     Z+NextH, Xh
    std     Z+NextL, Xl
    rjmp    AssnXOk

  .endfunc

/* -----------------------------------------------------------------------------
 * uint8_t ucAvrXStepNext(xPid)
 *
 * Unsuspends a task, adds it to the run queue
 * then resuspends the task.
 *
 * PASSED:       R25:R24 = Pid to single step
 * RETURN:       R24 = 1 if task not suspended (error)
 * USES:
 */
  .global ucAvrXStepNext
  .func ucAvrXStepNext
// -----------------------------------------------------------------------------
ucAvrXStepNext:
    AVRX_Prolog

    mov     Zh, p1h
    mov     Zl, p1l
    ldd     Xl, Z+PidState
    sbrs    Xl, SuspendedBit
    rjmp    AssnX

    cbr     Xl, _BV(SuspendBit) | _BV(SuspendedBit)
    std     Z+PidState, Xl
    rcall   vTaskQueuePid               // Add to run queue
// <BUG BUG - QueuePid trashes Z?, yeah, reloads with R25:R24
    ldd     Xl, Z+PidState
    sbr     Xl, _BV(SuspendBit)
    std     Z+PidState, Xl
AssnXOk:
    ldi     r1l, lo8(0)     // Ok Return
    rjmp    AssnX
AssnXErr:
    ldi     r1l, lo8(1)     // Err Return
AssnX:
    std     Y+_r1l, r1l
    rjmp    vAvrXIntEpilog

  .endfunc

/* ========================================================================== */
  .end
