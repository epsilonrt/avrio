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
 * @file test_xmem.c
 * @brief Test unitaire interface mémoire externe
 *
 * Ce test écrit une suite d'octets de valeurs croissantes (0x00 à 0xFF) dans
 * la mémoire RAM externe, puis relit ces octets en vérifiant que
 * les données lues correpondent à celles écrites.
 * Si cela fonctionne, la LED1 clignote régulièrement, sinon elle flashe
 * rapidement.
 *
 * Revision History ------------------------------------------------------------
 *    20120323 - Initial version by epsilonRT
 */
#include <avrio/led.h>
#include <avrio/xmem.h>
#include <avrio/delay.h>
#include <string.h>

/* constants ================================================================ */
#define TEST_PATTERN (uint8_t)0x55

/* private variables ======================================================== */
static volatile uint8_t * pucBuffer;
static volatile uint8_t ucReadByte;
static volatile uint8_t ucWriteByte;

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
  
  vLedInit();
  ucWriteByte = TEST_PATTERN;

  for (;;) {
    
    // Test 1 - Ecriture/lecture dans la mémoire externe
    pucBuffer = (volatile uint8_t *) XMEM_RAM_BASE;
    while (pucBuffer <= (volatile uint8_t *) XMEM_RAM_END) {
    
      *pucBuffer = ucWriteByte;
      ucReadByte = *pucBuffer;
      vAssert (ucReadByte == ucWriteByte);

      ucWriteByte ^= 0xFF; // Inverse chaque bit

      *pucBuffer = ucWriteByte;
      ucReadByte = *pucBuffer;
      vAssert (ucReadByte == ucWriteByte);

      *pucBuffer++;
    }
    
    vLedToggle (LED_LED1);
    delay_ms (500);
  }
  
  return 0;
}

/* ========================================================================== */
