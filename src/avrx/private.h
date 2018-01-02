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
 * @file avrx_private.h
 * @brief Déclarations privées du module
 * Copyright © 2011-2012 epsilonRT. All rights reserved.
 * @author Larry Barello, <larry@barello.net>. 
 *         Copyright © 1998-2002 All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 */
#ifndef _AVRX_PRIVATE_H_
#define _AVRX_PRIVATE_H_

#include <avrio/avrx.h>
#include "avrio-config.h"
#include "avrio-board-kernel.h"

#if defined(__cplusplus)
  extern "C" {
#endif
#  if !defined(__DOXYGEN__)
  /* ======================================================================== */

  /* macros ================================================================= */
  /*
   * @brief Declare the top level C declaration for an interrupt handler 
   */
#  define AVRX_SIGINT(vector)  AVRX_NAKED_FUNC(vector)

  /*
   * @brief Invalide les interruptions 
   */
#  define AVRX_BEGIN_CRITICAL() asm volatile ("cli\n")

  /*
   * @brief Valide les interruptions 
   */
#  define AVRX_END_CRITICAL()   asm volatile ("sei\n")

  /* constants ============================================================== */
  /* structures ============================================================= */

  //----------------------------------------------------------------------------
  typedef struct xAvrXKernelData {
  
    xPid RunQueue;
    xPid Running;
    void *              AvrXStack;
    uint8_t             SysLevel;
  } xAvrXKernelData;
 
  /* types ================================================================== */
  /* internal public functions ============================================== */
 /*
  * Pushes entire register context onto the stack, returning a frame pointer
  * to the saved context.  If running in user mode upon entry (SysLevel ==
  * 0xFF) then switches the stack to the kernel and stores the frame pointer
  * in the current processes PID.
  */
  void vAvrXIntProlog (void);

  /*
   * Restore previous context (kernel or user).  If task has SingleStep flag
   * set, then generate an interrupt before returning to the task.
   */
  void vAvrXEpilog (void);

  /*
   * Kernel Funtion called by timer ISR
   */
  void vAvrXTimerHandler (void);


  /* public variables ======================================================= */
  extern xAvrXKernelData xKernelData;
  extern xTimer * pxTimerQueue;
  extern uint8_t ucTimQLevel;
  extern xSem xEEPromSem;

  /* inline public functions ================================================ */

  /* ======================================================================== */
#  endif /* __DOXYGEN__ defined */
#if defined(__cplusplus)
  }
#endif
#endif /* _AVRX_PRIVATE_H_ not defined */
