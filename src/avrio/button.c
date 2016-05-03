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
 * @file button.c
 * @brief Gestion des boutons poussoirs.
 */
#include "avrio-config.h"

#ifdef AVRIO_BUTTON_ENABLE
/* ========================================================================== */
#  include <avrio/button.h>
#  include <avrio/delay.h>

/* public variables ========================================================= */
DECLARE_BUTTON_MASK_ARRAY;

/* internal public functions ================================================ */
// TODO: Gestionnaire avec une tâche

// ------------------------------------------------------------------------------
void
vButInit (void) {

  vButHardwareInit ();
}

#define TH_MIN -10
#define TH_MAX 20

// ------------------------------------------------------------------------------
xButMask
xButGet (xButMask xMask) {
  static xButMask xReturnMask;
  xButMask xPrevMask;

  xPrevMask = xButHardwareGet (xMask);
  
  if (xPrevMask != xReturnMask) {
    xButMask xNewMask;
    
    xNewMask = xButHardwareGet (xMask);
    if (xNewMask == xPrevMask) {
    
      xReturnMask = xPrevMask;
      delay_ms (20);
    }
  }

  return xReturnMask;
}

#endif /* AVRIO_BUTTON_ENABLE defined */
/* ========================================================================== */
