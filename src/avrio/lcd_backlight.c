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
 * @file lcd_backlight.c
 * @brief Rétroéclairage d'afficheur LCD
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
