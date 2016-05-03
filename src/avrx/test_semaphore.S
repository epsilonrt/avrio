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
