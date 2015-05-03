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
 * @file message.S
 * @brief Message Module
 * @author Larry Barello, <larry@barello.net>. 
 *         Copyright _ 1998-2002 All rights reserved.
 *
 * Revision History
 ---
 *    20110830 - Initial port to AvrIO by epsilonRT
 *    20010403 - Initial update for avrx-ctoasm.inc port
 */
#include "avrx.inc"

  .section .text.avrx_message,"ax",@progbits
/* ========================================================================== */
  .extern vAvrXWaitObjectSemaphore
  .extern vAvrXResetSemaphore
  .extern vAvrXResetObjectSemaphore
  .extern vAvrXIntProlog
  .extern vAvrXIntSetObjectSemaphore
  .extern vAvrXIntEpilog
  .extern prvvSemAppendObject
  .extern prvvSemRemoveFirstObject

  .text
/* -----------------------------------------------------------------------------
 * void vAvrXSendMessage(xMessageQueue *, xMessage *)
 *
 * PASSED:       p1h:p1l = Queue head
 *               p2h:p2l = Message
 * RETURNS:      
 * USES:         
 * CALLS:
 * ASSUMES:
 * NOTES:
 */
  .global vAvrXSendMessage
  .func vAvrXSendMessage
// -----------------------------------------------------------------------------
vAvrXSendMessage:
    AVRX_Prolog
    rcall   vAvrXIntSendMessage
    rjmp    vAvrXIntEpilog

  .endfunc

/* -----------------------------------------------------------------------------
 * void vAvrXIntSendMessage(xMessageQueue *, xMessage *)
 *
 * PASSED:       p1h:p1l = Queue head
 *               p2h:p2l = Message
 * RETURNS:      
 * USES:         Z, Y & some others.  See vAvrXIntSetObjectSemaphore
 * CALLS:
 * ASSUMES:      Called within an vAvrXIntEpilog/_Prolog wrapper
 * NOTES:        This could be called from interrupt code
 *               but you would need to make sure there is 
 *               only one source of messages for that queue
 *               or wrap _appendObject in a critical section.
 */
  .global vAvrXIntSendMessage
  .func vAvrXIntSendMessage
// -----------------------------------------------------------------------------
vAvrXIntSendMessage:
    // rcall   vAvrXResetObjectSemaphore
    mov     Zh, p1h
    mov     Zl, p1l
    rcall   prvvSemAppendObject   // Append the message onto the queue
    rjmp    vAvrXIntSetObjectSemaphore

  .endfunc

/* -----------------------------------------------------------------------------
 * xMessage * pxAvrXWaitMessage(xMessageQueue *)
 *
 * PASSED:       p1h:p1l = Queue head
 * RETURNS:      r1h:r1l = Message
 * USES:         Z, flags (See prvvSemRemoveFirstObject)
 * CALLS:
 * ASSUMES:      Null terminated list
 * NOTES:        pxAvrXRecvMessage is atomic for append-only queues
 *               just push registers to be saved
 */
  .global pxAvrXWaitMessage
  .func pxAvrXWaitMessage
// -----------------------------------------------------------------------------
pxAvrXWaitMessage:
    mov     Zl, p1l
    mov     Zh, p1h
    BeginCritical
    rcall   prvvSemRemoveFirstObject
    EndCritical
    brne    _rm01

    rcall   vAvrXWaitObjectSemaphore
    subi    p1l, lo8(2)
    sbci    p1h, hi8(2)             // p1 buggered by WaitObject
    rjmp    pxAvrXWaitMessage
_rm01:
    rcall   vAvrXResetObjectSemaphore      // Clear possible _PEND
    mov     p1l, p2l
    mov     p1h, p2h
    ret

  .endfunc

/* ========================================================================== */
  .end
