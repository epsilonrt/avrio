/**
 * @file lcd_backlight.c
 * @brief Rétroéclairage d'afficheur LCD
 * @author Copyright © 2011-2012 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * @version $Id$
 */
#include "avrio-config.h"

#ifdef AVRIO_LCD_BACKLIGHT_ENABLE
/* ========================================================================== */
#  include "avrio-board-lcdbacklight.h"
#  include <avrio/lcd_backlight.h>
/* internal public functions ================================================ */
// ------------------------------------------------------------------------------
void
vLcdCtrlBacklightInit (void) {

  vLcdDriverBacklightInit ();
  iLcdBacklightSet (0);
}

// ------------------------------------------------------------------------------
int8_t
iLcdBacklightSet (int8_t iPercent) {

  iPercent &= 0b00111111;
  vLcdDriverBacklightSet (iPercent);
  return iPercent;
}
#endif /* AVRIO_LCD_BACKLIGHT_ENABLE defined */
/* ========================================================================== */
