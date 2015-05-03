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
 * @file test_twi_pcf8574.c
 * @ingroup test_group
 * @brief Tests unitaires du module \ref twi_module "Bus I2C".
 *

   ========================================================================== */
#include <avrio/twi.h>
#include <avrio/led.h>
#include <avrio/button.h>
#include <avrio/delay.h>
#include <string.h>

/* ========================================================================== */
/**
 * @addtogroup test_group
 * @{
 */

/* constants ================================================================ */
#define LED_ALIVE LED_LED1

// Caractéristiques du circuit esclave
#define SLAVE_ADDR 0x70
//#define SLAVE_ADDR 0x72

/* private functions ======================================================== */
static uint8_t ucWrite = 0x0F;
static uint8_t ucRead;

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
  eTwiStatus eError;

  vTwiInit ();
  vLedInit ();
  vButInit ();
  vLedSet (LED_ALIVE);

  eError = eTwiSetSpeed (100);
  vAssert (eError== TWI_SUCCESS);

  for (;;) {

    eError = eTwiWrite (SLAVE_ADDR, ucWrite);
    vAssert (eError == TWI_SUCCESS);

    eError = eTwiRead (SLAVE_ADDR, &ucRead);
    vAssert (eError == TWI_SUCCESS);
    vAssert (ucRead == ucWrite);

    while (xButGet (BUTTON_BUTTON1) == 0) ;
    ucWrite ^= 0xFF;
    vLedToggle (LED_ALIVE);
    delay_ms (150);
  }
  return 0;
}

/** @} */
/* ========================================================================== */
