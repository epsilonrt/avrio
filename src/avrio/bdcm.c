/**
 * @file bdcm.c
 * @brief Commande PWM de moteur DC
 * @author Copyright _ 2011-2012 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * @version $Id$
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
