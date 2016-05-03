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
 * @file encoder_irq.c
 * @brief Encodeurs incrémentaux
 */
#include "avrio-config.h"

#ifdef AVRIO_ENCODER_ENABLE
/* ========================================================================== */
#  include "avrio-board-encoder.h"
#  include <avrio/encoder.h>
#  include <avrio/task.h>
#  include <util/atomic.h>

/* private variables ======================================================== */
static volatile xEncoderCounter axEncoderCounter[ENCODER_MAX];
static volatile eEncoderDir aeEncoderDir[ENCODER_MAX];

/* private functions ======================================================== */
// ------------------------------------------------------------------------------
static uint8_t
prvucAtomicReadByte (volatile uint8_t * pucByte) {
  uint8_t ucValue;

  ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {
    ucValue = *pucByte;
  }
  return ucValue;
}

// -----------------------------------------------------------------------------
static inline void 
prvvUpdateCount (uint8_t ucEncoder) {

  if (bEncoderIrqPin (ucEncoder)) {

    axEncoderCounter[ucEncoder] += aeEncoderDir[ucEncoder];
  }
}

#ifdef ENCODER_DEBOUNCE_TIME
// -----------------------------------------------------------------------------
static xTaskHandle xEncoderDebounceTimer[ENCODER_MAX];

// -----------------------------------------------------------------------------
static void
prvvDebounceTask (xTaskHandle xHandle) {
  uint8_t ucEncoder;
  
  ucEncoder = (uint8_t)((xHandle - xEncoderDebounceTimer[0]) / sizeof (xTaskHandle));
  prvvUpdateCount (ucEncoder);
  vEncoderBoardEnable (ucEncoder, true);
}

// -----------------------------------------------------------------------------
#endif /* ENCODER_SPEED_PERIOD defined */

#ifdef ENCODER_SPEED_PERIOD
// -----------------------------------------------------------------------------
static volatile int16_t asEncoderSpeed[ENCODER_MAX];
static xTaskHandle xEncoderSpeedTimer;

// -----------------------------------------------------------------------------
static void
prvvSpeedTask (xTaskHandle xDummy __attribute__ ((unused))) {
  static xEncoderCounter axPreviousCounter[ENCODER_MAX];
  xEncoderCounter lCounter;
  uint8_t ucEncoder;

  for (ucEncoder = 0; ucEncoder < ENCODER_MAX; ucEncoder++) {

    lCounter = xEncoderCount (ucEncoder);
    asEncoderSpeed[ucEncoder] =
       (int16_t) (lCounter - axPreviousCounter[ucEncoder]);
    axPreviousCounter[ucEncoder] = lCounter;
  }
  vTaskStart (xEncoderSpeedTimer);
}


// -----------------------------------------------------------------------------
int16_t
xEncoderSpeed (uint8_t ucEncoder) {
  int16_t sSpeed;

  ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {
    sSpeed = asEncoderSpeed[ucEncoder];
  }
  return sSpeed;
}

// -----------------------------------------------------------------------------
#endif /* ENCODER_SPEED_PERIOD defined */

/* internal public functions ================================================ */

// ------------------------------------------------------------------------------
void
vEncoderInit (void) {
  uint8_t ucEncoder;

  vEncoderBoardInit ();

  for (ucEncoder = 0; ucEncoder < ENCODER_MAX; ucEncoder++) {

    vEncoderReset (ucEncoder);
#ifdef ENCODER_DEBOUNCE_TIME
    xEncoderDebounceTimer[ucEncoder] = xTaskCreate (xTaskConvertMs (ENCODER_DEBOUNCE_TIME),
                                 prvvDebounceTask);
#endif
    vEncoderEnable (ucEncoder, true);
  }
#ifdef ENCODER_SPEED_PERIOD
  xEncoderSpeedTimer = xTaskCreate (xTaskConvertMs (ENCODER_SPEED_PERIOD),
                               prvvSpeedTask);
  vTaskStart (xEncoderSpeedTimer);
#endif
#if !defined(ENCODER_DEBOUNCE_TIME) && !defined(ENCODER_SPEED_PERIOD)
  sei();
#endif
}

// -----------------------------------------------------------------------------
void
vEncoderTask (uint8_t ucEncoder) {
  eEncoderDir eDir;

  if (bEncoderDirPin(ucEncoder)) {

    eDir = ENCODER_CW;
  } 
  else {

    eDir = ENCODER_CCW;
  }
  aeEncoderDir[ucEncoder] = eDir;

#ifndef ENCODER_DEBOUNCE_TIME
  prvvUpdateCount (ucEncoder);
#else
  vEncoderBoardEnable (ucEncoder, false);
  vTaskStart (xEncoderDebounceTimer[ucEncoder]);
#endif
}

// -----------------------------------------------------------------------------
void
vEncoderEnable (uint8_t ucEncoder, bool xEnable) {

  vEncoderBoardEnable (ucEncoder, xEnable);
}

// -----------------------------------------------------------------------------
xEncoderCounter
xEncoderCount (uint8_t ucEncoder) {
  xEncoderCounter xCount;

  ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {
    xCount = axEncoderCounter[ucEncoder];
  }
  return xCount;
}

// -----------------------------------------------------------------------------
eEncoderDir
eEncoderDirection (uint8_t ucEncoder) {

  return (eEncoderDir) prvucAtomicReadByte ((uint8_t *) &
                                            aeEncoderDir[ucEncoder]);
}

// -----------------------------------------------------------------------------
void
vEncoderReset (uint8_t ucEncoder) {

  ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {
    axEncoderCounter[ucEncoder] = 0;
  }
}

#endif /* AVRIO_ENCODER_ENABLE defined */
/* ========================================================================== */
