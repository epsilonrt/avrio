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
 * @file kernel_data.c
 * @brief Variables globales utilisées par le noyau
 *
 * Revision History ------------------------------------------------------------
 *    20110831 - Initial version by epsilonRT
 */
#include <avrx/private.h>

/* public variables ========================================================= */

/* -----------------------------------------------------------------------------
 * Critical Data Structures (now a data structure, see avrx.inc for offsets
 *
 * - RunQueue is the head of the run queue.
 * - Running is current running task - once inside a Prolog/vAvrXEpilog
 *   section, the RunQueue might get re-arranged so we need to track
 *   who we are.
 * - Syslevel is a counter tracking how many times we have reentered
 *   the kernel, or, how many interrupts are nested (same thing).
 * - Offsets are for index access to kernel data - this is probably
 *   broken for IAR and maybe even GCC if the assemblers don't pack
 *   the data on byte boundaries.
 */
xAvrXKernelData xKernelData;

// Head of the timer queue
xTimer * pxTimerQueue;

// Recurse level of timer interrupts
uint8_t ucTimQLevel;

// Sémaphore d'accès à l'EEPROM
xSem xEEPromSem;

/* ========================================================================== */
