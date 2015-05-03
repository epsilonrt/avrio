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
 * @file priority.S
 * @brief Gestion des priorités
 * @author Larry Barello, <larry@barello.net>. 
 *         Copyright © 1998-2002 All rights reserved.
 *
 * Revision History
 ---
 *    20110830 - Initial port to AvrIO by epsilonRT
 *    20020604 - Modified to use xKernelData Structure
 */
#include "avrx.inc"

  .section .text.avrx_priority,"ax",@progbits
/* ========================================================================== */

  .text
/* -----------------------------------------------------------------------------
 * uint8_t xAvrXChangePriority(xPid, uint8_t)
 * uint8_t xAvrXPriority(void)
 * xPid pxAvrXSelf(void)
 *
 * PASSED:       
 * RETURNS:      
 * USES: 
 * CALLS:
 * ASSUMES:	Top of RunQueue = current running task
 * NOTES:	Should use "Running" rather than RunQueue.
 */
  .global xAvrXChangePriority
  .func xAvrXChangePriority
// -----------------------------------------------------------------------------
xAvrXChangePriority:
    mov     Zl, p1l
    mov     Zh, p1h
    ldd     r1l, Z+PidPriority
    std     Z+PidPriority, p2l
    ret

  .endfunc

/* -----------------------------------------------------------------------------
*/
  .global pxAvrXSelf
  .func pxAvrXSelf
// -----------------------------------------------------------------------------
pxAvrXSelf:
    lds     r1h, xKernelData+RunQueue+NextH
    lds     r1l, xKernelData+RunQueue+NextL
    ret

  .endfunc

/* -----------------------------------------------------------------------------
*/
  .global xAvrXPriority
  .func xAvrXPriority
// -----------------------------------------------------------------------------
xAvrXPriority:
    lds     Zh, xKernelData+RunQueue+NextH
    lds     Zl, xKernelData+RunQueue+NextL
    ldd     r1l, Z+PidPriority
    ret

  .endfunc

/* ========================================================================== */
  .end
