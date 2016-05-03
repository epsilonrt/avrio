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
 * @file re_schedule.S
 * @brief Description
 * @author Larry Barello, <larry@barello.net>. 
 *         Copyright _ 1998-2002 All rights reserved.
 *
 * Revision History ---
 *    20110830 - Initial port to AvrIO by epsilonRT
 *    20050913 - removed local lable - messing up IAR
 *	  20020604 - Modified to use xKernelData structure
 */
#include "avrx.inc"

  .section .text.avrx_avrx_reschedule,"ax",@progbits
/* ========================================================================== */
  .extern vAvrXIntProlog
  .extern vAvrXIntEpilog
  .extern vTaskQueuePid
  .extern prvvSemRemoveObject

  .text
/* -----------------------------------------------------------------------------
 *
 * void AvrXIntReschedule(void)
 *
 * Yield function: to allow cooperative tasking amoung like priority tasks.
 *
 * NOTE: Can only be called from AvrX Kernel mode (e.g. inside an interrupt
 * handler).  Reschedules the top of the run queue, which, as noted below, may
 * not actually be the current running task.
 */
  .global AvrXIntReschedule
  .func AvrXIntReschedule
// -----------------------------------------------------------------------------
AvrXIntReschedule:
	lds	Zl, xKernelData+RunQueue+NextL	// Grab the top of the run queue
	lds	Zh, xKernelData+RunQueue+NextH
	adiw	Zl, 0
	brne	air1
	ret				// Exit if empty
air1:
	ldd	p1l, Z+NextL		// Point to the next
	ldd	p1h, Z+NextH
	sts	RunQueue+NextL, p1l
	sts	RunQueue+NextH, p1h
	rjmp	vTaskQueuePid

  .endfunc

/* -----------------------------------------------------------------------------
 *
 * void vAvrXYield(void)
 *
 * Removes self from run queue and re-queues.  If other tasks of the
 * same priority are on the queue, self will queue behind them (round robin)
 *
 * Note: we remove "Running" which may or may not be the top of the queue
 * depending upon whatever other interrupt activity might be going on.  If
 * we just removed the top of the run queue, it would work virutally all the
 * time, but technically, it could fail to reschedule the correct task (only
 * once in a blue moon...)
 */
  .global vAvrXYield
  .func vAvrXYield
// -----------------------------------------------------------------------------
vAvrXYield:
  BeginCritical                   // 051704 - per email.
	AVRX_Prolog
	ldi	Zl, lo8(xKernelData+RunQueue)
	ldi	Zh, hi8(xKernelData+RunQueue)
	ldd	p2l, Z+Running+NextL
	ldd	p2h, Z+Running+NextH
	rcall	prvvSemRemoveObject		// Can't fail, so don't bother checking
	EndCritical
	mov	p1l, p2l
	mov	p1h, p2h
	rcall	vTaskQueuePid		// requeue ourselves
	rjmp vAvrXIntEpilog // jump to the new top of the queue

  .endfunc

/* ========================================================================== */
  .end
