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
 * @file monitor.S
 * @brief Monitor module
 * @author Larry Barello, <larry@barello.net>. 
 *         Copyright _ 1998-2002 All rights reserved.
 *
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
#define NL "\n"
#define CR "\r"

/* external functions ======================================================= */
  .extern ucAvrXReadEEProm
  .extern usAvrXReadEEPromWord
  .extern vAvrXWriteEEProm

MODULE(avrx.monitorio)
/* internal public functions ================================================ */

/* -----------------------------------------------------------------------------
 * void vMonitorPrintHelp()
 */
  .global vMonitorPrintHelp
vMonitorPrintHelp:
    ldi     p1l, lo8(.LPHHelloStr)
    ldi     p1h, hi8(.LPHHelloStr)

.LPHLOOP:
    rcall   vDebugPutString
    subi    r1l, lo8(-1)
    sbci    r1h, hi8(-1)            // Keep looping until
    mov     Zl, r1l
    mov     Zh, r1h
    lpm                             // until all strings out.
    tst     R0
    brne    .LPHLOOP
    ret

.LPHHelloStr:  
  .asciz  CR
  .asciz  "AvrX Monitor V" MONITOR_VERSION, CR
  .asciz  CR
  .asciz  "Modify = m{s,e}[w][aaaa]", CR
  .asciz  "Dump   = d{s,e}[w][aaaa] [xx]", CR
  .asciz  "List   = l", CR
  .asciz  "Halt   = h[xxxx]", CR
  .asciz  "Step   = s[xxxx]", CR
#ifdef  AVRX_SINGLESTEP_ENABLE
  .asciz  "Next   = n[xxxx]", CR
#endif
  .asciz  "Resume = r[xxxx]", CR
  .asciz  "Context= x[xxxx]", CR
  .asciz  "Signal = i[xxxx]", CR
  .asciz  "Cancel = c[xxxx]", CR
  .asciz  "Kill   = k[xxxx]", CR
  .asciz  "IniTask= p[aaaa]", CR
  .asciz  "Quit   = q", CR
  .asciz  "Help   = ?", CR
  .asciz  "\000"
  .balign 2

/* -----------------------------------------------------------------------------
 * void * pvMonitorGetAddress()
 */
  .global pvMonitorGetAddress
pvMonitorGetAddress:
    rcall   usDebugGetHexWord
    brts    .LGAEXIT            // No Error, 4 chars entered
    rcall   xDebugIsWhite       // Last char is white ?
    brtc    .LGAEXIT            //  No, Error T = 0
    tst     p2l                 //  Yes, hex chars entered ?
    breq    .LGAEXIT            //    No, Error T = 0 
    set                         //    Yes, No Error, T = 1
.LGAEXIT:
    ret

/* -----------------------------------------------------------------------------
 * xPid pxMonitorGetObjectID()
 *
 * Little helper routine to get and return a byte value with
 * carry set if all Ok.  e.g. entered 2 hex digits, or terminated
 * with whitespace.
 *
 * Puts the ID in the R25:R24 register because that is what all the
 * AvrX routines expect.
 */
  .global pxMonitorGetObjectID
pxMonitorGetObjectID:
    rcall   usDebugGetHexWord
    brts    .LGOI00
    rcall   xDebugIsWhite
    brtc    .LGOI02
.LGOI00:
    tst     p2l
    brne    .LGOI01
    mov     p1l, ObjectIDl  // Restore default
    mov     p1h, ObjectIDh
.LGOI01:
    mov     ObjectIDl, p1l
    mov     ObjectIDh, p1h
.LGOI02:
    ret

/* -----------------------------------------------------------------------------
 * uint8_t ucMonitorGetDumpLength()
 *
 * Another helper to manage defaults with the dump length
 * counter
 */
  .global ucMonitorGetDumpLength
ucMonitorGetDumpLength:
    rcall   ucDebugGetHexByte     // get a dump counter
    brts    .LGDL00                 // If term invalid,
    rcall   xDebugIsWhite         // check for whitespace
    brtc    .LGDL02
.LGDL00:
    tst     p2l                   // If no user entry
    brne    .LGDL01
    mov     p1l, DumpLength       // load up default
.LGDL01:
    mov     DumpLength, p1l
.LGDL02:
    ret

/* -----------------------------------------------------------------------------
 * void * pxMonitorGetFlagsAndAddress()
 *
 * Parse input stream for byte/word flag and address.  All are optional
 * as long as the input stream is terminated with whitespace.
 * 
 *  PASSED:  MonFlags = DUMPFLAG = 1 if Dump needed
 *  RETURNS: p1  = Address (if EEFLAG = 0), EEAddress (if EEFLAG = 1)
 *           p2l = Dump length (if DUMPFLAG = 1)
 *           MonFlags = WORDFLAG, EEFLAG
 *           T = 0 if error
 *  USES:    r1l, tmp0
 *  STACK:   -
  */
  .global pxMonitorGetFlagsAndAddress
pxMonitorGetFlagsAndAddress:
    clt
    rcall   cDebugGetCharEcho // receive char in r1l
    mov     tmp0, MonFlags
    cpi     r1l, 's'
    brne    .LGFATRYEERAM
    cbr     tmp0, _BV(EEFLAG) // If "s" , clear flag
    rjmp    .LGFACONTINUE
    
.LGFATRYEERAM:
    cpi     r1l, 'e'        // if "e", set flag
    brne    .LGFADONE         // 's' or 'e' must be specified, T = 0 => Error
    sbr     tmp0, _BV(EEFLAG)

.LGFACONTINUE:
    cbr     tmp0, _BV(WORDFLAG)
    mov     MonFlags, tmp0        // Save EEFLAG
    rcall   usDebugGetHexWord 
    brtc    .LGFACHECKWORDFLAG    // no hex chars entered 
    rjmp    .LGFAUPDATEADDRESS    // Go for the address
    
.LGFACHECKWORDFLAG:
    mov     tmp0, MonFlags
    cpi     p2h, 'w'
    brne    .LGFACHECKFORWHITESPACE  // Only "w" allowed if not address
    sbr     tmp0, _BV(WORDFLAG)
    mov     MonFlags, tmp0
    
    rcall   usDebugGetHexWord     // Try again for address
    brts    .LGFAUPDATEADDRESS
    
.LGFACHECKFORWHITESPACE:
    rcall   xDebugIsWhite
    brtc    .LGFADONE

.LGFAUPDATEADDRESS:
    tst     p2l             // see if any user input
    breq    .LGFADONE
    sbrs    MonFlags, EEFLAG
    rjmp    .LGFAADDRESS
    mov     EEAddressl, p1l
    mov     EEAddressh, p1h
    rjmp    .LGFAGETLENGTH
    
.LGFAADDRESS:
    mov     Addressl, p1l
    mov     Addressh, p1h

.LGFAGETLENGTH:
    mov     tmp0, MonFlags
    sbrs    tmp0, DUMPFLAG
    rjmp    .LGFADONE
    
    rcall   xDebugIsWhite
    brtc    .LGFASPACE      // Last char is not white, ok, wait space or CR
    brne    .LGFALEN        // Last char is space, get length
    rjmp    .LGFADONE       // Last char is CR, terminated

.LGFASPACE:    
    rcall   cDebugGetCharEcho   // wait CR or Space
    mov     p2h, r1l
    rcall   xDebugIsWhite
    brtc    .LGFADONE           // char is not white, error
    breq    .LGFADONE           // char is CR, terminated 

.LGFALEN:   
    rcall   ucMonitorGetDumpLength       // Leave count in Xl
 
.LGFADONE:
    mov     p2l, DumpLength
    mov     p1l, Addressl   // Restore previous...
    mov     p1h, Addressh
    sbrc    MonFlags, EEFLAG
    mov     p1l, EEAddressl
    sbrc    MonFlags, EEFLAG
    mov     p1h, EEAddressh
    ret
  
/* -----------------------------------------------------------------------------
 * unsigned usMonitorGetByteWord(void *)
 *
 * PASSED:       p1 = address
 * RETURNS:      r1 = data
 * USES:         MonFlags
 */
  .global usMonitorGetByteWord
usMonitorGetByteWord:
    sbrc    MonFlags, EEFLAG
    rjmp    .LGBWEEPROM

    mov     Zl, TempWl
    mov     Zh, TempWh

    ldd     r1l, Z+0
    sbrs    MonFlags, WORDFLAG
    ret

    ldd     r1h, Z+NextH
    ldd     r1l, Z+NextL
    ret
.LGBWEEPROM:                      // With new calls must preserve Z
    mov     p1h, TempWh
    mov     p1l, TempWl
    sbrs    MonFlags, WORDFLAG
    rcall   ucAvrXReadEEProm
    sbrc    MonFlags, WORDFLAG
    rcall   usAvrXReadEEPromWord
    
.LGBWEXIT:
    ret

/* -----------------------------------------------------------------------------
 * void vMonitorPutByteWord(unsigned, void *)
 *
 * Passed:       p1 - value
 *               TempW - pointer
 * Returned      void
 * Uses          MonFlags (eeprom vs RAM)
 */
  .global vMonitorPutByteWord
vMonitorPutByteWord:
    sbrc    MonFlags, EEFLAG
    rjmp    .LPBWEEPROM

    mov     Zl, TempWl
    mov     Zh, TempWh
    std     Z+0, p1l
    sbrs    MonFlags, WORDFLAG
    ret

    std     Z+NextH, p1h
    std     Z+NextL, p1l
    ret
    
.LPBWEEPROM:
    mov     TempB, p1h
    mov     p2l, p1l
    mov     p1h, TempWh
    mov     p1l, TempWl
    rcall   vAvrXWriteEEProm
    sbrs    MonFlags, WORDFLAG
    rjmp    .LPBWEXIT

    mov     p1l, TempWl
    mov     p1h, TempWh
    subi    p1l, lo8(-1)
    sbci    p1h, hi8(-1)
    mov     p2l, TempB
    rcall   vAvrXWriteEEProm
.LPBWEXIT:
    ret
/* ========================================================================== */
  .end
