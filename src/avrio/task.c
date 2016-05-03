/**
 * Copyright © 2011-2015 epsilonRT. All rights reserved.
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
 * @file task.c
 * @brief Module multitâche collaboratif.
 */
#include "avrio-config.h"

#if defined(AVRIO_TASK_ENABLE) && !defined(AVRIO_AVRX_ENABLE)
/* ========================================================================== */
#include <avrio/osc.h>
#include "avrio-board-kernel.h"
#include <avr/interrupt.h>
#include <util/atomic.h>
#include <avrio/task.h>

/* private variables ======================================================== */
static volatile ticks_t xTaskTime; /* temps système */
static volatile uint8_t xTaskSize;  /* nombre da tâche */
static volatile uint8_t xTaskFlags; /* Drapeaux d'exécution des pxTasks */

/* constants ================================================================ */
#define AVRIO_KERNEL_MAX (sizeof(xTaskFlags) * 8)

/* public variables ========================================================= */
volatile xTask pxTasks[AVRIO_KERNEL_MAX]; /* tableaux des pxTasks */

/* constants ================================================================ */
#define KID_WAIT 0

/* private functions ======================================================== */
#ifndef AVRIO_KERNEL_CRITICAL
/* 
 * ISR_NOBLOCK indique que cette fonction n'est pas prioritaire et pourra être
 * interrompue par une autre interruption.
 */
ISR (AVRIO_KERNEL_vect, ISR_NOBLOCK) {
#else
ISR (AVRIO_KERNEL_vect) {
#endif
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
