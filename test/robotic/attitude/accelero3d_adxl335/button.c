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
 * @file button.c
 * @brief Gestion des boutons poussoirs.
 */
#include "avrio-config.h"

#ifdef AVRIO_BUTTON_ENABLE
/* ========================================================================== */
#  include <avrio/button.h>
#  include <avrio/delay.h>

#  define BUTTON_COL (_BV(BUTTON_C1) | _BV(BUTTON_C2) | _BV(BUTTON_C3) | _BV(BUTTON_C4))
#  define BUTTON_ROW (_BV(BUTTON_R1) | _BV(BUTTON_R2) | _BV(BUTTON_R3) | _BV(BUTTON_R4))

/* public variables ========================================================= */
#  if defined(BUTTON_MASK_ARRAY_ENABLE)
  const xButMask 
    xButMaskArray [BUTTON_QUANTITY] = { 
      BUTTON_BUTTON1, 
      BUTTON_BUTTON2, 
      BUTTON_BUTTON3, 
      BUTTON_BUTTON4, 
      BUTTON_BUTTON5, 
      BUTTON_BUTTON6, 
      BUTTON_BUTTON7, 
      BUTTON_BUTTON8, 
      BUTTON_BUTTON9, 
      BUTTON_BUTTON10, 
      BUTTON_BUTTON11, 
      BUTTON_BUTTON12, 
      BUTTON_BUTTON13, 
      BUTTON_BUTTON14, 
      BUTTON_BUTTON15, 
      BUTTON_BUTTON16, 
    };
#  endif

/* private functions ======================================================== */
// -----------------------------------------------------------------------------
xButMask 
prvxReadButton (uint8_t ucRow) {
  uint8_t ucCol;
  
  BUTTON_PORT &= ~_BV(ucRow);
  delay_us(20);
  
  ucCol = ~BUTTON_PIN & BUTTON_COL;
  if (ucCol)
    delay_ms (20);
  ucCol = ~BUTTON_PIN & BUTTON_COL; 
  
  BUTTON_PORT |= BUTTON_ROW;

  return ((xButMask)ucCol) >> BUTTON_C1;
}

/* internal public functions ================================================ */

// -----------------------------------------------------------------------------
void
vButInit (void) {

  BUTTON_PORT  =  BUTTON_ROW | BUTTON_COL; /* Sorties à 1, Entrées avec pullup */
  BUTTON_DDR  |=  BUTTON_ROW;
  BUTTON_DDR  &= ~BUTTON_COL;
}

// -----------------------------------------------------------------------------
xButMask
xButGet (xButMask xMask) {
  xButMask xReturnMask;

  xReturnMask  = prvxReadButton (BUTTON_R1);
  xReturnMask |= prvxReadButton (BUTTON_R2) << 4;
  xReturnMask |= prvxReadButton (BUTTON_R3) << 8;
  xReturnMask |= prvxReadButton (BUTTON_R4) << 12;
 
  return xReturnMask & xMask;
}

#endif /* AVRIO_BUTTON_ENABLE defined */
/* ========================================================================== */
