/**
 * @file util_swapbytes.S
 * @brief Implémentation utilitaires (assembleur)
 * @author Copyright © 2011-2012 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * @version $Id$
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

