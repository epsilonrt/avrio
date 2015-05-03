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
