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
 * @file recv_message.S
 * @brief pxAvrXRecvMessage
 * @author Larry Barello, <larry@barello.net>. 
 *         Copyright _ 1998-2002 All rights reserved.
 *
 * Revision History
 ---
 *    20110830 - Initial port to AvrIO by epsilonRT
 */
#include "avrx.inc"

  .section .text.avrx_recvmessage,"ax",@progbits
/* ========================================================================== */
  .extern vAvrXResetSemaphore
  .extern prvvSemRemoveFirstObject

  .text
/* -----------------------------------------------------------------------------
 * pMessage pxAvrXRecvMessage(xMessageQueue *)
 *
 * Removes the first item in a queue. Return Zero if empty.
 *
 * PASSED:       R25, R24 = Queue head
 * RETURNS:      R25, R24 = Object = 0 if list empty
 * USES:         Z, X, Flags, R23:R22
 * CALLS:
 * ASSUMES:      Null terminated list
 * NOTES:        If queue empty, then reset queue semaphore
 *               
 */
  .global pxAvrXRecvMessage
  .func pxAvrXRecvMessage
// -----------------------------------------------------------------------------
pxAvrXRecvMessage:
    mov     Zl, p1l
    mov     Zh, p1h
    BeginCritical
    rcall   prvvSemRemoveFirstObject
    subi    p1l, lo8(-2)
    sbci    p1h, hi8(-2)
    rcall   vAvrXResetSemaphore      // Note, interrupt enabled here
    mov     r1l, p2l
    mov     r1h, p2h
    EndCriticalReturn

  .endfunc

/* ========================================================================== */
  .end
