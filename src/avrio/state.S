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
 * @file state.S
 * @brief Machine d'états
 *
 * Revision History ---
 *    20111002 - Initial version by epsilonRT
 */
#define ucTmp 0
#define ucState 24
#define ucValue 22
#define ucFrom 22
#define ucTo 20
#define usTmp 18
#define pxState  24
#define SREG 0x3F

// -----------------------------------------------------------------------------
/* uint8_t xStateGet (xState * pxState); */
.section .text,"ax",@progbits
.global xStateGet
.type xStateGet,@function

xStateGet:
    in      ucTmp, SREG
    cli
    movw    r30, pxState
    ld      ucState, Z
    rjmp    .LSuccess
.size xStateGet, . - xStateGet + 1

// -----------------------------------------------------------------------------
/* void vStateSet(xState * pxState, uint8_t ucValue); */
.section .text,"ax",@progbits
.global vStateSet
.type vStateSet,@function

vStateSet:
    in      ucTmp, SREG
    cli
    movw    r30, pxState
    st      Z, ucValue
    rjmp    .LSuccess
.size vStateSet, . - vStateSet + 1

// -----------------------------------------------------------------------------
/* int8_t xStateTryswitch(xState * pxState, uint8_t ucFrom, uint8_t ucTo); */
.section .text,"ax",@progbits
.global xStateTryswitch
.type xStateTryswitch,@function

xStateTryswitch:
    in      ucTmp, SREG
    cli
    movw    r30, pxState

    ld      ucState, Z
    sub     ucState, ucFrom
    brne    .LFail
    st      Z, ucTo
    rjmp .LSuccess

.LFail:
    ser     ucState

.LSuccess:
    out     SREG, ucTmp
    ret
.size xStateTryswitch, . - xStateTryswitch + 1


// -----------------------------------------------------------------------------
/* int8_t xStateTrywait(state_t * ucState, uint8_t ucValue); */
.section .text,"ax",@progbits
.global xStateTrywait
.type xStateTrywait,@function

xStateTrywait:
    in      ucTmp, SREG
    cli
    movw    r30, pxState

    ld      ucState, Z

    sub     ucState, ucValue
    brne    .LFail

    rjmp .LSuccess
.size xStateTrywait, . - xStateTrywait + 1

// -----------------------------------------------------------------------------
/* void vStateSwitch (xState * pxState, uint8_t ucFrom, uint8_t ucTo); */
.section .text,"ax",@progbits
.global vStateSwitch
.type vStateSwitch,@function

vStateSwitch:
    movw usTmp, pxState
.LSsLoop:
    movw pxState, usTmp
    rcall xStateTryswitch
    tst ucState
    brne .LSsLoop
    ret
.size vStateSwitch, . - vStateSwitch + 1

// -----------------------------------------------------------------------------
/* void vStateWait(xState * pxState, uint8_t ucValue); */
.section .text,"ax",@progbits
.global vStateWait
.type vStateWait,@function

vStateWait:
    movw usTmp, pxState
.LSwLoop:
    movw pxState, usTmp
    rcall xStateTrywait
    tst ucState
    brne .LSwLoop
    ret
.size vStateWait, . - vStateWait + 1

/* ========================================================================== */
