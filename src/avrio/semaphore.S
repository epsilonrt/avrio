/**
 * @file semaphore.S
 * @brief Sémaphore
 * @author Copyright © 2011-2012 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * @version $Id$
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
