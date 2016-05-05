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
 * @file semaphore.S
 * @brief Sémaphore
 */

#define ucSREG 0x3F
#define ucTmp 0
#define pxSemaphore   24
#define ucValueLsb    24
#define ucValueMsb    25

.section .text,"ax",@progbits

.global vSemInit
.global vSemPost
.global xSemTryWait
.global vSemWait

.type vSemInit,@function
.type vSemPost,@function
.type xSemTryWait,@function
.type vSemWait,@function

//------------------------------------------------------------------------------
// void vSemInit (xSem * pxSemaphore, xSemValue xSetValue)
#define ucSetValueLsb 22
#define ucSetValueMsb 23

vSemInit:
    movw    r30, pxSemaphore

    in      ucTmp, ucSREG
    cli
    std     Z+1, ucSetValueMsb
    st      Z, ucSetValueLsb
    out     ucSREG, ucTmp
    ret

.size vSemInit, . - vSemInit + 1

//------------------------------------------------------------------------------
// void vSemPost (xSem * pxSemaphore)

vSemPost:
    movw    r30, pxSemaphore

    in      ucTmp, ucSREG
    cli
    ld      ucValueLsb, Z
    ldd     ucValueMsb, Z+1
    adiw    ucValueLsb, 1
    std     Z+1, ucValueMsb
    st      Z, ucValueLsb
    out     ucSREG, ucTmp
    ret

.size vSemPost, . - vSemPost + 1


//------------------------------------------------------------------------------
// int8_t xSemTryWait(xSem * pxSemaphore)

xSemTryWait:
    movw    r30, pxSemaphore

    in      ucTmp, ucSREG
    cli

    ld      ucValueLsb, Z
    ldd     ucValueMsb, Z+1
    sbiw    ucValueLsb, 0
    breq    0f

    sbiw    ucValueLsb, 1
    std     Z+1, ucValueMsb
    st      Z, ucValueLsb

    clr     ucValueLsb
    rjmp    1f

0:  ser     ucValueLsb

1:  out     ucSREG, ucTmp
    ret
.size xSemTryWait, . - xSemTryWait + 1

//------------------------------------------------------------------------------
// void vSemWait (xSem * pxSemaphore)
#define pxSemaphoreTmp  22

vSemWait:
    movw    pxSemaphoreTmp, pxSemaphore

2:  movw    pxSemaphore, pxSemaphoreTmp
    rcall   xSemTryWait
    tst     ucValueLsb
    brne    2b
    ret

.size vSemWait, . - vSemWait + 1

/* ========================================================================== */
