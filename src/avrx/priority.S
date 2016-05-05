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
