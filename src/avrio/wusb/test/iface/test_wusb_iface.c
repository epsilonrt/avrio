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
 * @file test_net_wusb_iface.c
 * @brief Test unitaire Interface module WirelessUSB
 *
 * Revision History ------------------------------------------------------------
 *    20111002 - Initial version by epsilonRT
 */
#include <avrio/wusb/net/device/iface.h>
#include <avrio/led.h>
#include <avrio/delay.h>
#include <avr/pgmspace.h>

/* private variables ======================================================== */
static uint8_t ucMid[4];
static uint8_t ucPnCode[8];
static const uint8_t ucPnCode_P[] PROGMEM = {
  // LSB MSB
  0xD3, 0x64, 0x44, 0x7A, 0x2D, 0xA8, 0xF7, 0x83, // PN Code ID 0 - Index 0
};
static const uint8_t ucPnCodeDef_P[] PROGMEM = {
  // LSB MSB
  0x22, 0xB2, 0xE9, 0xE0, 0x3D, 0x6A, 0x8B, 0x1E, // Defaut
};

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

// -----------------------------------------------------------------------------
static void
GetMid (uint8_t * mid) {
  uint8_t reg;

  reg = ucWIfcRead (REG_ANALOG_CTL);
  vWIfcWrite (REG_ANALOG_CTL, reg | bMIDREAD_ENABLE);
  vWIfcBurstRead (REG_MID, mid, 4);
  vWIfcWrite (REG_ANALOG_CTL, reg);
}

// -----------------------------------------------------------------------------
static void
SetPnCode_P (const uint8_t * ucPnCode) {

  vWIfcBurstWrite_P (REG_PN_CODE, ucPnCode, 8);
}

// -----------------------------------------------------------------------------
static void
SetPnCode (const uint8_t * ucPnCode) {

  vWIfcBurstWrite (REG_PN_CODE, ucPnCode, 8);
}

// -----------------------------------------------------------------------------
static void
GetPnCode (uint8_t * ucPnCode) {

  vWIfcBurstRead (REG_PN_CODE, ucPnCode, 8);
}

// -----------------------------------------------------------------------------
void
vWDevIsr (void) {

  for(;;) {
    
    vAssert(0);
  }
}

/* internal public functions ================================================ */
int
main (void) {

  vLedInit();
  for (;;) {

    vWIfcInit ();
    GetMid (ucMid);
    GetPnCode (ucPnCode);
    vAssert (memcmp_P (ucPnCode, ucPnCodeDef_P, sizeof(ucPnCodeDef_P)) == 0);
    SetPnCode_P (ucPnCode_P);
    GetPnCode (ucPnCode);
    vAssert (memcmp_P (ucPnCode, ucPnCode_P, sizeof(ucPnCode_P)) == 0);
    SetPnCode (ucPnCode);
    vLedToggle (LED_LED1);
    delay_ms (500);
  }
  return 0;
}
/* ========================================================================== */
