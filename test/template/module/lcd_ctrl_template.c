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
 * @file lcd_ctrl_template.c
 * @brief Modèle de Contrôleur LCD
 *
 *
 * Revision History ------------------------------------------------------------
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
