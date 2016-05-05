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
