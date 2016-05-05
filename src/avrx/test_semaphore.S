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
 * @file test_semaphore.S
 * @brief Test Sémaphore
 * @author Larry Barello, <larry@barello.net>. 
 *         Copyright © 1998-2002 All rights reserved.
 *
 * Revision History
 ---
 *    20110830 - Initial port to AvrIO by epsilonRT
 */
#include "avrx.inc"

  .section .text.avrx_atest_semaphore,"ax",@progbits
/* ========================================================================== */

  .text
/* -----------------------------------------------------------------------------
 * xAvrXTestSemaphore
 * AvrXTIntTestSemaphore
 *
 * Non blocking call to check state of the Semaphore.
 *
 * xAvrXTestObjectSemaphore
 * xAvrXTestTimer
 * xAvrXTestPid
 * xAvrXTestMessageAck
 *
 * Non blocking version of vAvrXWaitObjectSemaphore
 *
 * PASSED:       R25:R24 = Semaphore/Object
 * RETURNS:      R25:R24 = 0 - _PEND
 *                       = 1 - _DONE
 *                       = 2 - something is waiting on it.
 * USES:         Z, X
 * STACK:
 * SIDE EFFECTS:	If semaphore is _DONE, we return _DONE, but reset semaphore to _PEND
 * NOTES:
 */
  .global xAvrXTestTimer
  .func xAvrXTestTimer
  .global xAvrXTestPid
  .global xAvrXTestMessageAck
  .global xAvrXTestObjectSemaphore
// -----------------------------------------------------------------------------
xAvrXTestTimer:
xAvrXTestPid:
xAvrXTestMessageAck:
xAvrXTestObjectSemaphore:
    subi    p1l, lo8(-2)
    sbci    p1h, hi8(-2)

  .endfunc

  .global xAvrXIntTestSemaphore
  .func xAvrXIntTestSemaphore
  .global xAvrXTestSemaphore
// -----------------------------------------------------------------------------
xAvrXIntTestSemaphore:
xAvrXTestSemaphore:
    mov     Zh, p1h
    mov     Zl, p1l
    ldi     p1l, lo8(_DONE)     // Preload _DONE (1) return
    ldi     p1h, hi8(_DONE)

    BeginCritical
    ldd     tmp0, Z+NextL
    ldd     tmp1, Z+NextH
    sub     tmp0, p1l
    sbc     tmp1, p1h
    breq    aits01

    brlo    aits00          // if > _DONE return 2
    subi    r1l, lo8(-2)	// very tricky... = 3
    //        sbci    r1h, hi8(-2)	// falls through and decrements.
aits00:
    subi    r1l, lo8(1)	//
    //        sbci    r1h, hi8(1)    // if < _DONE return 0
    EndCriticalReturn
aits01:
    std     Z+NextH, tmp0    // Store _PEND (0)
    std     Z+NextL, tmp1    // Return _DONE (R25:R24)
    EndCriticalReturn

  .endfunc

/* ========================================================================== */
  .end
