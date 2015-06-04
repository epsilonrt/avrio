/**
 * @file icounter.c
 * @brief Comptage des signaux d'interruption (Implémentation)
 *
  * Copyright © 2015 Pascal JEAN aka epsilonRT. All rights reserved.
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
 */
#include "avrio-config.h"

#if defined(AVRIO_IRQ_ENABLE) && defined(AVRIO_TASK_ENABLE)
/* ========================================================================== */
#include <util/atomic.h>
#include "icounter.h"
#include "led.h"

/* constants ================================================================ */
#define DEFAULT_WINDOW 100

/* private functions ======================================================== */

// -----------------------------------------------------------------------------
static void
vIsr (xIrqHandle i) {
  xICounter * c = (xICounter *) pvIrqGetUserData (i);

  c->usCounter++;
}

// -----------------------------------------------------------------------------
static void
vWindowTask (xTaskHandle t) {
  xICounter * c = (xICounter *) pvTaskGetUserData (t);

  c->usLastValue = c->usCounter;
  c->usCounter = 0;
  vMutexUnlock (&c->xReady);

  if (c->eMode == eICounterFreeRunning) {

    vTaskStart (c->xTask);
  }
  else {

    vIrqDisable (c->xInt);
  }
}

/* internal public functions ================================================ */

// -----------------------------------------------------------------------------
void
vICounterInit (xICounter * c, xIrqHandle i) {

  c->xInt = i;
  c->usCounter = c->usLastValue = 0;
  c->xReady = MUTEX_INITIALIZER;
  c->eMode = eICounterSingle;
  vICounterSetWindow (c, DEFAULT_WINDOW);
  c->xTask = xTaskCreate (xTaskConvertMs (DEFAULT_WINDOW), vWindowTask);
  vTaskSetUserData (c->xTask, c);
  vIrqSetUserData (i, c);
  vIrqAttach (i, vIsr, eIrqRising);
  vIrqDisable (i);
}

// -----------------------------------------------------------------------------
void
vICounterStart (xICounter * c) {

  vMutexLock (&c->xReady);
  ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {

    vTaskStart (c->xTask);
    vIrqEnable (c->xInt);
  }
}

// -----------------------------------------------------------------------------
void
vICounterSetMode (xICounter * c, eICounterMode m) {

  vMutexLock (&c->xReady);
  c->eMode = m;
  vMutexUnlock (&c->xReady);
}

// -----------------------------------------------------------------------------
void
vICounterSetWindow (xICounter * c, uint16_t usWindowMs) {

  vMutexLock (&c->xReady);
  vTaskSetInterval (c->xTask, xTaskConvertMs (usWindowMs));
  c->usWindow = xTaskConvertTicks (xTaskGetInterval (c->xTask));
  vMutexUnlock (&c->xReady);
}

// -----------------------------------------------------------------------------
bool
bICounterIsComplete (xICounter * c) {

  if (xMutexTryLock (&c->xReady) == 0) {

    vMutexUnlock (&c->xReady);
    return true;
  }
  return false;
}

// -----------------------------------------------------------------------------
void
vICounterWaitForComplete (xICounter * c) {

  vMutexLock (&c->xReady);
  vMutexUnlock (&c->xReady);
}

// -----------------------------------------------------------------------------
double
dICounterFreq (xICounter * c) {

  return (double) c->usLastValue * 1000. / (double) c->usWindow;
}

// -----------------------------------------------------------------------------
uint16_t
usICounterCount (xICounter * c) {

  return c->usLastValue;
}

#endif /* AVRIO_IRQ_ENABLE defined */
/* ========================================================================== */
