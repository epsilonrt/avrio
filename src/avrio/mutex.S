/**
 * @file mutex.S
 * @brief Mutex
 * @author Copyright _ 2011-2012 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * @version $Id$
 */
#define ucSREG 0x3F
#define ucTmp    0
#define ucNull   1
#define pxMutex  24
#define ucValue  24

.section .text,"ax",@progbits

.global vMutexUnlock
.global xMutexTryLock
.global vMutexLock

.type vMutexUnlock,@function
.type xMutexTryLock,@function
.type vMutexLock,@function

//------------------------------------------------------------------------------
// void vMutexUnlock (xMutex * pxMutex)
vMutexUnlock:
    movw    r30, pxMutex

    in      ucTmp, ucSREG
    ser     ucValue
    cli
//  ---- Atomic --->
    st      Z, ucValue
//  <--- Atomic ----
    out     ucSREG, ucTmp
    ret
.size vMutexUnlock, . - vMutexUnlock + 1

//------------------------------------------------------------------------------
// int8_t xMutexTryLock(xMutex * pxMutex)
xMutexTryLock:
    movw    r30, pxMutex

    in      ucTmp, ucSREG
    cli
//  ---- Atomic --->
    ld      ucValue, Z
    tst     ucValue
    breq    0f  ; mutex is locked
    st      Z, ucNull
    ser     ucValue
//  <--- Atomic ----
0:  com     ucValue 
    out     ucSREG, ucTmp
    ret

.size xMutexTryLock, . - xMutexTryLock + 1

//------------------------------------------------------------------------------
// void vMutexLock (xMutex * pxMutex)
vMutexLock:
    movw    r30, pxMutex

1:  in      ucTmp, ucSREG
    cli
//  ---- Atomic --->
    ld      ucValue, Z
    tst     ucValue
    brne    2f
//  <--- Atomic ----
    out     ucSREG, ucTmp
    breq    1b

2:  st      Z, ucNull
    out     ucSREG, ucTmp
    ret

.size vMutexLock, . - vMutexLock + 1

/* ========================================================================== */
