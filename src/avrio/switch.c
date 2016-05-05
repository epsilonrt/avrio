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
 * @file switch.c
 * @brief Gestion des micro-interrupteurs.
 */
#include "avrio-config.h"

#ifdef AVRIO_SWITCH_ENABLE
/* ========================================================================== */
#  include <avrio/switch.h>
#  include <avrio/delay.h>

/* internal public functions ================================================ */

// -----------------------------------------------------------------------------
void
vSwitchInit (void) {

  vSwitchHwInit();
}

// -----------------------------------------------------------------------------
xSwitch
xSwitchGet (void) {
  static xSwitch xReturnMask;
  xSwitch xPrevMask;

  xPrevMask = xSwitchHwGet ();

  if (xPrevMask != xReturnMask) {
    xSwitch xNewMask;

    xNewMask = xSwitchHwGet ();
    if (xNewMask == xPrevMask) {

      xReturnMask = xPrevMask;
      delay_ms (20);
    }
  }

  return xReturnMask;
}
#endif /* AVRIO_SWITCH_ENABLE defined */
/* ========================================================================== */
