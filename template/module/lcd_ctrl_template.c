/**
 * @file lcd_ctrl_template.c
 * @brief Modèle de Contrôleur LCD
 * @author epsilonRT. Copyright © 2012 All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * @version $Id$
 * Revision History ---
 *    20120501 - Initial version by epsilonRT
 */
#include "avrio-config.h"

#ifdef AVRIO_LCD_ENABLE
#include "avrio-board-lcd.h"

#if LCD_CTRL == LCD_CTRL_TEMPLATE
/* ========================================================================== */

/* constants ================================================================ */

/* private variables ======================================================== */
static xLcdCoord xLcdCtrlX, xLcdCtrlY;

/* private functions ======================================================== */

// -----------------------------------------------------------------------------

/* internal public functions ================================================ */
// -----------------------------------------------------------------------------
int8_t
iLcdCtrlInit (void) {
  int8_t iError;
  
  iError = iLcdIoInit ();
  
  if (iError == 0) {
  
  }
  
  return iError;
}

// -----------------------------------------------------------------------------
uint8_t 
ucLcdCtrlContrastSet (uint8_t ucContrast) {

}

// -----------------------------------------------------------------------------
void
vLcdCtrlPutChar (char cChar) {

  xLcdCtrlX++;
}

// -----------------------------------------------------------------------------
void
vLcdCtrlPutCmd (char cCmd) {

}

// -----------------------------------------------------------------------------
void
vLcdCtrlClear (void) {

  xLcdCtrlY = xLcdCtrlX = 0;
}

// -----------------------------------------------------------------------------
void 
vLcdCtrlSetX (xLcdCoord xX) {

  xLcdCtrlX = xX;
}

// -----------------------------------------------------------------------------
void 
vLcdCtrlSetY (xLcdCoord xY) {

  xLcdCtrlY = xY;
}

// -----------------------------------------------------------------------------
xLcdCoord 
xLcdCtrlGetX (void) {

  return xLcdCtrlX;
}

// -----------------------------------------------------------------------------
xLcdCoord 
xLcdCtrlGetY (void) {

  return xLcdCtrlY;
}

// -----------------------------------------------------------------------------
void
vLcdCtrlEnableCursor (eLcdCursor eCursor) {

}

#if defined(LCD_CTRL_ICON_ENABLE)
#include <avr/pgmspace.h>
// -----------------------------------------------------------------------------
void
vLcdCtrlSetIcon (uint8_t ucIndex, const uint8_t * pucIconPattern) {

}
#endif /* LCD_CTRL_ICON_ENABLE */

/* ========================================================================== */
#endif  /* LCD_CTRL == LCD_CTRL_TEMPLATE */
#endif /* AVRIO_LCD_ENABLE defined */
