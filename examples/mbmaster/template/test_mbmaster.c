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
 * @file test_mbmaster.c
 * @brief Test de la couche port AVR de MBMaster
 * Copyright _ 2007 Christian Walter <wolti@sil.at> All rights reserved.
 *
 *
 * Revision History ------------------------------------------------------------
 *    20120604 - Initial version by epsilonRT from MBmaster 2.7.0
 */
#include <avr/interrupt.h>
#include <avrio/led.h>
#include "mbport.h"
#include "mbmporttest.h"

/* macros =================================================================== */
/* constants ================================================================ */
/* private variables ======================================================== */

/* private functions ======================================================== */

// -----------------------------------------------------------------------------
void
vMBPTestDebugLED ( UBYTE ubIdx, BOOL bTurnOn ) {
  uint8_t ucLed = xLedGetMask (ubIdx);

  if (bTurnOn) {
  
    vLedSet (ucLed);
  }
  else {
  
    vLedClear (ucLed);
  }
}

/* internal public functions ================================================ */
int
main (void) {

  vLedInit();
  sei();
  
  for (;;) {
  
    vMBPTestRun ();
  }

  return 0;
}
/* ========================================================================== */
