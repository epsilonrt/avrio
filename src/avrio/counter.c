/**
 * @file counter.c
 * @brief Comptage d'événement (Implémentation)
 *
  * Copyright © 2015 epsilonRT. All rights reserved.
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

#if defined(AVRIO_TASK_ENABLE)
/* ========================================================================== */
#include <util/atomic.h>
#include "counter.h"
#include "led.h"

/* constants ================================================================ */
#define DEFAULT_WINDOW 100

/* private functions ======================================================== */

// -----------------------------------------------------------------------------
static void
vWindowTask (xTaskHandle t) {
  xCounter * c = (xCounter *) pvTaskGetUserData (t);

  c->usLastValue = c->xOps->read();
  c->xOps->clear();
  vMutexUnlock (&c->xReady);

  if (c->eMode == eCounterFreeRunning) {

    vTaskStart (c->xTask);
  }
  else {

    c->xOps->enable (false);;
  }
}

/* internal public functions ================================================ */

// -----------------------------------------------------------------------------
void
vCounterInit (xCounter * c, xCounterOps * o) {

  c->xOps = o;
  c->xReady = MUTEX_INITIALIZER;
  c->eMode = eCounterSingle;
  vCounterSetWindow (c, DEFAULT_WINDOW);
  c->xTask = xTaskCreate (xTaskConvertMs (DEFAULT_WINDOW), vWindowTask);
  vTaskSetUserData (c->xTask, c);
  o->init();
  o->enable (false);
  o->clear();
}

// -----------------------------------------------------------------------------
void
vCounterStart (xCounter * c) {

  vMutexLock (&c->xReady);
  ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {

    vTaskStart (c->xTask);
    c->xOps->enable (true);
  }
}

// -----------------------------------------------------------------------------
void
vCounterSetMode (xCounter * c, eCounterMode m) {

  vMutexLock (&c->xReady);
  c->eMode = m;
  vMutexUnlock (&c->xReady);
}

// -----------------------------------------------------------------------------
void
vCounterSetWindow (xCounter * c, uint16_t usWindowMs) {

  vMutexLock (&c->xReady);
  vTaskSetInterval (c->xTask, xTaskConvertMs (usWindowMs));
  c->usWindow = xTaskConvertTicks (xTaskGetInterval (c->xTask));
  vMutexUnlock (&c->xReady);
}

// -----------------------------------------------------------------------------
bool
bCounterIsComplete (xCounter * c) {

  if (xMutexTryLock (&c->xReady) == 0) {

    vMutexUnlock (&c->xReady);
    return true;
  }
  return false;
}

// -----------------------------------------------------------------------------
void
vCounterWaitForComplete (xCounter * c) {

  vMutexLock (&c->xReady);
  vMutexUnlock (&c->xReady);
}

// -----------------------------------------------------------------------------
double
dCounterFreq (xCounter * c) {

  return (double) c->usLastValue * 1000. / (double) c->usWindow;
}

// -----------------------------------------------------------------------------
uint16_t
usCounterCount (xCounter * c) {

  return c->usLastValue;
}

#endif /* AVRIO_TASK_ENABLE defined */
/* ========================================================================== */
