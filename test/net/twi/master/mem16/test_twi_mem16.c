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
 * @file test_twi_mem16.c
 * @brief Test unitaire mémoire I2C.
 *

   ========================================================================== */
#include <avrio/twi.h>
#include <avrio/led.h>
#include <avrio/delay.h>
#include <string.h>

/* ========================================================================== */

/* constants ================================================================ */

// Caractéristiques de la trame envoyée
#define BUFFER_SIZE  16

// Caractéristiques du circuit esclave
#define SLAVE_ADDR  0xA0
#define MEM_BASE    0x0000
#define MEM_SIZE    8192
#define MEM_END     (MEM_BASE + MEM_SIZE - 1)

/* private variables ======================================================== */
TWIFRAME_STATIC_DECLARE (xTxFrame, BUFFER_SIZE);
TWIFRAME_STATIC_DECLARE (xRxFrame, BUFFER_SIZE);

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
  uint16_t usMemAddr;
  uint8_t ucCount;
  uint8_t * pucData = xTxFrame.pxData;

  vTwiInit ();
  vLedInit ();

  vAssert(eTwiSetSpeed (400) == TWI_SUCCESS);

  for (ucCount = 1; ucCount <= BUFFER_SIZE; ucCount++) {
  
    *pucData++ = ucCount;
  }
  xTxFrame.xLen  = BUFFER_SIZE;
  xTxFrame.xAddr = SLAVE_ADDR;
  xRxFrame.xLen  = BUFFER_SIZE;
  xRxFrame.xAddr = SLAVE_ADDR;


  for (;;) {

    for (usMemAddr = MEM_BASE; usMemAddr <= MEM_END; usMemAddr += BUFFER_SIZE) {

      // Ecriture en mémoire
      vAssert(eTwiMem16Write (usMemAddr, &xTxFrame) == TWI_SUCCESS);

      // Lecture en mémoire
      vAssert(eTwiMem16Read  (usMemAddr, &xRxFrame) == TWI_SUCCESS);
      
      // Vérif
      vAssert(memcmp (xTxFrame.pxData, xRxFrame.pxData, BUFFER_SIZE) == 0);
      vLedToggle (LED_LED1);
      delay_ms (500);
     }

  }
  return 0;
}

/** @} */
/* ========================================================================== */
