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
 * @file mutex_bit.S
 * @brief Mutex bit
 */
#include <stdbool.h>

#define ucSREG 0x3F
#define ucTmp    0
#define ucNull   1

// Arguments
#define pxMutex  24
#define ucMask   22

// Return values
#define ucValue  24

.section .text,"ax",@progbits

.global vMutexUnlockBit
.global vMutexLockBit
.global vMutexUntilBitUnlocked
.global xMutexBitLocked
.global xMutexTryLockBit

.type vMutexUnlockBit,@function
.type vMutexLockBit,@function
.type vMutexUntilBitUnlocked,@function
.type xMutexBitLocked,@function
.type xMutexTryLockBit,@function

//------------------------------------------------------------------------------
// void vMutexUnlockBit (xMutex * pxMutex, uint8_t ucMask)
vMutexUnlockBit:
    movw    r30, pxMutex

    in      ucTmp, ucSREG
    cli
//  ---- Atomic --->
    ld      ucValue, Z
    or      ucValue, ucMask
    st      Z, ucValue
//  <--- Atomic ----
    out     ucSREG, ucTmp
    ret
.size vMutexUnlockBit, . - vMutexUnlockBit + 1

//------------------------------------------------------------------------------
// void vMutexLockBit (xMutex * pxMutex, uint8_t ucMask)
vMutexLockBit:
    movw    r30, pxMutex
    in      ucTmp, ucSREG

L_LoopIfLocked:  
    cli
//  ---- Atomic --->
    ld      ucValue, Z
    and     ucValue, ucMask
    cp      ucValue, ucMask
    breq    L_LockAndExit // lock and exit
//  <--- Atomic ----
    out     ucSREG, ucTmp
    rjmp    L_LoopIfLocked

.size vMutexLockBit, . - vMutexLockBit + 1

//------------------------------------------------------------------------------
// void vMutexUntilBitUnlocked (xMutex * pxMutex, uint8_t ucMask)
vMutexUntilBitUnlocked:
    movw    r30, pxMutex
    in      ucTmp, ucSREG

L_LoopUntilLocked:  
    cli
//  ---- Atomic --->
    ld      ucValue, Z
    and     ucValue, ucMask
    cp      ucValue, ucMask
    breq    L_Exit
//  <--- Atomic ----
    out     ucSREG, ucTmp
    rjmp    L_LoopUntilLocked

.size vMutexUntilBitUnlocked, . - vMutexUntilBitUnlocked + 1

//------------------------------------------------------------------------------
// int8_t xMutexBitLocked(xMutex * pxMutex, uint8_t ucMask)
xMutexBitLocked:
    movw    r30, pxMutex
    in      ucTmp, ucSREG
    
    cli
//  ---- Atomic --->
    ld      ucValue, Z
    and     ucValue, ucMask
    cp      ucValue, ucMask
    brne    L_ReturnTrue
    rjmp    L_ReturnFalse

.size xMutexBitLocked, . - xMutexBitLocked + 1

//------------------------------------------------------------------------------
// int8_t xMutexTryLockBit(xMutex * pxMutex, uint8_t ucMask)
xMutexTryLockBit:
    movw    r30, pxMutex

    in      ucTmp, ucSREG
    cli
//  ---- Atomic --->
    ld      ucValue, Z
    and     ucValue, ucMask
    cp      ucValue, ucMask
    brne    L_ReturnTrue

.size xMutexTryLockBit, . - xMutexTryLockBit + 1

//  ---- Atomic --->
L_LockAndExit:
    ld      ucValue, Z
    com     ucMask
    and     ucValue, ucMask
    st      Z, ucValue
//  <--- Atomic ----

L_ReturnFalse:
    clr     ucValue
L_Exit:
    out     ucSREG, ucTmp
    ret
L_ReturnTrue:
    ldi     ucValue, true
    rjmp    L_Exit

/* ========================================================================== */
