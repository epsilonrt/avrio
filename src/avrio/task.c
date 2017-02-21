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

/* types ==================================================================== */
typedef uint8_t xTaskFlag;

/* private variables ======================================================== */
static volatile ticks_t xTaskTime; /* temps système */
static volatile uint8_t xTaskSize;  /* nombre da tâche */
static volatile xTaskFlag xTaskStarted; /* Drapeaux d'exécution des pxTasks */
static volatile xTaskFlag xTaskCreated; /* Drapeaux de création des pxTasks */

/* constants ================================================================ */
#define AVRIO_KERNEL_MAX (sizeof(xTaskFlag) * 8)

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
  xTaskHandle t = 0;
  volatile xTaskFlag c = xTaskCreated;

  vKernelIrqDisable (); /* évite la récursivité de l'it timer */
  xTaskTime++;
  while (c) {

    if (bit_is_set (c, t)) {

      if (bit_is_set (xTaskStarted, t)) {

        if (pxTasks[t].xValue-- == 0) {

          cbi (xTaskStarted, t);
          if (pxTasks[t].xFunction != 0) {
            pxTasks[t].xFunction (t);  /* exécute la fonction de service */
          }
        }
      }
      cbi (c, t);
    }
    t++;
  }
  vKernelIrqEnable ();
}

/*
 * Partie de code exécutée lors de la phase 5 de l'init.
 */
void vTaskInit5 (void) __attribute__ ( (naked))
__attribute ( (section (".init5")));
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
  xTaskHandle t = 0;

  ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {

    // Recherche une tâche libre est la réserve
    while ( (bit_is_set (xTaskCreated, t)) && (t < AVRIO_KERNEL_MAX)) {

      t++;
    }
    if (t < AVRIO_KERNEL_MAX) {

      xTaskSize++;
      sbi (xTaskCreated, t);
    }
  }

  if (t >= AVRIO_KERNEL_MAX) {

    return AVRIO_KERNEL_ERROR;
  }

  vTaskSetInterval (t, xIntervalTicks);
  pxTasks[t].xFunction = xFunction;
  return t;
}

/*
 * Spprime une tâche
 */
void
vTaskDelete (xTaskHandle t) {

  ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {

    cbi (xTaskCreated, t);
    cbi (xTaskStarted, t);
  }
}

/*
 * Démarre une tâche
 */
void
vTaskStart (xTaskHandle t) {

  ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {
    pxTasks[t].xValue = pxTasks[t].xInterval;
    sbi (xTaskStarted, t);
  }
}

/*
 * Arrête une tâche
 */
void
vTaskStop (xTaskHandle t) {

  ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {
    cbi (xTaskStarted, t);
  }
}

/*
 * Indique si une tâche est lancé
 */
bool
xTaskIsStarted (xTaskHandle t) {
  bool xStarted;

  ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {
    xStarted = ( (xTaskStarted & _BV (t)) != 0);
  }
  return xStarted;
}

/*
 * Modifie la période d'une tâche
 */
void
vTaskSetInterval (xTaskHandle t, ticks_t xIntervalTicks) {

  ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {
    pxTasks[t].xInterval = MIN (xIntervalTicks - 1, xIntervalTicks);
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
  }
  while (xStarted != false);
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
