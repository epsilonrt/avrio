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
 * @file suspend.S
 * @brief Suspend
 * @author Larry Barello, <larry@barello.net>. 
 *         Copyright _ 1998-2002 All rights reserved.
 *
 * Revision History
 ---
 *    20110830 - Initial port to AvrIO by epsilonRT
 */
#include "avrx.inc"

  .section .text.avrx_suspend,"ax",@progbits
/* ========================================================================== */
  .extern vAvrXIntProlog
  .extern vAvrXIntEpilog
  .extern prvvSemRemoveObject

  .text
/* -----------------------------------------------------------------------------
 * vAvrXSuspend
 *
 * Mark a pid for suspension and attempt to remove from the run queue
 *
 * -----------------------------------------------------------------------------
 * AvrXBreakpoint
 *
 * Suspend Self (Running) if previously suspended (single stepped)
 *
 * PASSED:       R25:R24 = PID
 * RETURNS:
 * USES:
 * ASSUMES:
 * NOTES:        Marks PID for suspension.  If removed from the run
 *               queue, then marks suspended.  vTaskQueuePid will take care
 *               of this for pids queued elsewhere when they become
 *               eligable for running.
 *
 * 20020604 - modified to use xKernelData structure
 */
  .global AvrXBreakpoint
  .func AvrXBreakpoint
// -----------------------------------------------------------------------------
AvrXBreakpoint:
    AVRX_Prolog
    lds     Zh, xKernelData+Running+NextH
    lds     Zl, xKernelData+Running+NextL
    ldd     tmp0, Z+PidState
    sbrc    tmp0, SuspendBit
    rjmp    as00
    rjmp    vAvrXIntEpilog
  .endfunc

  .global vAvrXSuspend
  .func vAvrXSuspend
// -----------------------------------------------------------------------------
vAvrXSuspend:                    // User Entry Point
    AVRX_Prolog
    mov     Zh, p1h
    mov     Zl, p1l
    ldd     tmp0, Z+PidState
    sbr     tmp0, _BV(SuspendBit)      // Mark process for suspending
    std     Z+PidState, tmp0
as00:
    mov     p2h, Zh
    mov     p2l, Zl
    ldi     Zh, hi8(xKernelData+RunQueue)
    ldi     Zl, lo8(xKernelData+RunQueue)
    BeginCritical
    rcall   prvvSemRemoveObject           // Attempt to remove from run queue
    mov     Yl, tmp0
    mov     Yh, tmp1
    adiw    Yl, 0
    breq    as01

    ldd     Zl, Y+PidState
    sbr     Zl, _BV(SuspendedBit)    // If found, then mark suspended.
    std     Y+PidState, Zl
as01:
    rjmp    vAvrXIntEpilog

  .endfunc

/* ========================================================================== */
  .end
