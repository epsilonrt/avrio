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
 * @file time_queue.S
 * @brief Time Queue
 * @author Larry Barello, <larry@barello.net>. 
 *         Copyright _ 1998-2002 All rights reserved.
 *
 * Revision History
 ---
 *    20110830 - Initial port to AvrIO by epsilonRT
 *    20010403 - Initial update for avrx-ctoasm.inc port
 */
#include "avrx.inc"

  .section .text.avrx_time_queue,"ax",@progbits
/* ========================================================================== */
  .extern vAvrXIntSendMessage
  .extern vAvrXIntSetObjectSemaphore
  .extern vAvrXSetObjectSemaphore
  .extern vAvrXIntResetObjectSemaphore
  .extern vAvrXWaitTimer
  .extern vAvrXIntProlog

  .extern vAvrXIntEpilog

  .text
/* -----------------------------------------------------------------------------
 * void vAvrXDelay(pTcb, unsigned)
 *
 * Passed:       p1h:p1l = TCB
 *               p2h:p2l = Count
 * Returns:      
 * Uses:         
 * Stack: 
 * Notes:        See vAvrXStartTimer, vAvrXWaitTimer
 */
  .global vAvrXDelay
  .func vAvrXDelay
// -----------------------------------------------------------------------------
vAvrXDelay:
    rcall   vAvrXStartTimer
    rjmp    vAvrXWaitTimer

  .endfunc

/* -----------------------------------------------------------------------------
 * void vAvrXStartTimer(pTcb, unsigned)
 *
 * Passed:       p1h:p1l = TCB
 *               p2h:p2l = Count
 * Returns:
 * Uses:
 * Stack:        
 * Notes:        Should check and halt if TCB already queued
 *               Resets TCB Semaphore  If count Zero, just flag
 *               semaphore and return
 */
  .global vAvrXStartTimer
  .func vAvrXStartTimer
// -----------------------------------------------------------------------------
vAvrXStartTimer:
    subi    p2l, lo8(-0)
    sbci    p2h, hi8(-0)
    brne    prvvTimerCountNotZero
    rjmp    vAvrXSetObjectSemaphore

  .global prvvTimerCountNotZero
// -----------------------------------------------------------------------------
prvvTimerCountNotZero:        
    AVRX_Prolog

    rcall   vAvrXIntResetObjectSemaphore

    ldi     Zl, lo8(pxTimerQueue)
    ldi     Zh, hi8(pxTimerQueue)
    
    BeginCritical
    lds     tmp0, ucTimQLevel
    dec     tmp0
    sts     ucTimQLevel, tmp0
    EndCritical

ast00:
    mov     Yl, Zl          // Y -> Previous
    mov     Yh, Zh
    ldd     Zl, Y+NextL     // Z -> Current
    ldd     Zh, Y+NextH
    adiw    Zl, 0
    breq    ast01           // End of queue, Wrap up.

    ldd     tmp0, Z+TcbCount+NextL// tmp = Current count
    ldd     tmp1, Z+TcbCount+NextH// p2 = our count

    sub     p2l, tmp0
    sbc     p2h, tmp1         // Subtract it from us
    brsh    ast00           // Its less than us, continue walking the list

    add     p2l, tmp0         // Restore us
    adc     p2h, tmp1
    sub     tmp0, p2l         // Subtract us from it
    sbc     tmp1, p2h

    std     Z+TcbCount+NextL, tmp0
    std     Z+TcbCount+NextH, tmp1  // Put it back out and insert us in front.

/*
 * Wrap up: Z->Current, P1->Tcb, Y->Prev, P2 = Count
 * Insert Tcb into chain
 */
ast01:
    std     Y+NextH, p1h
    std     Y+NextL, p1l    // Prev.next = NewTCB
    mov     Yh, p1h
    mov     Yl, p1l
    std     Y+NextH, Zh     // NewTCB.next = Current
    std     Y+NextL, Zl
    std     Y+TcbCount+NextL, p2l
    std     Y+TcbCount+NextH, p2h // NewTCB.Count = count

    rcall   TimerHandler   // process any nested timer interrupts

    rjmp    vAvrXIntEpilog

  .endfunc

/* -----------------------------------------------------------------------------
 * void vAvrXTimerHandler(void)
 *
 * THIS IS INTERRUPT CODE and must be called within an
 * _Prolog/vAvrXIntEpilog section.
 *
 * The register variable ucTimQLevel is used to count re-
 * entry into the code.  It is also a semaphore that prevents
 * timer interrupts from hosing the vAvrXStartTimer code, above.
 * Thus, the entire timer handler chain can be run with interrupts
 * enabled.
 *
 * Since this can be called from C code (avrx 2.5/IAR) gotta preserve everything
 * but Z and tmp0-4.  System calls within can and do trash the trashable
 * registers, hence all the push/pop's
 */
  .global vAvrXTimerHandler
  .func vAvrXTimerHandler
// -----------------------------------------------------------------------------
vAvrXTimerHandler:
    BeginCritical
    lds     tmp0, ucTimQLevel
    subi    tmp0, 1          // Can't use "dec" because doesn't affect 
    sts     ucTimQLevel, tmp0 // carry flag.
    EndCritical
    brcs    ati00
    ret                     // Do not branch on rollover (0->FF)

ati00:
    push    Yl              // Gotta save these since we do not know
    push    Yh              // who called us (C code)
    push    Xl
    push    Xh
    lds     Yh, pxTimerQueue+NextH
    lds     Yl, pxTimerQueue+NextL
    adiw    Yl, 0
    breq    ati02           // Empty queue, return
/*
 * Y points to the first element in the queue.
 */
    ldd     Zh, Y+TcbCount+NextH
    ldd     Zl, Y+TcbCount+NextL        
    sbiw    Zl, 1           // Y->Count--
    std     Y+TcbCount+NextH, Zh
    std     Y+TcbCount+NextL, Zl

ati01:
    or      Zl, Zh          // While (Y->Count == 0)
    brne    ati02           // {

    ldd     Xl, Y+NextL
    ldd     Xh, Y+NextH
    sts     pxTimerQueue+NextH, Xh // Point queue to (new) first element
    sts     pxTimerQueue+NextL, Xl
    std     Y+NextH, Zh     //   Zero out link
    std     Y+NextL, Zl
    
    //        push    Zl
    //        push    Zh
    ldd     p1l, Y+TcbSemaphore+NextL
    ldd     p1h, Y+TcbSemaphore+NextH
    subi    p1l, lo8(TIMERMESSAGE_EV)
    sbci    p1h, hi8(TIMERMESSAGE_EV)
    brne    ati04
    ldd     p1l, Y+TcbQueue+NextL
    ldd     p1h, Y+TcbQueue+NextH
    mov     p2h, Yh
    mov     p2l, Yl
    rcall   vAvrXIntSendMessage
    rjmp    ati03

ati04:
    mov     p1l, Yl
    mov     p1h, Yh
    rcall   vAvrXIntSetObjectSemaphore

ati03:
    //        pop     Zh
    //        pop     Zl

    adiw    Xl, 0           //   If Next Tcb == 0 (list empty)
    breq    ati02           //       break//
                            //   else
    mov     Yl, Xl
    mov     Yh, Xh
    ldd     Zh, Y+TcbCount+NextH //       load up count and loop.
    ldd     Zl, Y+TcbCount+NextL
    rjmp    ati01           // }

ati02:
    pop     Xh
    pop     Xl
    pop     Yh
    pop     Yl

TimerHandler:
    BeginCritical
    lds     tmp0, ucTimQLevel
    inc     tmp0
    sts     ucTimQLevel, tmp0
    EndCritical
    brne    ati00
    ret

  .endfunc

/* ========================================================================== */
  .end
