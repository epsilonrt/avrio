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
