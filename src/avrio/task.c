/**
 * @file task.c
 * @brief Module multitâche collaboratif.
 * @author Copyright © 2011-2012 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>

 */
#include "avrio-config.h"

#if defined(AVRIO_TASK_ENABLE) && !defined(AVRIO_AVRX_ENABLE)
/* ========================================================================== */
#  include "avrio-board-kernel.h"
#  include <avr/interrupt.h>
#  include <util/atomic.h>
#  include <avrio/task.h>

/* types ==================================================================== */

/* structures =============================================================== */
struct xTask {
  ticks_t xInterval;  /* période en ticks d'exécution de la fonction */
  ticks_t xValue; /* valeur courante en ticks de la tâche */
  xTaskFunction xFunction;  /* pointeur sur la fonction de la tâche */
};

/* private variables ======================================================== */
static volatile ticks_t xTaskTime; /* temps système */
static volatile uint8_t xTaskSize;  /* nombre da tâche */

static volatile uint8_t xTaskFlags; /* Drapeaux d'exécution des pxTasks */

#  define AVRIO_KERNEL_MAX (sizeof(xTaskFlags) * 8)

static volatile struct xTask pxTasks[AVRIO_KERNEL_MAX]; /* tableaux des pxTasks */

/* constants ================================================================ */
#  define KID_WAIT 0

/* private functions ======================================================== */
#  ifndef AVRIO_KERNEL_CRITICAL
/* 
 * ISR_NOBLOCK indique que cette fonction n'est pas prioritaire et pourra être
 * interrompue par une autre interruption.
 */
ISR (AVRIO_KERNEL_vect, ISR_NOBLOCK) {
#  else
ISR (AVRIO_KERNEL_vect) {
#  endif
  xTaskHandle xTask;

  vKernelIrqDisable (); /* évite la récursivité de l'it timer */
  xTaskTime++;
  for (xTask = 0; xTask < xTaskSize; xTask++) {

    if (bit_is_set (xTaskFlags, xTask)) {

      if (pxTasks[xTask].xValue-- == 0) {

        cbi (xTaskFlags, xTask);
        if (pxTasks[xTask].xFunction != 0)
          pxTasks[xTask].xFunction (xTask); /* exécute la fonction de service */
      }
    }
  }
  vKernelIrqEnable ();
}

/* 
 * Partie de code exécutée lors de la phase 5 de l'init.
 */
void vTaskInit5 (void) __attribute__ ((naked))
   __attribute ((section (".init5")));
void
vTaskInit5 (void) {

  vKernelHardwareInit ();
  (void) xTaskCreate (0, 0);  /* tâche pour delay_ms */
  vKernelIrqEnable ();
  sei ();
}

/* internal public functions ================================================ */

/* 
 * Ajout d'une tâche
 */
xTaskHandle
xTaskCreate (ticks_t xIntervalTicks, xTaskFunction xFunction) {

  if (xTaskSize >= AVRIO_KERNEL_MAX) {

    return AVRIO_KERNEL_ERROR;
  } else {
    xTaskHandle xTask = xTaskSize;

    vTaskSetInterval (xTask, xIntervalTicks);
    pxTasks[xTask].xFunction = xFunction;
    ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {
      xTaskSize++;
    }

    return xTask;
  }
}

/* 
 * Démarre une tâche
 */
void
vTaskStart (xTaskHandle xTask) {

  ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {
    pxTasks[xTask].xValue = pxTasks[xTask].xInterval;
    sbi (xTaskFlags, xTask);
  }
}

/* 
 * Arrête une tâche
 */
void
vTaskStop (xTaskHandle xTask) {

  ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {
    cbi (xTaskFlags, xTask);
  }
}

/* 
 * Indique si une tâche est lancé
 */
bool
xTaskIsStarted (xTaskHandle xTask) {
  bool xStarted;

  ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {
    xStarted = ((xTaskFlags & _BV (xTask)) != 0);
  }
  return xStarted;
}

/* 
 * Modifie la période d'une tâche
 */
void
vTaskSetInterval (xTaskHandle xTask, ticks_t xIntervalTicks) {

  ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {
    pxTasks[xTask].xInterval = MIN (xIntervalTicks - 1, xIntervalTicks);
  }
}

/* 
 * Convertit une valeur de temps (millisecondes) en ticks
 */
inline ticks_t
xTaskConvertMs (time_t xTimeMs) {

  return (ticks_t)(((uint32_t)(xTimeMs) * AVRIO_KERNEL_TICK_RATE) / 1000UL);
}

/* 
 * Attente
 */
void
vDelayWaitMs (time_t xMs) {
  bool xStarted;

  vTaskSetInterval (KID_WAIT, xTaskConvertMs (xMs));
  vTaskStart (KID_WAIT);
  do {

    xStarted = xTaskIsStarted (KID_WAIT);
  } while (xStarted != false);
}

/*
 * Renvoie le nombre de ticks depuis le démarrage du système
 */
ticks_t 
xTaskSystemTime (void) {
  ticks_t xCurrentTime;
  
  ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {
  
    xCurrentTime = xTaskTime;
  }
  return xCurrentTime;
}


#endif /* AVRIO_TASK_ENABLE defined and AVRIO_AVRX_ENABLE not defined */
/* ========================================================================== */
