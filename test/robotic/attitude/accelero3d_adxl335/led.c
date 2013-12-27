/**
 * @file led.c
 * @brief LED (diodes électroluminescentes)
 * @author Copyright © 2011-2012 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * @version $Id$
 */
#include "avrio-config.h"

#ifdef AVRIO_LED_ENABLE
/* ========================================================================== */
#  include <avrio/led.h>

/* public variables ========================================================= */
DECLARE_LED_MASK_ARRAY;
xLedMask xLedCurrent = 0;

/* internal public functions ================================================ */
// -----------------------------------------------------------------------------
void
vLedUpdate (void) {

  (void) eTwiWrite (LED_TWI_CTRL_LSB, (~xLedCurrent) & 0xFF);
  (void) eTwiWrite (LED_TWI_CTRL_MSB, ((~xLedCurrent) >> 8) & 0xFF);
}

#endif /* AVRIO_LED_ENABLE defined */
/* ========================================================================== */
