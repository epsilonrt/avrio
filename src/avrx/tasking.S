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
 * @file tasking.S
 * @author Larry Barello, <larry@barello.net>. 
 *         Copyright © 1998-2002 All rights reserved.
 *
 * Revision History
 *    20110830 - Initial port to AvrIO by epsilonRT
 *    20050913 - Modified QueuePid() to preserve the I flag across critical sections.
 *    20020907 - Added _SFR_IO_ADDR() macro to all I/O accesses for GCC 3.2
 *    20020603 - Initial rewrite of _Prolog to skip context push when in idle task.
 *	           - modify vTaskQueuePid to return status if top of run queue changed.
 *    20010515 - Added support for IAR c_stack (initialize Y in task def)
 *    20010403 - Ported to avrx-ctoasm.inc
 *    20010307 - Moved AvrXStack into RAM variable so it can be set from main().
 *             - Added function pvAvrXSetKernelStack
 * ---
 * @brief Gestion multitâche
 * 
 * Ce module fournit toutes les fonctions de base pour gérer le multitâche.
 * Il constitue le noyau d'AvrX et fournit les API suivantes:
 * - vAvrXIntProlog
 * - vAvrXIntEpilog
 * - vAvrXEpilog
 * - pvAvrXSetKernelStack
 * - vAvrXStart
 * - vAvrXRunTcb
 * - xAvrXInitTcb
 * - vAvrXResume
 * .
 * Partie privée (appel en asm seulement) :
 * - vTaskQueuePid()
 * .
 */
#include "avrx.inc"

/* ========================================================================== */
  .extern vAvrXGenerateInterrupt

/* -----------------------------------------------------------------------------
 * void vAvrXIntProlog(void)
 *
 * Pushes entire register context onto the stack, returning a frame pointer
 * to the saved context.  If running in user mode upon entry (SysLevel == 0xFF)
 * then switches the stack to the kernel and stores the frame pointer in the
 * current processes PID.
 *
 * PASSED:       Nothing
 * RETURN:       Y = Frame Pointer (See AvrX.inc)
 *               R0 = pid.state (Single Step Flag) or 0
 * ASSUMES:
 * USES:         R14-R18, R26-R32, SysLevel
 * CALLS:
 * NOTES:        Does not muck with R16-R25 so passed parameters from C to
 *               AvrX functions can be accessed directly after a call to Prolog.
 *
 *		Special case for IDLE: Don't save context or restore context when
 *		transitioning from USER to Kernel context - it has no context when in
 *		in user mode.  Don't put any code into the IDLE task - it will break!
 */
FUNCTION(vAvrXIntProlog)
    push	  R29
    push	  R28
    push	  R27
    push	  R26
    push	  R25

    ldi	    Yh, hi8(xKernelData)
    ldi	    Yl, lo8(xKernelData)

    in	    R25, _SFR_IO_ADDR(SREG)		// Save flags

    ldd	    Xl, Y + SysLevel
    subi	  Xl, lo8(-1)		// Carry set if results 0
    std	    Y + SysLevel, Xl		// if already in kernel, then save context
    brcs	  .LIPSAVECONTEXT

    ldd	    Xl, Y + Running + NextL	// Check if current task = IDLE
    ldd	    Xh, Y + Running + NextH
    or	    Xh, Xl
    brne	  .LIPSAVECONTEXT		// Carry cleared if results 0
/*
 * When interrupting IDLE, just reset the stack pointer to PRIOR the interrupt.
 * in vAvrXEpilog, if still IDLE, control will return to the start of the IDLE 
 * loop. I.E. we don't need the return address to IDLE, just the return address 
 * to the caller of Prolog() (the interrupt handler).
 *
 * The code, below, is obscure, but cycle efficient.  The net effect is to reset
 * the stack to AvrXStack before returning to the interrupt handler.
 */
    in	    Yl, _SFR_IO_ADDR(SPL)	//Idle task: don't bother saving registers, etc.
    in	    Yh, _SFR_IO_ADDR(SPH)
    ldd	    Zh, Y + 6         	// Get return address
    ldd	    Zl, Y + 7
    adiw	  Yl, 9			// Adjust pointer
    out	    _SFR_IO_ADDR(SPL), Yl  	// This is cycle efficient, but obscure.
    out	    _SFR_IO_ADDR(SPH), Yh
    ijmp				// ~37 cycles for IDLE task.

.LIPSAVECONTEXT:
    push	  R24
    push    R23
    push    R22
    push    R21
    push    R20
    push    R19
    push    R18
    push    R17
    push    R16
    push    R15
    push    R14
    push    R13
    push    R12
    push    R11
    push    R10
    push    R9
    push    R8
    push    R7
    push    R6
    push    R5
    push    R4
    push    R3
    push    R2
    push    R1
    push    R0
    push    R25		// SREG

    in	    Yl, _SFR_IO_ADDR(SPL)
    in	    Yh, _SFR_IO_ADDR(SPH)         // Grab frame pointer

    ldd	    R25, Y + _R25	// Need to restore this register (used for SREG)
    ldd	    tmp1, Y + _RetLo
    ldd	    tmp2, Y + _RetHi
    std     Y + _R30, Zl
    std     Y + _R31, Zh      // Swap return address with Z

#ifdef AVRX_SINGLESTEP_ENABLE
    ldi	    Xl, 0
    mov	    R0, Xl
#endif /* AVRX_SINGLESTEP_ENABLE */

    brcs	  .LIPALREADYINKERNEL // Flags, still good, from above.

    ldi	    Zh, hi8(xKernelData)
    ldi	    Zl, lo8(xKernelData)
    ldd	    Yh, Z + Running + NextH    // Point to running task
    ldd	    Yl, Z + Running + NextL
    in	    Xh, _SFR_IO_ADDR(SPH)
    in	    Xl, _SFR_IO_ADDR(SPL)
    std	    Y + PidSP + NextH, Xh
    std	    Y + PidSP + NextL, Xl

#ifdef  AVRX_SINGLESTEP_ENABLE
    ldd     tmp0, Y + PidState // Return to caller SingleStepFlag.
    sbrs    tmp0, SingleStep
    rjmp    .LIPSKIPDEQUEUE

    ldd     R1, Y + PidNext + NextH  // Dequeue current task
    std     Z + RunQueue + NextH, R1
    ldd     R1, Y + PidNext + NextL
    std     Z + RunQueue + NextL, R1
    mov     R0, tmp0        // Return to caller SingleStepFlag.
    cbr     tmp0, _BV(SingleStep)
    std     Y + PidState, tmp0 // Clear flag in PID
.LIPSKIPDEQUEUE:
#endif /* AVRX_SINGLESTEP_ENABLE */

    ldd	    tmp0, Z + AvrXStack + NextL
    out	    _SFR_IO_ADDR(SPL), tmp0
    ldd	    tmp0, Z + AvrXStack + NextH
    out	    _SFR_IO_ADDR(SPH), tmp0       // Swap to kernel stack
    mov	    Yl, Xl
    mov	    Yh, Xh		// restore frame pointer

.LIPALREADYINKERNEL:      // (85/102)
    clr     R1        // R1 = __Zero__ for Avr-gcc
    mov     Zl, tmp1  // scratch in IARICC
    mov     Zh, tmp2
    ijmp		      	  // Return to caller (89/106)

/* -----------------------------------------------------------------------------
 * void vAvrXStart (void)
 *
 * Démarre le noyau AvrX et ne retourne jamais
 */
FUNCTION(vAvrXStart)
    rcall   vAvrXEpilog
.LSLOOP:
    rjmp    .LSLOOP

/* -----------------------------------------------------------------------------
 * vAvrXEpilog
 *
 * Restore previous context (kernel or user).  If task has SingleStep flag
 * set, then generate an interrupt before returning to the task.
 */
FUNCTION(vAvrXEpilog)
    pop     R30
    pop     R30
 
/* -----------------------------------------------------------------------------
 * vAvrXIntEpilog
 *
 * Restore previous context (kernel or user).
 * If task has SingleStep flag set, then generate an interrupt
 * before returning to the task.
 *
 * PASSED:
 * RETURN:
 * ASSUMES:      SysLevel >= 0 (running on kernel stack)
 * USES:         SysLevel
 * CALLS:
 */
FUNCTION(vAvrXIntEpilog)
    ldi	    Zl, lo8(xKernelData)
    ldi	    Zh, hi8(xKernelData)
    BeginCritical     // Interrupts off..
    ldd     R16, Z + SysLevel
    dec     R16
    std	    Z + SysLevel, R16
    brge    .LIESKIPTASKSWAP

    ldd     Yh, Z + RunQueue + NextH
    ldd     Yl, Z + RunQueue + NextL
    std     Z + Running + NextH, Yh
    std     Z + Running + NextL, Yl   // Update current running task
    adiw    Yl, 0
    breq    .LIEIDLETASK

    ldd     Xh, Y + PidSP + NextH
    ldd     Xl, Y + PidSP + NextL
    out     _SFR_IO_ADDR(SPL), Xl
    out     _SFR_IO_ADDR(SPH), Xh         // 20 cycles
#ifdef  AVRX_SINGLESTEP_ENABLE
    ldd     R0, Y + PidState  // X, Y and R0 available
    sbrc    R0, SingleStep
    rcall   vAvrXGenerateInterrupt
#endif

.LIESKIPTASKSWAP:                   // 20/6
    pop     R0
    out     _SFR_IO_ADDR(SREG), R0
    pop     R0
    pop     R1
    pop     R2
    pop     R3
    pop     R4
    pop     R5
    pop     R6
    pop     R7
    pop     R8
    pop     R9
    pop     R10
    pop     R11
    pop     R12
    pop     R13
    pop     R14
    pop     R15
    pop     R16
    pop     R17
    pop     R18
    pop     R19
    pop     R20
    pop     R21
    pop     R22
    pop     R23
    pop     R24
    pop     R25
    pop     R26
    pop     R27
    pop     R28
    pop     R29
    pop     R30
    pop     R31
    EndCriticalReturn       // 97/83 cycles with interrupts off

/* 
 * Jump here if there are no entries in the _RunQueue.  Never return.  
 * vAvrXEpilog will take care of that.  NB - this code has *NO* context.  
 * Do not put anything in here that uses registers as you will get hosed every 
 * time an interrupt occurs.
 */

.LIEIDLETASK:
    //	ldd	Yl, Z + AvrXStack + NextL
    //	ldd	Yh, Z + AvrXStack + NextH
    //	out	SPL, Yl
    //	out	SPL, Yh		// Reset kernel stack
    BeginIdle               // Any interrupt will EndIdle
    EndCritical             // Enable interrupts
    sleep                   // Power Down..
    EndIdle
    rjmp    .LIEIDLETASK

/* -----------------------------------------------------------------------------
 * void * pvAvrXSetKernelStack(void * newstack)
 *
 * PASSED: Pointer to end of new stack or NULL
 * RETURN: pointer to end of stack
 */
FUNCTION(pvAvrXSetKernelStack)
    pop     R31
    pop     R30
    subi    p1l, 0		// May not be a high pair in IAR
    sbci    p1h, 0
    brne    .LSKS01
    in      p1l, _SFR_IO_ADDR(SPL)
    in      p1h, _SFR_IO_ADDR(SPH)
.LSKS01:
    sts     xKernelData+AvrXStack + NextL, p1l
    sts     xKernelData+AvrXStack + NextH, p1h
    ijmp

/* -----------------------------------------------------------------------------
 * PID AvrXInitProcess(*xTask)
 * PID vAvrXRunTcb(*xTask)
 *
 * Initialize the PID based upon the TCB table.  Run Task
 * "resumes" the process as well
 *
 * PASSED:       R25:R24 = TCB
 * RETURNS:      R25:R24 = PID
 * USES:         X, Z and R0
 * CALLS:
 * NOTES:        TCB, Layout, below:
 *               2 PID
 *               1 Flags
 *               2 STACK
 *               2 Entry
 */
FUNCTION(vAvrXRunTcb)
    rcall   xAvrXInitTcb
    rjmp    vAvrXResume

  /* xPid xAvrXInitTcb (xTcb * pxTcb)
   * Initialise une tâche
   *
   * Initialise une tâche en initialisant et en sauvegardant son contexte sur
   * la pile. Le contexte est constitué de tous les registres de calcul
   * du processeur. Tous ces registres sont initialisés à zéro.
   *
   * PASSED:  p1 Task Block Control in Flash
   * RETURNS: r1 PID
   * USES:    -
   * STACK:   -
   */  
FUNCTION(xAvrXInitTcb)
    mov     Zl, p1l
    mov     Zh, p1h

    rcall   prvucLpmInc  // Pointeur de pile du processus dans X
    mov     Xl, R0
    rcall   prvucLpmInc
    mov     Xh, R0

#ifdef  AVRX_TASKEXIT_ENABLE
  .extern vAvrXTaskExit

    ldi     tmp2, lo8_pm(vAvrXTaskExit)
    st      X, tmp2
    ldi     tmp2, hi8_pm(vAvrXTaskExit)
    st      -X, tmp2 // Empile l'adresse d'exécution de vAvrXTaskExit

    rcall   prvucLpmInc
    st      -X, R0
    rcall   prvucLpmInc
    st      -X, R0   // Empile l'adresse d'exécution du processus

#else
    // Empile l'adresse d'exécution du processus
    rcall   prvucLpmInc
    st      X, R0
    rcall   prvucLpmInc
    st      -X, R0

#endif /* AVRX_TASKEXIT_ENABLE */

    // Remplit le reste de la pile avec des 0
    ldi     tmp2, 0
    ldi     tmp3, 33        // R0-R31 and SREG get "pushed"
.LITPUSHREGISTERS:
    st      -X, tmp2
    dec     tmp3
    brne    .LITPUSHREGISTERS   

    sbiw    Xl, 1           // because pre dec is not a push!

    rcall   prvucLpmInc
    mov     r1l, R0
    rcall   prvucLpmInc
    mov     r1h, R0         // Récupère l'adresse du processus (PID) dans r1
    rcall   prvucLpmInc     // R0 contient la priorité
    mov     Zl, r1l
    mov     Zh, r1h         // Z pointe le PID
    
    std     Z + PidSP + NextL, Xl
    std     Z + PidSP + NextH, Xh  // Sauve le pointeur de pile dans le PID

    std     Z + PidPriority, R0    // Sauve la priorité dans le PID
    
    ldi     Xl, _BV(SuspendedBit) | _BV(SuspendBit)
    std     Z + PidState, Xl        // Indique que le processus peut être lancé
    
    std     Z + PidNext + NextH, tmp2
    std     Z + PidNext + NextL, tmp2 // Prochain PID = 0 (plus de processus après)
    ret

prvucLpmInc:  // Transfert l'octet de FLASH pointé par Z dans R0 et incrémente Z
    lpm
    adiw    Zl, 1
    ret

/* -----------------------------------------------------------------------------
 * void vAvrXResume(xPid)
 *
 * Takes a PID and inserts it into the run queue.
 *
 * Uses two flags, Suspended and Suspend to determine if a task is
 * actually suspended, as opposed to just being marked for suspension.
 * Mark _SUSP to make QueuePid not queue, Mark _SUSPENDED when attempt
 * to queue is made (e.g. not blocked on a semaphore).  This is because
 * we can't "resume" something that is blocked on a semaphore, even though
 * it is marked for suspension.
 *
 * PASSED:       R25:24 = PID to queue
 * RETURNS:
 * USES:
 * ASSUMES:
 * NOTES:
 */
FUNCTION(vAvrXResume) // User Entry Point
    AVRX_Prolog

    mov     Zl, p1l
    mov     Zh, p1h

    ldd     tmp0, Z + PidState
    cbr     tmp0, _BV(SuspendBit)
    std     Z + PidState, tmp0    // clear Suspend flag

    sbrs    tmp0, SuspendedBit
    rjmp    .LREXIT

    cbr     tmp0, _BV(SuspendedBit)   // If removed from run queue,
    std     Z + PidState, tmp0        // Clear flag and queue it up.

    rcall   vTaskQueuePid          // If found, then add to run queue
.LREXIT:
    rjmp    vAvrXIntEpilog

/* -----------------------------------------------------------------------------
 * void vTaskQueuePid(xPid)
 *
 * Takes a PID and inserts it into the run queue.  The run queue is sorted
 * by priority.  Lower numbers go first.  If there are multiple tasks of equal
 * priority, then the new task is appended to the list of equals (round robin)
 *
 * PASSED:       p1h:p1l = PID to queue
 * RETURNS:      r1l:	-1  = suspended
 *			              0   = Top of run queue
 *			              1-N = Depth in run queue
 * USES:         Z, tmp0-3 and SREG, RunQueue
 * ASSUMES:
 * NOTES:        Returns with interrupts on.
 *		// 9/13/04 Preserves INTERRUPTS
 */
FUNCTION(vTaskQueuePid) // Kernel entry point only
    mov     Zl, p1l
    mov     Zh, p1h
    ldi	    tmp1, lo8(-1)

    ldd     tmp0, Z + PidState          // Xh = Priority & Flags
    andi    tmp0, (_BV(SuspendBit) | _BV(IdleBit)) // if marked Suspended or idle
    brne    .LTQPSUSPEND

    //  push	Yl		// 9/13/04
    //  mov	tmp0, Yh	// preserve Y
    push	  Yl		// 9/13/04
    push	  Yh		// 9/13/04

    ldd     tmp2, Z + PidPriority
    ldi     Yl, lo8(xKernelData+RunQueue)
    ldi     Yh, hi8(xKernelData+RunQueue)
    //  BeginCritical	// 9/13/04
    in	    tmp0, _SFR_IO_ADDR(SREG)
    cli		// 9/13/04
    
.LTQP00:
    inc	    tmp1     		// Tmp1 = counter of insertion point.
    mov     Zl, Yl                 	// 0 = head of run queue.
    mov     Zh, Yh
    ldd     Yl, Z + PidNext + NextL
    ldd     Yh, Z + PidNext + NextH     // Z = current, X = Next
    adiw    Yl, 0
    breq    .LTQP01                   // End of queue, continue
    ldd     tmp3, Y + PidPriority
    cp      tmp2, tmp3
    brsh    .LTQP00                   // Loop until pri > PID to queue

.LTQP01:
    std     Z + NextH, p1h
    std     Z + NextL, p1l    // Prev->Next = Object
    mov     Zh, p1h
    mov     Zl, p1l
    std     Z + NextH, Yh     // Object->Next = Next
    std     Z + NextL, Yl
    //  pop     Yl		// 9/13/04
    //  mov     Yh, tmp0	// 9/13/04
    pop	    Yh		// 9/13/05
    pop	    Yl		// 9/13/04
    mov	    r1l, tmp1
    //  EndCriticalReturn	// 9/13/04
    out	    _SFR_IO_ADDR(SREG), tmp0
    ret			// 9/13/04

.LTQPSUSPEND:
    mov	    r1l, tmp1
    sbr     tmp0, _BV(SuspendedBit)  // Mark suspended and return
    std     Z + PidState, tmp0
    //  EndCriticalReturn	// 9/13/04
    ret		// 9/13/04

/* ========================================================================== */
  .end
