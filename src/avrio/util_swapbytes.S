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
 * @file util_swapbytes.S
 * @brief Implémentation utilitaires (assembleur)
 *
 * Revision History ---
 *    20111113 - Initial version by epsilonRT
 */

/* registers ================================================================ */
#define ucTmp    0
#define ucNull   1

// Arguments
#define pucBytes  24
#define ucSize    22

#define ucHead    24
#define ucTail    25

/* internal public functions ================================================ */


/* void vSwapBytes (uint8_t * pucBytes, uint8_t ucSize); */
// -----------------------------------------------------------------------------
.section .text,"ax",@progbits
.global vSwapBytes
.type vSwapBytes,@function

vSwapBytes:
    movw    r30, pucBytes
    movw    r26, r30
    add     r26, ucSize
    adc     r27, ucNull
    sbiw    r26, 1
    lsr     ucSize

L_SBLoop:

    ld      ucHead, Z
    ld      ucTail, X
    st      Z+, ucTail
    st      X, ucHead
    sbiw    r26, 1
    dec     ucSize
    brne    L_SBLoop
    ret

.size vSwapBytes, . - vSwapBytes + 1

/* ========================================================================== */

