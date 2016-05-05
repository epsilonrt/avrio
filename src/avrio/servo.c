/**
 * Copyright © 2016 epsilonRT. All rights reserved.
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
