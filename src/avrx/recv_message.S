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
