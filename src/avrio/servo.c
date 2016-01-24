/**
 * Copyright © 2016 Pascal JEAN aka epsilonRT. All rights reserved.
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
 * @file servo.c
 * @brief Servo-moteurs (Implémentation)
 */
#include "avrio-config.h"

#ifdef AVRIO_SERVO_ENABLE
/* ========================================================================== */
#include <avrio/servo.h>
#include "avrio-board-servo.h"

/* macros =================================================================== */
/* constants ================================================================ */
/* structures =============================================================== */
/* types ==================================================================== */
/* private variables ======================================================== */
/* private functions ======================================================== */
/* public variables ========================================================= */
static uint16_t usServoTop;

/* internal public functions ================================================ */

// -----------------------------------------------------------------------------
void
vServoInit (void) {

  usServoTop = usServoTimerInit();
}

// -----------------------------------------------------------------------------
uint8_t
ucServoChannels (void) {
  return SERVO_CHANNELS;
}

// -----------------------------------------------------------------------------
void
vServoSetPulse (uint8_t ucChannel, uint16_t usPulseUs) {
  uint16_t usSetting;

  usSetting = ( (uint32_t) usPulseUs * (uint32_t) usServoTop) / SERVO_PERIOD_US;
  vServoTimerSet (ucChannel, usSetting);
}

// -----------------------------------------------------------------------------
uint16_t
usServoGetPulse (uint8_t ucChannel) {
  uint16_t usSetting = usServoTimerGet (ucChannel);

  return ( (uint32_t) usSetting * SERVO_PERIOD_US) / (uint32_t) usServoTop;
}

// -----------------------------------------------------------------------------
void
vServoEnable (uint8_t ucChannel, bool bEnabled) {

  if (bEnabled) {

    vServoTimerEnable (ucChannel);
  }
  else {

    vServoTimerDisable (ucChannel);
  }
}

#endif /* AVRIO_SERVO_ENABLE defined */
/* ========================================================================== */
