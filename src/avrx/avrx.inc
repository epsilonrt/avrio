/**
 * @file avrx.inc
 * @brief Définitions et macros ASM pour AvrX
 * @author Larry Barello, <larry@barello.net>. 
 *         Copyright © 1998-2002 All rights reserved.
 * @author Pascal Jean, epsilonRT. Copyright © 2011 All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * @version $Id$
 * Revision History ---
 *    20110830 - Initial port to AvrIO by epsilonRT
 *    20020907 - Added _IOMACRO_H_ definition to prevent older GCC io.h files
 *               from including C only header information
 *	           - Added #defines for _SFR_IO_ADDR() macro to make it a NOP for
 *               pre GCC 3.2 compilers and for IAR (I wonder what they do for
 *	             extended I/O access?)
 *	             THIS works because avrx.inc loads "io.h" which loads "sfr-defs.h"
 *	             which defines _SFR_IO_ADDR() macro on the 3.2 compiler.
 */
#ifndef _AVRX_INC_
#define _AVRX_INC_
/* ========================================================================== */

#include <avrio/defs.h>
#include "avrx-ctoasm.inc"
#include "io.h"
#include "avrio-config.h"
#include "avrio-board-kernel.h"

/* public variables ========================================================= */
  .extern xKernelData
  .extern pxTimerQueue
  .extern ucTimQLevel
  .extern xEEPromSem

/* macros =================================================================== */
#if !defined(_SFR_IO_ADDR)      /* GCC 3.2 compatibility */
#define _SFR_IO_ADDR(A) (A)   /* earlier compilers do not use this */
#endif

/* constants ================================================================ */
/*
 * Frame Pointer Offsets for various registers after the return from
 * a call to _Prolog
 *
 * Used mainly to restore registers used internally (rather than saving
 * and restoring) and to stuff return values across vAvrXIntEpilog.
 */
#define _RetLo  33      /* Return address on stack big endian */
#define _R31    33      /* Registers saved in order (little endian) */
#define _RetHi  32
#define _R30    32
#define _R29    31
#define _R28    30
#define _R27    29
#define _R26    28
#define _R25    27
#define _R24    26
#define _R23    25
#define _R22    24
#define _R0     2
#define _SREG   1  /* SP points below last saved item */

#define _p1l    26
#define _p1h    27
#define _p2l    24
#define _p2h    25
#define _r1l    26
#define _r1h    27

/* -----------------------------------------------------------------------------
 *          AVRX kernel data structure offsets  (struct xKernelData)
 * These are offsets into a slab of memory called "xKernelData"
 * defined in avrx_tasking.s
 *
 * NB: CODE depends upon RunQueue being the FIRST item!
 */
#define RunQueue    0	/* Head of the run queue */
#define Running     2	/* Current running task */
#define AvrXStack   4	/* User defined stack location */
#define SysLevel    6	/* re-entry counter into kernel context */
/* Taille de la structure */
#define xKernelDataSz 7 

/* -----------------------------------------------------------------------------
 *        TCB (Task Control Block) offsets (struct xTaskControlBlock)
 */
#define TaskSP          0  /* Stack pointer */
#define TaskMain        2  /* Entry point of process */
#define TaskPid         4  /* Pointer to Process ID */
#define TaskFlags       6  /* Priority */
/* Taille de la structure */
#define TaskSz  7

/* -----------------------------------------------------------------------------
 *    All objects have same link list structure
 *    Change this to convert to big-endian for direct C access
 */
#define NextL 0		/* Big-endian for C compatibility */
#define NextH 1

/* -----------------------------------------------------------------------------
 *           PID (Process ID) block offsets (struct xProcess)
 */
#define PidNext         0 /* Next item on list (semaphore, run) */
#define PidState        2 /* Upper Nibble: Task flags, Lower Nibble :Priority */
#define PidPriority     3
#define PidSP           4 /* Context Pointer */

#ifdef AVRX_SINGLESTEP_ENABLE
#define bp1             6 /* Make room for two breakpoints/task */
#define bp2             8
/* Taille de la structure */
#define PidSz           10

#else
/* Taille de la structure */
#define PidSz           6
#endif

/* -----------------------------------------------------------------------------
 *           PID (Process ID) BLOCK BIT DEFINITIONS (PID.State)
 */
#define IdleBit         4 /* Dead Task, don't schedule, resume or step */
#define SuspendBit      5 /* Mark task for suspension (may be blocked elsewhere) */
#define SuspendedBit    6 /* Mark task suspended (it was removed from the run queue) */
#define SingleStep      7 /* Enable single step debug support */

/* -----------------------------------------------------------------------------
 *   SEMAPHORE BIT DEFINITIONS
 *
 *     General Semaphore Flags.  When a task is waiting on the
 *     semaphore, the semaphore is set to the TASK ID.  Otherwise
 *     _PEND means the semaphore is set and _DONE means it has
 *     been triggered.  A task will only block if it tries to
 *     grab a _PEND semaphore.  Semaphores are initialized in the
 *     _PEND state and are left in the _PEND state after each
 *     complete operation.  Processes are allowed to queue on
 *     a semaphore.  This effectively make the semaphore a Sem
 *     when it has been "set" prior to the first call.  When a process
 *     done with the semaphore, it releases it by "setting" it again.
-*/
#define _PEND           0
#define _DONE           1
/* Taille d'un semaphore */
#define SemSz           2

/* -----------------------------------------------------------------------------
 *  EVENTS
 *
 *    Events replace PID's in semaphores.  The kernel recognizes
 *    semaphore values (contents) < 0x60 as events, indexes into
 *    a table and calls a routine.  Macros and segments are used
 *    to build the table.
 */
#define TIMERMESSAGE_EV   2
#define PERIODICTIMER_EV  3      /* Not implemented */
#define _LASTEV           0x5F

/* -----------------------------------------------------------------------------
 *  TIMERS
 */
#define TcbNext         0  /* Pointer in linked list */
#define TcbSemaphore    2  /* Associated semaphore */
#define TcbCount        4  /* Timer ticks till expired */
#define TcbQueue        6
/* Tailles */
#define TcbSz           6  /* Primitive Timer */
#define TmbSz           8  /* Timer Message */

/* -----------------------------------------------------------------------------
 *  MESSAGE QUEUE
 */
#define MsqMessage      0  /* Head of list of messages */
#define MsqPid          2  /* Head of list of waiting processes */
#define MsqSz           4  /* Head of message queue */

#define QcbNext         0
#define QcbSemaphore    2  /* Return Receipt Semaphore */
#define QcbData         4  /* pointer to data/or data */
#define QcbSz           4  /* Queue Block Size (No data) */

/* -----------------------------------------------------------------------------
 *  FIFOS
 */
#define WriteSemaphore  0
#define ReadSemaphore   2
#define fBuffer         4
#define FifoSz 4   /* Size of AvrX Fifo Data Struct */

/* User section */
#define f_Size      0
#define f_Count     1
#define f_In        2
#define f_Out       3
#define f_Data      4   /* Variable length */
#define f_buffersz  4    /* Size of buffer header */

#ifndef AVRXCHEADER
/* -----------------------------------------------------------------------------
    Handy Macros

       DEBUG statements are diagnostics for twiddling and output port bit
       to reflect the I bit state (interrupt latency) and when AvrX is in
       the Idle loop
 */

/* DEBUG is set to a PORT{ABCD} value (for twiddling output bits) */
#ifdef  DEBUG

// TODO Utilisation de led.h
# ifndef ILED
#   define ILED     0
# endif

# ifndef IDLELED
#   define IDLELED  1
# endif

# define DEBUG_DDR  (DEBUG-1)

/* macros =================================================================== */
  .macro  Breakpoint
    rcall   AvrXBreakpoint
  .endm

  .macro  BeginInterrupt
    cbi     DEBUG, ILED
    sbi     DEBUG, IDLELED  /* Obviously not idle... */
  .endm

  .macro  EndInterrupt
    sbi     DEBUG, ILED
    reti
  .endm

  .macro BeginCritical
    cbi     DEBUG, ILED
    cli
  .endm

  .macro  EndCritical
    sbi     DEBUG, ILED
    sei
  .endm

  .macro  BeginIdle
snore:
    cbi     DEBUG, IDLELED
  .endm

  .macro  EndIdle
snooze:
    sbi     DEBUG, IDLELED
  .endm

  .macro  INIT_DEBUG
    ldi     Xl, (1<<ILED | 1<<IDLELED)
    out     DEBUG_DDR, Xl
    out     DEBUG, Xl
  .endm

#else /* DEBUG not defined */

/* macros =================================================================== */
  .macro  Breakpoint
  .endm

  .macro  BeginInterrupt
  .endm

  .macro  EndInterrupt
    reti
  .endm

  .macro BeginCritical
    cli
  .endm

  .macro  EndCritical
    sei
  .endm

  .macro  BeginIdle
snore:
  .endm

  .macro  EndIdle
snooze:
  .endm

  .macro  INIT_DEBUG
  .endm

#endif /* DEBUG not defined */

#define EndCriticalReturn EndInterrupt

/* -----------------------------------------------------------------------------
    Handy macros for common assembly chores
 */
  .macro  SignExtend Dest, Source
    clr     Dest
    sbrc    Source, 7
    com     Dest
  .endm

  .macro  addi reg, val
    subi    reg, -(val)
  .endm

  .macro  adci reg, val
    sbci    reg, -(val)
  .endm

  .macro  tstw reg
    adiw    reg, 0
  .endm

  .macro  negw regh, regl
    com     regh
    neg     regl
    sbci    regh, -1
  .endm

/*
 * Use this macro rather than a call to _Prolog, see
 * version notes in AvrX.asm
 */
  .macro  AVRX_Prolog
    BeginCritical
    rcall   vAvrXIntProlog
    EndCritical
  .endm

/*
 * These register definitions are just handy aliases for the
 * various index and word math registers
 */
#define		Wl 		r24
#define		Wh 		r25
#define		Xl 		r26
#define		Xh 		r27
#define		Yl 		r28
#define		Yh 		r29
#define		Zl 		r30
#define		Zh 		r31

#endif /*AVRXCHEADER */

/* ========================================================================== */
#endif  /* _AVRX_INC_ not defined */
