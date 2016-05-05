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
