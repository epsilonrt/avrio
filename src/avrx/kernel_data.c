/**
 * @file kernel_data.c
 * @brief Variables globales utilisées par le noyau
 * @author Copyright © 2011-2012 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
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
