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
 * @file halt.S
 * @brief vAvrXHalt
 * @author Larry Barello, <larry@barello.net>. 
 *         Copyright _ 1998-2002 All rights reserved.
 *
 * Revision History
 ---
 *    20110830 - Initial port to AvrIO by epsilonRT
 */
#include "avrx.inc"

  .section .text.avrx_halt,"ax",@progbits
/* ========================================================================== */

  .text
/* -----------------------------------------------------------------------------
 * vAvrXHalt
 *
 * Locks up the system.
 *
 * PASSED:       
 * RETURNS:      
 * USES: 
 * CALLS:
 * ASSUMES:
 * NOTES:        
 */
  .global vAvrXHalt
  .func vAvrXHalt
// -----------------------------------------------------------------------------
vAvrXHalt:
    BeginCritical
    rjmp    vAvrXHalt

  .endfunc

/* ========================================================================== */
  .end

