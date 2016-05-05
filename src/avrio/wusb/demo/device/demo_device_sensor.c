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
