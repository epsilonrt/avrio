/**
 * @file generate_single_step_interrupt.c
 * @brief Génération d'interruption pour le mode pas à pas
 * @author Copyright © 2011-2012 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * @version $Id$
 * Revision History
 *    20110830 - Initial version epsilonRT
 */
#include <avrx/private.h>

#ifdef AVRX_SINGLESTEP_ENABLE
/* ========================================================================== */

// -----------------------------------------------------------------------------
void
vAvrXGenerateInterrupt (void) {

  vKernelIrqGenerate ();
}

/* ========================================================================== */
#endif /* AVRX_SINGLESTEP_ENABLE defined */
