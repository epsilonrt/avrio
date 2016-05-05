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
