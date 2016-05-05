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
