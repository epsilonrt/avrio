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
 * @file compat.h
 * @brief Compatibilité ascendante AvxX
 */
#ifndef _AVRX_COMPAT_H_
#  define _AVRX_COMPAT_H_

#  include <avrio/avrx.h>

__BEGIN_C_DECLS
  /* ======================================================================== */
  /**
   * @addtogroup avrx_module
   * @{
   *
   *  @defgroup avrx_compat_module Compatibilité ascendante
   *
   *  Permet une compatibilité du code utilisateur avec la version précédante
   *  d'AvrX (2.6f)
   *  @deprecated Ne devrait plus être utilisé pour un nouveau code source. 
   *  @{
   */

  /* macros ================================================================= */
#  define AVRX_TASKDEF AVRX_TASK
#  define AVRX_EXTERNTASK AVRX_EXTERN_TASK
#  define AVRX_GCC_TASK AVRX_DECLARE_TASK
#  define AVRX_GCC_TASKDEF AVRX_TASKDEF

#  define AVRX_IAR_TASK(A,B,C,D)    // IAR n'est plus supporté, désolé !
#  define AVRX_IAR_TASKDEF(A,B,C,D) // IAR n'est plus supporté, désolé !


  /**
   * @brief Déclaration d'une variable timer 
   * @deprecated Ne plus utiliser cette macro !
   */
#  define AVRX_TIMER(A) xTimer A

  /**
   * @brief Déclaration d'une variable xSem
   * @deprecated Ne plus utiliser cette macro !
   */
#  define AVRX_SEMAPHORE(A) xSem A

  /**
   * @brief Déclaration d'une variable xSem
   * Un mutex est un sémaphore BINAIRE (qui ne peut prendre que 2 valeur 
   * S ou P). Il n'est donc pas logique d'utiliser le terme mutex pour une 
   * variable qui peut prendre plus de 2 valeurs (car des sémaphores AvrX) !
   * @deprecated Ne plus utiliser cette macro qui est ambigüe !
   */
#  define AVRX_MUTEX(A) AVRX_SEMAPHORE(A)

  /**
   * @brief Déclaration d'une variable Message (MCB)
   * @deprecated Ne plus utiliser cette macro !
   */
#  define AVRX_MESSAGE(A) xMessage A

  /**
   * @brief Déclaration d'une variable file de messages
   * @deprecated Ne plus utiliser cette macro !
   */
#  define AVRX_MESSAGEQ(A) xMessageQueue A

    /* constants ============================================================ */
  /**
   * @brief Timer nul
   * @deprecated Ne plus utiliser cette macro !
   */
#  define NOTIMER ((xTimer *)0)

  /**
   * @brief Message nul
   * @deprecated Ne plus utiliser cette macro !
   */
#  define NOMESSAGE ((xMessage *)0)

  /**
   * @brief PID nul
   * @deprecated Ne plus utiliser cette macro !
   */
#  define NOPID ((xPid)0)

  /**
   * @deprecated Ne plus utiliser cette macro !
   */
#  define SEM_PEND AVRX_PEND

  /**
   * @deprecated Ne plus utiliser cette macro !
   */
#  define SEM_DONE AVRX_DONE

  /**
   * @deprecated Ne plus utiliser cette macro !
   */
#  define SEM_WAIT AVRX_WAIT

    /* macros =============================================================== */

    
    /* macros =============================================================== */
  
  /* types ================================================================== */
  typedef xProcess ProcessID;
  typedef xSem Mutex;
  typedef xMessage MessageControlBlock;
  typedef xMessageQueue MessageQueue;
  typedef xTimer TimerControlBlock;
  typedef xTimerMessage TimerMessageBlock;

  typedef ProcessID * pProcessID;
  typedef Mutex * pMutex;
  typedef MessageControlBlock * pMessageControlBlock;
  typedef MessageQueue * pMessageQueue;
  typedef TimerControlBlock * pTimerControlBlock;
  typedef TimerMessageBlock * pTimerMessageBlock;

  /* internal public functions ============================================== */
/* Tasks */
#  define AvrXRunTask vAvrXRunTcb
#  define AvrXInitTask xAvrXInitTcb
#  define AvrXResume vAvrXResume
#  define AvrXSuspend vAvrXSuspend
#  define AvrXTerminate vAvrXTerminate
#  define AvrXTaskExit vAvrXTaskExit
#  define AvrXWaitTask vAvrXWaitTask
#  define AvrXTestPid xAvrXTestPid
#  define AvrXPriority xAvrXPriority
#  define AvrXChangePriority xAvrXChangePriority
#  define AvrXSelf pxAvrXSelf

/* Tasks Debug */
#  define AvrXHalt vAvrXHalt
#  define AvrXBreakPoint vAvrXBreakPoint
#  define AvrXSingleStep xAvrXSingleStep
#  define AvrXSingleStepNext xAvrXSingleStepNext

/* Interrupt */
#  define IntProlog vAvrXIntProlog
#  define Epilog vAvrXEpilog
#  define _Epilog _vAvrXEpilog

/* Sémaphores */
#  define AvrXSetSemaphore vAvrXSetSemaphore
#  define AvrXIntSetSemaphore vAvrXIntSetSemaphore
#  define AvrXWaitSemaphore vAvrXWaitSemaphore
#  define AvrXTestSemaphore xAvrXTestSemaphore
#  define AvrXIntTestSemaphore xAvrXIntTestSemaphore
#  define AvrXResetSemaphore vAvrXResetSemaphore
#  define AvrXResetObjectSemaphore vAvrXResetObjectSemaphore

/* Messages */
#  define AvrXRecvMessage pxAvrXRecvMessage
#  define AvrXWaitMessage pxAvrXWaitMessage
#  define AvrXSendMessage vAvrXSendMessage
#  define AvrXIntSendMessage vAvrXIntSendMessage
#  define AvrXAckMessage vAvrXAckMessage
#  define AvrXWaitMessageAck vAvrXWaitMessageAck
#  define AvrXTestMessageAck xAvrXTestMessageAck

/* Timers */
#  define AvrXStartTimer vAvrXStartTimer
#  define AvrXCancelTimer pxAvrXCancelTimer
#  define AvrXDelay vAvrXDelay
#  define AvrXWaitTimer vAvrXWaitTimer
#  define AvrXTestTimer xAvrXTestTimer
#  define AvrXTimerHandler vAvrXTimerHandler
#  define AvrXStartTimerMessage vAvrXStartTimerMessage
#  define AvrXCancelTimerMessage pxAvrXCancelTimerMessage

/* EEPROM */
#  define AvrXReadEEProm ucAvrXReadEEProm
#  define AvrXReadEEPromWord usAvrXReadEEPromWord
#  define AvrXWriteEEProm vAvrXWriteEEProm

  /* public variables ======================================================= */
  /* inline public functions ================================================ */
  /**
   *   @}
   * @}
   */
  /* ======================================================================== */
__END_C_DECLS
#endif /* _AVRX_COMPAT_H_ not defined */
