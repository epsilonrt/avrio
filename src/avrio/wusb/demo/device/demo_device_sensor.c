/**
 * Copyright © 2011-2015 epsilonRT. All rights reserved.
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
 * @file demo_device_sensor.c
 * @brief Démonstration Couche Device WirelessUSB
 *
 * Revision History ------------------------------------------------------------
 *    20111003 - Initial version by epsilonRT
 */
#include <avrio/wusb/net/device/device.h>
#include <avrio/delay.h>
#include <avrio/led.h>
#include <stdlib.h>

/* constants ================================================================ */
#define PACKET_SIZE 40

/* internal private functions =============================================== */
// ------------------------------------------------------------------------------
void
vAssert (bool test) {

  if (test == false) {

     for (;;) {

      vLedSet (LED_LED1);
      delay_ms (50);
      vLedClear (LED_LED1);
      delay_ms (500);
    }
  }
}

/* private variables ======================================================== */
static uint16_t usIndex = 0;
static char pcIndex[7] = "T";
static uint8_t ucRxPacket[PACKET_SIZE];
static uint8_t ucRxValid[PACKET_SIZE];
static int iRxPacketSize;

/* main ===================================================================== */
int
main (void) {
  
  /* 
   * Init module WirelessUSB
   * Le configuration est celle par défaut définie dans wusb-config.h
   * Channel = 2 - PA Bias = 5
   */
  vWDevInit (WDEV_RATE_16KBPS);
  vWDevSetPaBias (7);
  sei(); /* Le module Device fonctionne sous interruption... */

  for (;;usIndex++) {

    utoa (usIndex, &pcIndex[1], 10);
    vWDevTransmit ((uint8_t *)pcIndex, strlen(pcIndex) + 1);
    
    iRxPacketSize = iWDevReceive (ucRxPacket, ucRxValid, PACKET_SIZE, 5000);
    vAssert  (iRxPacketSize >= 0);
    if (iRxPacketSize > 0) {
    
      vLedSet (LED_LED1);
    }
    delay_ms (100);
  }
}

/* ========================================================================== */
