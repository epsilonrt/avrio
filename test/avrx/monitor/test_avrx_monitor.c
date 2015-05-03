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
 * @file test_avrx_monitor.c
 *
 * Revision History ------------------------------------------------------------
 *    20110907 - Initial version by epsilonRT
 * ---
 * @brief Test du moniteur pour le debug
 */
#include <avrio/led.h>
#include <avrio/avrx.h>
#include <avrio/serial.h>
#include <avrx/debug.h>

/* constants ================================================================ */
#define BAUDRATE 9600

/* private functions ======================================================== */
static void
prvvFillRamBlock (uint8_t * pucBlock, size_t xSize) {
  uint8_t ucValue = 0;
  
  while (xSize--) {
  
    *pucBlock++ = ucValue++;
  }
}

/* internal public functions ================================================ */
AVRX_TASK (task1, 20, 2) {
  
  for (;;) {
    
    vDebugMonitor ();
    vLedToggle (LED_LED1);
  }
}

// -----------------------------------------------------------------------------
int
main (void){

  vLedInit();
  vDebugInit (SERIAL_BAUD_X1(BAUDRATE / 100L));
  prvvFillRamBlock ((uint8_t *)0x200, 0x100);

  vAvrXRunTask (task1);
  vAvrXStart ();
}

/* ========================================================================== */
