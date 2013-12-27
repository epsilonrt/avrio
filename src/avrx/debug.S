/**
 * @file debug.S
 * @author Larry Barello, <larry@barello.net>. 
 *         Copyright _ 1998-2002 All rights reserved.
 * @author Copyright _ 2011-2012 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * @version $Id$
 * Revision History ---
 *    20110905 - Initial version by epsilonRT
 * ----
 * @brief Fonctions de debugage
 *
 * This code is might be C compatible. I have not tested it or gone over it
 * carefully to insure compatibility.   In general the Monitor and the
 * serial routines should run in their own task space and not be called by
 * any C code.
 */
#include "avrx.inc"

  .section .text.avrx_debug,"ax",@progbits
/* ========================================================================== */
  .extern vDebugInit
  .extern vDebugPutChar
  .extern cDebugGetChar

/* internal public functions ================================================ */
  .text

// -----------------------------------------------------------------------------
// cDebugGetCharEcho
// Special version of GetChar that echos
//
// PASSED:  -
// RETURNS: r1l = rx char
// USES:    -
// STACK:   -
//
// History ---
//---
  .global cDebugGetCharEcho
  .func cDebugGetCharEcho
// -----------------------------------------------------------------------------
cDebugGetCharEcho:
    rcall   cDebugGetChar
    mov     p2h, r1l
    rcall   xDebugIsWhite
    brne    .LGCEPUT        // print if not CR
    ret
.LGCEPUT:   
    rjmp    vDebugPutChar
  .endfunc

// -----------------------------------------------------------------------------
//
// vDebugPutCR
// Put a CR to the output stream
//
// PASSED:  -
// RETURNS: -
// USES:    p1l
// STACK:   -
//
// History ---
//---
  .global vDebugPutCR
  .func vDebugPutCR
// -----------------------------------------------------------------------------
vDebugPutCR:
    ldi     p1l, 0x0D
    rjmp    vDebugPutChar
  .endfunc

// -----------------------------------------------------------------------------
//
// vDebugPutSpace
// Put a space to the output stream
//
// PASSED:  -
// RETURNS: -
// USES:    p1l
// STACK:   -
//
// History ---
//---
  .global vDebugPutSpace
  .func vDebugPutSpace
// -----------------------------------------------------------------------------
vDebugPutSpace:
    ldi     p1l, ' '
    rjmp    vDebugPutChar
  .endfunc

// -----------------------------------------------------------------------------
//
// const char * vDebugPutString (const char *)
// Put FLASH string, return pointer to next char after NULL
//
// PASSED:  p1 = near pointer to string in FLASH memory
// RETURNS: r1 = pointer to next char after NULL
// USES:    Z, r0
// STACK:   2
//
// History ---
//---
  .global vDebugPutString
  .func vDebugPutString
// -----------------------------------------------------------------------------
vDebugPutString:
    mov     Zl, p1l
    mov     Zh, p1h
_foo:
    lpm
    tst     r0
    breq    _ps00
    mov     p1l, r0
    push    Zl
    push    Zh
    rcall   vDebugPutChar
    pop     Zh
    pop     Zl
    adiw    Zl, 1
    rjmp    _foo
_ps00:
    mov     r1l, Zl
    mov     r1h, Zh
    ret
  .endfunc

// -----------------------------------------------------------------------------
// xDebugGetHex
//
// PASSED:  p1l = Maximum number of digits
// RETURNS: r1 = val, p2l = Number of digits entered, p2h = last char entered
//          T = 0 if error
// USES:    r24, r25
// STACK:   4
//
// History ---
//    20110907 - pj - p2h = last char entered
//---
  .global xDebugGetHex
  .func xDebugGetHex
// -----------------------------------------------------------------------------
xDebugGetHex:
    push    r10
    push    r11
    push    r12
    push    r13
    
    mov     r13, p1l
    clr     r10
    clr     r11
    clr     r12

gh_00:
    rcall   cDebugGetCharEcho
    mov     p2h, r1l    // backup last char in p2h
    rcall   xDebugIsHex // Set T if Hex
    brtc    gh_xx
    // [0-9] or [A-F] ?
    cpi     r1l, '9' + 1
    brge    gh_01
    
    // convert [0-9] char from ascii to binary
    subi    r1l, '0'
    rjmp    gh_02

gh_01:
    cbr     r1l, 0x20 // convert to upper
    // convert [A-F] char from ascii to binary
    subi    r1l, 'A'-10

gh_02:
    // shift r10 high nibble in in r11 low nibble
    lsl     r10
    rol     r11
    lsl     r10
    rol     r11
    lsl     r10
    rol     r11
    lsl     r10
    rol     r11
    
    // Add current nibble with result
    add     r10, r1l
    inc     r12
    dec     r13
    brne    gh_00

gh_xx:
    mov     r1l, r10
    mov     r1h, r11
    mov     p2l, r12

gh_ret:
    pop     r13
    pop     r12
    pop     r11
    pop     r10
    ret
  .endfunc

// -----------------------------------------------------------------------------
// usDebugGetHexWord
//
// PASSED:  -
// RETURNS: r1 = val, p2l = Number of digits entered, p2h = last char entered
//          T = 0 if error
// USES:    r24, r25
// STACK:   4
//
// History ---
//---
  .global usDebugGetHexWord
  .func usDebugGetHexWord
// -----------------------------------------------------------------------------
usDebugGetHexWord:
    ldi     p1l, 4
    rjmp    xDebugGetHex
  .endfunc

// -----------------------------------------------------------------------------
// ucDebugGetHexByte
//
// PASSED:  -
// RETURNS: r1 = val, p2l = Number of digits entered, p2h = last char entered
//          T = 0 if error
// USES:    r24, r25
// STACK:   4
//
// History ---
//---
  .global ucDebugGetHexByte
  .func ucDebugGetHexByte
// -----------------------------------------------------------------------------
ucDebugGetHexByte:
    ldi     p1l, 2
    rjmp    xDebugGetHex
  .endfunc

// -----------------------------------------------------------------------------
// ucDebugGetHexDigit
//
// PASSED:  -
// RETURNS: r1 = val, p2l = Number of digits entered, p2h = last char entered
//          T = 0 if error
// USES:    r24, r25
// STACK:   4
//
// History ---
//---
  .global ucDebugGetHexDigit
  .func ucDebugGetHexDigit
// -----------------------------------------------------------------------------
ucDebugGetHexDigit:
    ldi     p1l, 1
    rjmp    xDebugGetHex
  .endfunc

// -----------------------------------------------------------------------------
// ucDebugGetError
// For convenience in C
// Must be called immediately after calling the function that can cause the error
// PASSED:  T
// RETURNS: r1l = 0 no error
// USES:    -
// STACK:   -
//
// History ---
//---
  .global ucDebugGetError
  .func ucDebugGetError
// -----------------------------------------------------------------------------
ucDebugGetError:
    brtc    .LGEERROR
    clr     r1l
    ret
.LGEERROR:
    ser     r1l
    ret
  .endfunc

// -----------------------------------------------------------------------------
// void vDebugPutHex(unsigned val, char digits)
//
// PASSED:       p1 = Value, p2l = # digits
// RETURN:       Nothing
// USES:         r25, TxByte
// STACK:        2
//
// History ---
//---
  .global vDebugPutHex
  .func vDebugPutHex
// -----------------------------------------------------------------------------
vDebugPutHex:
    ldi     tmp2, 4          // Maximum number of digits.
    mov     tmp3, p2l
    mov     p2l, p1l
    mov     p2h, p1h
    
ph00:
    lsl     p2l
    rol     p2h
    rol     p1l
    lsl     p2l
    rol     p2h
    rol     p1l
    lsl     p2l
    rol     p2h
    rol     p1l
    lsl     p2l
    rol     p2h
    rol     p1l

    cp      tmp3, tmp2
    brlt    ph02

    cbr     p1l, 0xF0
    subi    p1l, lo8(-'0')          // Addi 0
    cpi     p1l, lo8('9' + 1)       // if > 9
    brlt    ph01                    // subi 9 and add A
    subi    p1l, lo8('9' + 1 - 'A')
ph01:
    rcall   vDebugPutChar
ph02:
    dec     tmp2
    brne    ph00

    ret
  .endfunc

// -----------------------------------------------------------------------------
// Put out a hex word
//
// PASSED:  p1 = Value
// RETURNS: -
// USES:    -
// STACK:   -
//
// History ---
//---
  .global vDebugPutHexWord
  .func vDebugPutHexWord
// -----------------------------------------------------------------------------
vDebugPutHexWord:
    ldi     p2l, 4
    rjmp    vDebugPutHex
  .endfunc

// -----------------------------------------------------------------------------
// Put out a two ascii hex byte
//
// PASSED:  p1 = Value
// RETURNS: -
// USES:    -
// STACK:   -
//
// History ---
//---
  .global vDebugPutHexByte
  .func vDebugPutHexByte
// -----------------------------------------------------------------------------
vDebugPutHexByte:
    ldi     p2l, 2
    rjmp    vDebugPutHex
  .endfunc

// -----------------------------------------------------------------------------
// vDebugPutHexDigit
// Put out a hex digit
//
// PASSED:  p1 = Value
// RETURNS: -
// USES:    -
// STACK:   -
//
// History ---
//---
  .global vDebugPutHexDigit
  .func vDebugPutHexDigit
// -----------------------------------------------------------------------------
vDebugPutHexDigit:
    ldi     p2l, 1
    rjmp    vDebugPutHex
  .endfunc

// -----------------------------------------------------------------------------
// xDebugIsHex
//
// Tests p1l for ascii hex char [0-9, A-F, a-f]
// Returns: T bit
//
// History ---
//---
  .global xDebugIsHex
  .func xDebugIsHex
// -----------------------------------------------------------------------------
xDebugIsHex:
    clt
    cpi     p1l, '0'
    brlt    .LIHNO
    cpi     p1l, '9' + 1
    brlt    .LIHYES
    cpi     p1l, 'A'
    brlt    .LIHNO
    cpi     p1l, 'F' + 1
    brlt    .LIHYES
    cpi     p1l, 'a'
    brlt    .LIHNO
    cpi     p1l, 'f' + 1
    brge    .LIHNO
.LIHYES:
    set
.LIHNO:
    ret
  .endfunc

// -----------------------------------------------------------------------------
// xDebugIsWhite
//
// Tests p2h for space or CR
// Returns: T = 1 if true
//          Z = 1 if CR
//
// History ---
//  20110907 - pj - Z = 1 if CR, passed p2h
//---
  .global xDebugIsWhite
  .func xDebugIsWhite
// -----------------------------------------------------------------------------
xDebugIsWhite:
    clt
    cpi     p2h, 0x0D
    breq    .LIHYES     // CR        -> T = 1, Z = 1
    cpi     p2h, ' '
    brne    .LIHNO      // Not White -> T = 0
    clz
    rjmp    .LIHYES     // SPACE     -> T = 1, Z = 0
  .endfunc

/* ========================================================================== */
  .end

