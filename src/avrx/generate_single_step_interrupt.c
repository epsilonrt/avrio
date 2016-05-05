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
