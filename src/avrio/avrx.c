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
 * @file avrx.c
 * @brief Intégration du noyau multitâche temps réel AvrX
 */

#include "avrio-config.h"

#ifdef AVRIO_AVRX_ENABLE
/* ========================================================================== */
#  include "avrio-board-kernel.h"
#  include <avr/interrupt.h>
#  include <avrio/delay.h>
#  include <avrx/private.h>

/* private functions ======================================================== */

/* -----------------------------------------------------------------------------
 * Kernel Interrupt handler
 * - Switch to kernel context
 * - Handle interrupt
 * - Switch back to interrupted context. */

AVRX_SIGINT (AVRIO_KERNEL_vect) {

  vAvrXIntProlog ();      // Save interrupted context, switch stacks
#ifndef AVRIO_KERNEL_CRITICAL
  AVRX_END_CRITICAL();
#endif

#ifdef AVRIO_TASK_ENABLE
  vAvrXTimerHandler ();   // Process Timer queue
#endif

  vAvrXEpilog ();         // Restore context of next running task
}

/* -----------------------------------------------------------------------------
 * Partie de code exécutée lors de la phase 5 de l'init.
 */
void vAvrXInit5 (void) __attribute__ ((naked))
   __attribute ((section (".init5")));
void
vAvrXInit5 (void) {

  vKernelHardwareInit ();
  vKernelIrqEnable ();
  pvAvrXSetKernelStack (0);
}

/* internal public functions ================================================ */

#endif /* AVRIO_AVRX_ENABLE defined */
/* ========================================================================== */
