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
 * @file demo_spi.c
 * @brief Exemple accès carte MMC/SD
 *
 * Ce test écrit écrit un octet dans un registre et relit cet octet en
 * vérifiant que la valeur lue est identique à celle écrite. \n
 * Si cela fonctionne, la LED1 clignote régulièrement, sinon elle flashe
 * rapidement.
 * Copyright © 2011-2014 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * Revision History ------------------------------------------------------------
 *    20140306 - Initial version by epsilonRT
 */
#include <avrio/led.h>
#include <avrio/spi.h>
#include <avrio/delay.h>

/* constants ================================================================ */
#define TEST_BYTE 0x55

/* private functions ======================================================== */

// ------------------------------------------------------------------------------
static void
vAssert (bool test) {

  if (test == false) {

    for (;;) {

      vLedSet (LED_LED1);
      delay_ms (50);
      vLedClear (LED_LED1);
      delay_ms (150);
    }
  }
}

/* main ===================================================================== */
int
main (void) {
  uint8_t ucByte;

  vLedInit();
  vSpiSetSsAsOutput();
  vSpiMasterInit (SPI_DIV2);

  for (;;) {

    vSpiSetSs();
    vSpiMasterWrite (TEST_BYTE);
    ucByte = ucSpiMasterRead();
    vSpiClearSs();

    vAssert (ucByte == TEST_BYTE);
    vLedToggle (LED_LED1);
    delay_ms (500);
  }

  return 0;
}

/* ========================================================================== */
