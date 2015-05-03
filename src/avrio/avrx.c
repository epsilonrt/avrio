/**
 * @file avrx.c
 * @brief Intégration du noyau multitâche temps réel AvrX
 * @author Copyright © 2011-2012 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>

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
