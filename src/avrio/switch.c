/**
 * @file switch.c
 * @brief Gestion des micro-interrupteurs.
 * @author Copyright © 2014 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>

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
