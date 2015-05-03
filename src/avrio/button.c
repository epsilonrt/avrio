/**
 * @file button.c
 * @brief Gestion des boutons poussoirs.
 * @author Copyright © 2011-2012 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>

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
