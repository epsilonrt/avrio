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
 * @file generate_single_step_interrupt.c
 * @brief Génération d'interruption pour le mode pas à pas
 *
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
