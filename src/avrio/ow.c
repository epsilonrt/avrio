/**
 * Copyright © 2017 epsilonRT. All rights reserved.
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
 * @file 
 * @brief
 */
#include "ow.h"
#include "ow-driver/common.h"
#include <string.h>

/* internal public functions ================================================ */

// -----------------------------------------------------------------------------
int8_t 
iOwSearchDevices (xOwDevice * devices, uint8_t len) {
  uint8_t i;
  uint8_t presence;
  uint8_t * newID;
  uint8_t * currentID;
  uint8_t lastDeviation = 0;
  uint8_t numDevices = 0;

  // Initialize all addresses as zero, on bus 0 (does not exist).
  // Do a search on the bus to discover all addresses.
  for (i = 0; i < len; i++) {

    memset (&devices[i], 0, sizeof (xOwDevice));
  }

  // Find the buses with slave devices.
  presence = bOwReset ();

  newID = devices[0].id;
  currentID = newID;

  if (presence) { // Devices available
    // Do slave search on each bus, and place identifiers and corresponding
    // bus "addresses" in the array.
    do {
      memcpy (newID, currentID, 8);
      bOwReset ();
      lastDeviation = ucOwSearchRom (newID, lastDeviation);
      currentID = newID;
      numDevices++;
      newID = devices[numDevices].id;
    }
    while (lastDeviation != 0);
  }

  // Go through all the devices and do CRC check.
  for (i = 0; i < numDevices; i++) {
    
    // If any id has a crc error, return error.
    if (iOwCheckCRC8 (devices[i].id, 7) != 0) {
      
      return -1;
    }
  }
  return numDevices;
}

//------------------------------------------------------------------------------
void 
vOwSkip (void) {
  
  // Send the SKIP ROM command on the bus.
  vOwWrite (OW_ROM_SKIP);
}

//------------------------------------------------------------------------------
uint8_t 
ucOwComputeCRC8 (uint8_t inData, uint8_t seed) {
  uint8_t bitsLeft;
  uint8_t temp;

  for (bitsLeft = 8; bitsLeft > 0; bitsLeft--) {
    temp = ( (seed ^ inData) & 0x01);
    if (temp == 0) {
      seed >>= 1;
    }
    else {
      seed ^= 0x18;
      seed >>= 1;
      seed |= 0x80;
    }
    inData >>= 1;
  }
  return seed;
}

//------------------------------------------------------------------------------
int8_t 
iOwCheckCRC8 (uint8_t * buffer, uint8_t len) {
  uint8_t i;
  uint8_t crc8 = 0;

  for (i = 0; i < len; i++) {
    crc8 = ucOwComputeCRC8 (*buffer, crc8);
    buffer++;
  }
  if (crc8 == (*buffer)) {
    return 0;
  }
  return -1;
}

#include "ow-driver/polled.c"
#include "ow-driver/polled-pio.c"
#include "ow-driver/polled-uart.c"
/* ========================================================================== */
