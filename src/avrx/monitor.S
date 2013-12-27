/**
 * @file monitor.S
 * @brief Monitor module
 * @author Larry Barello, <larry@barello.net>. 
 *         Copyright _ 1998-2002 All rights reserved.
 * @author Copyright _ 2011-2012 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * @version $Id$
 * Revision History ---
 *    20110830 - Initial port to AvrIO by epsilonRT
 *    9/13/2005 - Added EXTERN (vAvrXTerminate)
 *    6/4/2002  - Fixed memory address increment bug in modify memory command
 *              - added command to terminate a process
 *    5/2001    - rewrote to be mostly C compatible.  R4-R15
 *                used for locals (no statics)
 */
 #include "monitor.inc"

/* constants ================================================================ */
#define SRAM_BASE 0x0060

/* external functions ======================================================= */
  .extern vAvrXSetSemaphore
  .extern vAvrXSuspend
  .extern ucAvrXStepNext
  .extern vAvrXTerminate
  .extern vAvrXResume
  .extern pxAvrXCancelTimer
  .extern xAvrXInitTcb
  .extern xAvrXSingleStepNext

  .extern vMonitorPrintHelp
  .extern pxMonitorGetFlagsAndAddress
  .extern pxMonitorGetObjectID
  .extern pvMonitorGetAddress
  .extern usMonitorGetByteWord
  .extern vMonitorPutByteWord
// .extern ucMonitorGetDumpLength


/* internal public functions ================================================ */

/* -----------------------------------------------------------------------------
 * vDebugMonitor: program for examining and modifying
 * registers, data, i/o and eeprom.
 *
 * NB    {opt, opt}      Must select one of the options
 *       [opt, opt]      Optional input - can be ignored
 *
 * Command summary:
 *
 *       cmd                     Action
 *       --------------------    ------
 *       m{s,e}[w][aaaa]         Modify SRAM/EEram by byte/word starting at AAAA
 *       d{s,e}[w][aaaa] [xxxx]  Dump XX Bytes/Words of Sram/EERam, starting at AAAA
 *       l                       List information about all processes
 *       h[xxxx]                 Halt process xxxx
 *       s[xxxx]                 Step Process xxxx (run until next kernel entry)
 *       n[xxxx]                 Step Process till next instruction
 *       r[xxxx]                 Resume Process xxxx
 *       x[xxxx]                 display context of xxxx (use 's' to modify)
 *       i[xxxx]                 Signal (set) Semaphore
 *       c[xxxx]                 cancel timer xxxx
 *	     k[xxxx]			           Kill process xxxx
 *       p[xxxx]                 Initialize Process (xAvrXInitTcb)
 *
 *
 * Stack Usage:
 *       12 or more bytes
 *
 * Register Usage:
 *       Everything
 *
 * Global data information
 * History ---
 *    20110912 - Add list process command
 *    20110830 - Initial port to AvrIO by epsilonRT (pj)
 * ---
 */
FUNCTION(vDebugMonitor)
    ldi     p1l, lo8(xEEPromSem)
    ldi     p1h, hi8(xEEPromSem)
    rcall   vAvrXSetSemaphore        // Initialize EEProm xSem
    rcall   vMonitorPrintHelp
    
.LDMINIT:
    ldi     tmp0, lo8(SRAM_BASE)
    mov     Addressl, tmp0
    ldi     tmp0, hi8(SRAM_BASE)
    mov     Addressh, tmp0
    clr     ObjectIDl
    clr     ObjectIDh
    clr     MonFlags
    clr     DumpLength
    clr     EEAddressl
    clr     EEAddressh

.LDMLOOP:
    rcall   vDebugPutCR
    ldi     p1l, '>'
    rcall   vDebugPutChar
    rcall   prvvParse
    rjmp    .LDMLOOP

/* private functions ======================================================== */
/* -----------------------------------------------------------------------------
 * void prvvParse()
 *
 */
prvvParse:
    rcall   cDebugGetCharEcho
    cpi     r1l, 'm'     // Modify Memory
    breq    .LPMODIFYMEM
    cpi     r1l, 'd'     // Dump Memory
    breq    .LPDUMPMEM
    cpi     r1l, 'l'     // List Processes
    breq    .LPLISTPROCESS
    cpi     r1l, 'h'     // Suspend Process
    breq    .LPHALTPROCESS
    cpi     r1l, 's'     // Step process
    breq    .LPSTEPPROCESS
    cpi	    r1l, 'k'     // Terminate Process
    breq	  .LPKILLPROCESS
    cpi     r1l, 'r'     // Resume Process
    breq    .LPRESUMEPROCESS
    cpi     r1l, 'i'     // Signal Semaphore
    breq    .LPSETSEMAPHORE
    cpi     r1l, 'c'     // Cancel Timer
    breq    .LPCANCELTIMER
    cpi     r1l, 'x'     // Print Process Context
    breq    .LPPRINTCONTEXT
    cpi     r1l, 'p'     // Init Task
    breq    .LPINITTASK
    cpi     r1l, '?'     // Help
    breq    .LPHELP
    cpi     r1l, 'q'     // Quit
    breq    .LPQUIT
#ifdef  AVRX_SINGLESTEP_ENABLE
    cpi     r1l, 'n'     // Next instruction
    breq    .LPSINGLESTEP
#endif

.LPERR:
    ldi     p1l, '?'
    rjmp    vDebugPutChar

.LPMODIFYMEM:
    mov     tmp0, MonFlags
    cbr     tmp0, _BV(DUMPFLAG)
    mov     MonFlags, tmp0
    rcall   pxMonitorGetFlagsAndAddress
    brtc    .LPERR
    rjmp    prvvModifyMemory

.LPDUMPMEM:
    mov     tmp0, MonFlags
    sbr     tmp0, _BV(DUMPFLAG)
    mov     MonFlags, tmp0
    rcall   pxMonitorGetFlagsAndAddress
    brtc    .LPERR 
    rjmp    prvvDumpMemory

.LPLISTPROCESS:
    rjmp    prvvListProcesses
    
.LPHALTPROCESS:
    rcall   pxMonitorGetObjectID
    brtc    .LPERR
    rjmp    vAvrXSuspend

.LPSTEPPROCESS:
    rcall   pxMonitorGetObjectID
    brtc    .LPERR
    rcall   ucAvrXStepNext
    tst     r1l
    brne    .LPNOTSUSPENDED
    ret

.LPKILLPROCESS:
    rcall	pxMonitorGetObjectID
    brtc	.LPERR
    rjmp	vAvrXTerminate

.LPRESUMEPROCESS:
    rcall   pxMonitorGetObjectID
    brtc    .LPERR
    rjmp    vAvrXResume

.LPSETSEMAPHORE:
    rcall   pxMonitorGetObjectID
    brtc    .LPERR
    rjmp    vAvrXSetSemaphore

.LPCANCELTIMER:
    rcall   pxMonitorGetObjectID
    brtc    .LPERR
    rjmp    pxAvrXCancelTimer

.LPPRINTCONTEXT:
    rcall   pxMonitorGetObjectID
    brtc    .LPERR
    rjmp    prvvPrintContext

.LPINITTASK:
    rcall   pvMonitorGetAddress
    brtc    .LPERR
    rjmp    xAvrXInitTcb

.LPHELP:
    rjmp    vMonitorPrintHelp

.LPQUIT:
    pop     Xl      // Hack to allow user i/f to call monitor
    pop     Xl
    ret

#ifdef AVRX_SINGLESTEP_ENABLE
.LPSINGLESTEP:
    rcall   pxMonitorGetObjectID
    brtc    .LPERR
    
.LPNEXTSTEP:
    rcall   xAvrXSingleStepNext // This will return after
    tst     r1l
    brne    .LPNOTSUSPENDED   // the step is done

    mov     Zh, ObjectIDh
    mov     Zl, ObjectIDl
    ldd     Yl, Z+PidSP+NextL // Y = Frame Pointer
    ldd     Yh, Z+PidSP+NextH

    ldd     p1l, Y+_RetLo+2   // Y = Address of Next inst
    ldd     p1h, Y+_RetHi+2

    ldd     Xh, Z+bp1+NextH   // Breakpoints are stored in
    ldd     Xl, Z+bp1+NextL   // the PID..
    adiw    Xl, 0
    breq    .LPBREAKPOINT
    cp      p1l, Xl
    cpc     p1h, Xh
    breq    .LPBREAKPOINT

    ldd     Xh, Z+bp2+NextH
    ldd     Xl, Z+bp2+NextL
    adiw    Xl, 0
    breq    .LPNEXTSTEP       // If we dont hit the breakpoint
    cp      p1l, Xl           // the monitor hangs here
    cpc     p1h, Xh
    brne    .LPNEXTSTEP

.LPBREAKPOINT:
    rjmp    vDebugPutHexWord  // Print out our breakpoint address
#endif  /* AVRX_SINGLESTEP_ENABLE defined */

.LPNOTSUSPENDED:
    rcall   vDebugPutCR
    mov	    p1l, ObjectIDl
    mov	    p1h, ObjectIDh
    rcall   vDebugPutHexWord
    ldi     p1l, lo8(.LPNotSuspendedStr)
    ldi     p1h, hi8(.LPNotSuspendedStr)
    rjmp    vDebugPutString

//---
.LPNotSuspendedStr:
  .asciz  " - Not Suspended!\r"
  .balign 2

/* -----------------------------------------------------------------------------
 * void prvvDumpMemory(char, void *)
 *
 * Passed:       p1  = Start Address
 *               p2l = number of bytes to dump 
 * Returns:      Nothing
 * Uses:         MonFlags (register)
 */
prvvDumpMemory:
    sbrs    MonFlags, WORDFLAG
    rjmp    .LDPMINIT

    cbr     p1l, 0x01        // force even address to start.
    sbrc    p2l, 0x00        // Add one for even count
    inc     p2l
    
.LDPMINIT:
    mov     TempWl, p1l
    mov     TempWh, p1h
    mov     r1, p2l
    
.LDPMPRINTADDRESS:
    rcall   vDebugPutCR
    mov     p1h, TempWh          // Spit out address
    mov     p1l, TempWl
    rcall   vDebugPutHexWord
    
.LDPMPRINTNEXT:
    rcall   vDebugPutSpace
    rcall   usMonitorGetByteWord
    sbrc    MonFlags, WORDFLAG
    rjmp    .LDPMPRINTWORD
    
    rcall   vDebugPutHexByte
    ldi     p2l, lo8(1)
    ldi     p2h, hi8(1)
    rjmp    .LDPMNEXTLINE
    
.LDPMPRINTWORD:
    rcall   vDebugPutHexWord
    ldi     p2l, lo8(2)
    ldi     p2h, hi8(2)
    
.LDPMNEXTLINE:
    add     TempWl, p2l
    adc     TempWh, p2h
    sub     r1, p2l
    breq    .LDPMEXIT
    
    mov     Zl, TempWl
    cbr     Zl, 0xF0
    breq    .LDPMPRINTADDRESS  // New line every 16 bytes.
    rjmp    .LDPMPRINTNEXT
    
.LDPMEXIT:
    ret

/* -----------------------------------------------------------------------------
 * void prvvModifyMemory(void *)
 *
 * PASSED:       p1 = address
 *
 * Uses:         MonFlags (register)
 */
prvvModifyMemory:
    mov     TempWl, p1l
    mov     TempWh, p1h
    
.LMMMODIFYMEMORY:
    rcall   vDebugPutCR
    mov     p1l, TempWl
    mov     p1h, TempWh
    rcall   vDebugPutHexWord
    rcall   vDebugPutSpace
    
.LMMNEXT:
    rcall   usMonitorGetByteWord
    sbrc    MonFlags, WORDFLAG
    rcall   vDebugPutHexWord
    sbrs    MonFlags, WORDFLAG
    rcall   vDebugPutHexByte
    
.LMMGETDATA:
    ldi     p1l, '-'
    rcall   vDebugPutChar
    sbrs    MonFlags, WORDFLAG
    rcall   ucDebugGetHexByte
    sbrc    MonFlags, WORDFLAG
    rcall   usDebugGetHexWord
    brtc    .LMMNODATA
    
.LMMWRITE:
    rcall   vMonitorPutByteWord
    rcall   vDebugPutSpace
    
.LMMGOTONEXT:
    ldi     Zl, lo8(1)
    ldi     Zh, hi8(1)
    sbrc    MonFlags, WORDFLAG
    adiw    Zl, 1
    add     TempWl, Zl
    adc     TempWh, Zh
    mov     Zl, TempWl
    cbr     Zl, 0xF0
    breq    .LMMMODIFYMEMORY   // Start a new line every 16 bytes
    rjmp    .LMMNEXT
    
.LMMNODATA:  // a non-hex was typed
    rcall   xDebugIsWhite
    brtc    .LMMDONE         // If not white, then error
    in	    tmp0, _SFR_IO_ADDR(SREG)
    tst     p2l
    brne    .LMMWRITE        // If some data entered, then write
    out	    _SFR_IO_ADDR(SREG), tmp0
    brne    .LMMGOTONEXT     // if CR then exit, otherwize, just go to next byte
    
.LMMDONE:
    ret

/* -----------------------------------------------------------------------------
 * void prvvListProcesses(void)
 *
 * List all running processes
 */
prvvListProcesses:
  
    ldi     p1l, lo8(.LLPHeaderStr)
    ldi     p1h, hi8(.LLPHeaderStr)
    rcall   vDebugPutString
    
    BeginCritical
    ldi     Yl, lo8(xKernelData + RunQueue)
    ldi     Yh, hi8(xKernelData + RunQueue)
    EndCritical
    ldd     p1l, Y + NextL	
    ldd     p1h, Y + NextH	
    
.LLPLOOP:
    adiw    p1l, 0
    breq    .LLPEXIT

    mov     Yl, p1l
    mov     Yh, p1h
    rcall   vDebugPutCR
    
    mov     p1l,Yl
    mov     p1h,Yh
    rcall   vDebugPutHexWord
    rcall   vDebugPutSpace
    
    ldd     p1l, Y + PidState
    rcall   vDebugPutHexByte
    rcall   vDebugPutSpace
    rcall   vDebugPutSpace
    
    ldd     p1l, Y + PidPriority
    rcall   vDebugPutHexByte
    rcall   vDebugPutSpace
    rcall   vDebugPutSpace
    
    BeginCritical
    ldd     p1l, Y + PidSP + NextL
    ldd     p1h, Y + PidSP + NextH
    EndCritical
    rcall   vDebugPutHexWord

    BeginCritical
    ldd     p1l, Y + PidNext + NextL
    ldd     p1h, Y + PidNext + NextH
    EndCritical
    rjmp    .LLPLOOP
    
.LLPEXIT:
    ret

//---
.LLPHeaderStr:
  .asciz  "\rPID  Flg Pri Frame"
  .balign 2

/* -----------------------------------------------------------------------------
 * void prvvPrintContext(ObjectID *)
 *
 * Prints a pretty picture of the context of a process.
 *
 * PASSED:       nothing
 * RETURNS:
 * USES:         ObjectID (register)
 */
prvvPrintContext:

    ldi     p1h, hi8(.LPCCTDT0)
    ldi     p1l, lo8(.LPCCTDT0)
    rcall   vDebugPutString

    mov     p1l, ObjectIDl
    mov     p1h, ObjectIDh
    rcall   vDebugPutHexWord      // Print PID
    rcall   vDebugPutSpace

    mov     Zl, ObjectIDl
    mov     Zh, ObjectIDh
    ldd     p1l, Z+PidState
    rcall   vDebugPutHexByte
    rcall   vDebugPutSpace
    mov     Zl, ObjectIDl
    mov     Zh, ObjectIDh
    ldd     p1l, Z+PidPriority
    rcall   vDebugPutHexByte
    rcall   vDebugPutSpace
    rcall   vDebugPutSpace

    mov     Zl, ObjectIDl
    mov     Zh, ObjectIDh
    ldd     Yl, Z+PidSP+NextL   // Z -> Frame
    ldd     Yh, Z+PidSP+NextH
    mov     p1l, Yl
    mov     p1h, Yh
    rcall   vDebugPutHexWord      // Print Frame Pointer
    rcall   vDebugPutSpace
    rcall   vDebugPutSpace

    adiw    Yl, 1           // Remember Stack are post dec!

    ld      p1l, Y+         // SREG
    rcall   vDebugPutHexByte
    //
    // Print R0-R15
    //
.LPCLOOP:
    rcall   .LPCPRINT16REGPRE
    ldi     p1h, hi8(.LPCCTDT1)
    ldi     p1l, lo8(.LPCCTDT1)
    rcall   .LPCPRINT16REG
    rcall   .LPCPRINT16REGPRE
    ldi     p1h, hi8(.LPCCTDT2)
    ldi     p1l, lo8(.LPCCTDT2)
    rcall   .LPCPRINT16REG
    ldi     p1h, hi8(.LPCCTDT3)
    ldi     p1l, lo8(.LPCCTDT3)
    rcall   vDebugPutString
    ld      p1h, Y+
    ld      p1l, Y
    rcall   vDebugPutHexWord
    ret

.LPCPRINT16REGPRE:
    rcall   vDebugPutCR
    mov     p1l, Yl
    mov     p1h, Yh
    rcall   vDebugPutHexWord
    rjmp    vDebugPutSpace

.LPCPRINT16REG:
    rcall   vDebugPutString
    ldi     Zl, 16
    mov     r1, Zl
    
.LPCP16R0:
    ld      p1l, Y+
    rcall   vDebugPutHexByte
    rcall   vDebugPutSpace
    dec     r1
    brne    .LPCP16R0
    ret

.LPCCTDT0:  
  .asciz  "\rPID Flg Pri Frame SR\r"
.LPCCTDT1:  
  .asciz  "R0  "
.LPCCTDT2:  
  .asciz  "R16 "
.LPCCTDT3:  
  .asciz  "\r     PC  "
  .balign 2

/* ========================================================================== */
  .end
