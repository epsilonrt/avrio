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
 * @file thmi.c
 * @brief tiny HMI
 */
#include <avrio/keyb.h>
#include <avrio/lcd.h>
#include "avrio-config.h"

#if defined(AVRIO_KEYB_ENABLE) && defined(AVRIO_LCD_ENABLE)
#include "avrio-board-lcd.h"
#include "avrio-board-keyboard.h"

#if LCD_CTRL == LCD_CTRL_THMI && defined(KEYB_THMI_BUFFERSIZE)
#include <avrio/lcd/ctrl/lcd_ctrl_thmi.h>

/* internal public functions ================================================ */
// -----------------------------------------------------------------------------
uint16_t
usThmiVersion (void) {
  uint16_t usVersion = 0;
  
  if (usKeybHit() == 0) {
    char c;
    
    vLcdCtrlPutExtCmd (THMI_FVER);
    c = cKeybGetChar();
    usVersion  = c << 8; // Major
    c = cKeybGetChar();
    usVersion |= c; // Minor
  }
  
  return usVersion;
}
#endif /* LCD_CTRL == LCD_CTRL_THMI && defined(KEYB_THMI_BUFFERSIZE) */
#endif /* AVRIO_KEYB_ENABLE defined */
/* ========================================================================== */
