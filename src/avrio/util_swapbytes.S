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

