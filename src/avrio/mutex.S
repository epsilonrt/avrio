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
    breq    1b

2:  st      Z, ucNull
    out     ucSREG, ucTmp
    ret

.size vMutexLock, . - vMutexLock + 1

/* ========================================================================== */
