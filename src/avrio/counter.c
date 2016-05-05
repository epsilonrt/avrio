/**
 * @file counter.c
 * @brief Comptage d'événement (Implémentation)
 *
  * Copyright © 2015 epsilonRT. All rights reserved.
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
