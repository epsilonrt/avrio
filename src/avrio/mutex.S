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
 * @file mutex.S
 * @brief Mutex
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
    nop
    rjmp    1b

2:  st      Z, ucNull
    out     ucSREG, ucTmp
    ret

.size vMutexLock, . - vMutexLock + 1

/* ========================================================================== */
