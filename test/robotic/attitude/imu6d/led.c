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
 * @file led.c
 * @brief LED (diodes électroluminescentes)
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
