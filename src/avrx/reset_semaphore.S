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
 * @file reset_semaphore.S
 * @brief Reset semaphore
 * @author Larry Barello, <larry@barello.net>. 
 *         Copyright _ 1998-2002 All rights reserved.
 *
 * Revision History ---
 *    20110830 - Initial port to AvrIO by epsilonRT
 *    20010403 - Initial update for avrx-ctoasm.inc port
*/
#include "avrx.inc"

  .section .text.avrx_reset_semaphore,"ax",@progbits
/* ========================================================================== */

  .text
/* -----------------------------------------------------------------------------
 * void vAvrXResetSemaphore (xSem *)
 * void vAvrXResetObjectSemaphore (xSem *)
 *
 * Force a semaphore into the _PEND state.  This is almost identical
 * to SetSemaphore, but the end state is always _PEND rather than,
 * possibly _DONE
 *
 * Usable in either USER or KERNEL code.
 *
 * PASSED:       R25:R24 = Semaphore
 * RETURNS:      
 * USES:         Z, X, flags
 * STACK:
 * SIDE EFFECTS: 
 * Notes:        It doesn't make sense to reset a semaphore that has
 *               a process waiting, so just skip that situation.
 */
  .global vAvrXIntResetObjectSemaphore
  .global vAvrXResetObjectSemaphore
  .func vAvrXResetObjectSemaphore
// -----------------------------------------------------------------------------
vAvrXResetObjectSemaphore:
vAvrXIntResetObjectSemaphore:
    mov     Zl, p1l
    mov     Zh, p1h
    adiw    Zl, 2
    rjmp    ars00

  .endfunc

/* -----------------------------------------------------------------------------
 * void vAvrXResetSemaphore (xSem *)
 * void vAvrXIntResetSemaphore (xSem *)
 */
  .global vAvrXResetSemaphore
  .func vAvrXResetSemaphore
  .global vAvrXIntResetSemaphore
// -----------------------------------------------------------------------------
vAvrXResetSemaphore:
vAvrXIntResetSemaphore:
    mov     Zh, p1h
    mov     Zl, p1l
    
ars00:
    BeginCritical
    ldd     tmp2, Z+NextL
    ldd     tmp3, Z+NextH
    subi    tmp2, lo8(_DONE)
    sbci    tmp3, hi8(_DONE)
    breq    ars01           // If it was _DONE, now it is _PEND
                            // otherwize restore Pointer
    subi    tmp2, lo8(-_DONE)
    sbci    tmp3, hi8(-_DONE)
    
ars01:
    std     Z+NextH, tmp3 
    std     Z+NextL, tmp2
    EndCriticalReturn

  .endfunc

/* ========================================================================== */
  .end
