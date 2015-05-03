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
