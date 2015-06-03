/**
 * @file freqmeter.c
 * @brief Mesure de fréquence d'un signal d'interruption (Implémentation)
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
#include "freqmeter.h"
#include "led.h"

/* constants ================================================================ */
#define DEFAULT_WINDOW 100

/* private functions ======================================================== */

// -----------------------------------------------------------------------------
static void
vIsr (xIrqHandle i) {
  xFreqMeter * f = (xFreqMeter *) pvIrqGetUserData (i);

  f->usCounter++;
}

// -----------------------------------------------------------------------------
static void
vWindowTask (xTaskHandle t) {
  xFreqMeter * f = (xFreqMeter *) pvTaskGetUserData (t);

  f->usLastValue = f->usCounter;
  f->usCounter = 0;
  vMutexUnlock (&f->xReady);

  if (f->eMode == eFreqMeterFreeRunning) {

    vTaskStart (f->xTask);
  }
  else {

    vIrqDisable (f->xInt);
  }
}

/* internal public functions ================================================ */

// -----------------------------------------------------------------------------
void
vFreqMeterInit (xFreqMeter * f, xIrqHandle i) {

  f->xInt = i;
  f->usCounter = f->usLastValue = 0;
  f->xReady = MUTEX_INITIALIZER;
  f->eMode = eFreqMeterSingle;
  vFreqMeterSetWindow (f, DEFAULT_WINDOW);
  f->xTask = xTaskCreate (xTaskConvertMs (DEFAULT_WINDOW), vWindowTask);
  vTaskSetUserData (f->xTask, f);
  vIrqSetUserData (i, f);
  vIrqAttach (i, vIsr, eIrqRising);
  vIrqDisable (i);
}

// -----------------------------------------------------------------------------
void
vFreqMeterStart (xFreqMeter * f) {

  vMutexLock (&f->xReady);
  ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {

    vTaskStart (f->xTask);
    vIrqEnable (f->xInt);
  }
}

// -----------------------------------------------------------------------------
void
vFreqMeterSetMode (xFreqMeter * f, eFreqMeterMode m) {

  vMutexLock (&f->xReady);
  f->eMode = m;
  vMutexUnlock (&f->xReady);
}

// -----------------------------------------------------------------------------
void
vFreqMeterSetWindow (xFreqMeter * f, uint16_t usWindowMs) {

  vMutexLock (&f->xReady);
  vTaskSetInterval (f->xTask, xTaskConvertMs (usWindowMs));
  f->usWindow = xTaskConvertTicks (xTaskGetInterval (f->xTask));
  vMutexUnlock (&f->xReady);
}

// -----------------------------------------------------------------------------
bool
bFreqMeterIsComplete (xFreqMeter * f) {

  if (xMutexTryLock (&f->xReady) == 0) {

    vMutexUnlock (&f->xReady);
    return true;
  }
  return false;
}

// -----------------------------------------------------------------------------
void
vFreqMeterWaitForComplete (xFreqMeter * f) {

  vMutexLock (&f->xReady);
  vMutexUnlock (&f->xReady);
}

// -----------------------------------------------------------------------------
double
dFreqMeterRead (xFreqMeter * f) {

  return (double) f->usLastValue * 1000. / (double) f->usWindow;
}

#endif /* AVRIO_IRQ_ENABLE defined */
/* ========================================================================== */
