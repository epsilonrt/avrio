/**
 * Copyright © 2011-2015 Pascal JEAN aka epsilonRT. All rights reserved.
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
 * @file bdcm.c
 * @brief Commande PWM de moteur DC
 */
#include "avrio-config.h"

#ifdef AVRIO_BDCM_ENABLE
/* ========================================================================== */
#  include "avrio-board-bdcm.h"
#  include <avrio/bdcm.h>

/* private variables ======================================================== */
static uint16_t usBdcmTop;
static uint16_t usBdcmPwmFreq;
static int16_t asBdcmRatio[BDCM_MAX];

/* internal public functions ================================================ */

// ------------------------------------------------------------------------------
void
vBdcmInit (uint16_t usFreq) {

  usBdcmTop = usBdcmHarwareInit (usFreq);
  usBdcmPwmFreq = usFreq;
}

// ------------------------------------------------------------------------------
void
vBdcmSetRatio (uint8_t ucMotor, int16_t sRatio) {

  if (sRatio == 0) {

    vBdcmHardwareControl (BDCM_STOP, ucMotor);
  } else {
    uint32_t ulSetting = ((uint32_t) usBdcmTop * ABS (sRatio)) / 10000UL;

    if (sRatio > 0)
      vBdcmHardwareControl (BDCM_FORWARD, ucMotor);
    else
      vBdcmHardwareControl (BDCM_REVERSE, ucMotor);
    vBdcmHardwareSet ((uint16_t) ulSetting, ucMotor);
  }
  if (ucMotor < BDCM_MAX) {

    asBdcmRatio[ucMotor] = sRatio;
  }

}

// ------------------------------------------------------------------------------
int16_t
sBdcmRatio (uint8_t ucMotor) {

  if (ucMotor < BDCM_MAX) {

    return asBdcmRatio[ucMotor];
  }
  return 0;
}

// ------------------------------------------------------------------------------
uint16_t
usBdcmFreq (void) {

  return usBdcmPwmFreq;
}

#endif /* AVRIO_BDCM_ENABLE defined */
/* ========================================================================== */
