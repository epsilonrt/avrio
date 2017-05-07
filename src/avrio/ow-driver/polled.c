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
#if defined(OW_DRIVER_POLLED_PIO) || defined(OW_DRIVER_POLLED_UART)

//#include "ow.h"
#include "polled.h"
#include <string.h>

/* internal public functions ================================================ */
//------------------------------------------------------------------------------
uint8_t
ucOwSearchRom (uint8_t * bitPattern, uint8_t lastDeviation) {
  uint8_t currentBit = 1;
  uint8_t newDeviation = 0;
  uint8_t bitMask = 0x01;
  uint8_t bitA;
  uint8_t bitB;

  // Send SEARCH ROM command on the bus.
  vOwWrite (OW_ROM_SEARCH);

  // Walk through all 64 bits.
  while (currentBit <= 64) {
    // Read bit from bus twice.
    bitA = bOwBitRead ();
    bitB = bOwBitRead ();

    if (bitA && bitB) {
      // Both bits 1 (Error).
      newDeviation = -1;
      break;
    }
    else if (bitA ^ bitB) {
      // Bits A and B are different. All devices have the same bit here.
      // Set the bit in bitPattern to this value.
      if (bitA) {
        (*bitPattern) |= bitMask;
      }
      else {
        (*bitPattern) &= ~bitMask;
      }
    }
    else { // Both bits 0
      // If this is where a choice was made the last time,
      // a '1' bit is selected this time.
      if (currentBit == lastDeviation) {
        (*bitPattern) |= bitMask;
      }
      // For the rest of the id, '0' bits are selected when
      // discrepancies occur.
      else if (currentBit > lastDeviation) {
        (*bitPattern) &= ~bitMask;
        newDeviation = currentBit;
      }
      // If current bit in bit pattern = 0, then this is
      // out new deviation.
      else if (! (*bitPattern & bitMask)) {
        newDeviation = currentBit;
      }
      // IF the bit is already 1, do nothing.
      else {

      }
    }

    // Send the selected bit to the bus.
    if ( (*bitPattern) & bitMask) {
      vOwBitWrite1 ();
    }
    else {
      vOwBitWrite0 ();
    }

    // Increment current bit.
    currentBit++;

    // Adjust bitMask and bitPattern pointer.
    bitMask <<= 1;
    if (!bitMask) {
      bitMask = 0x01;
      bitPattern++;
    }
  }
  return newDeviation;
}

//------------------------------------------------------------------------------
void 
vOwWrite (uint8_t data) {
  uint8_t temp;
  uint8_t i;

  // Do once for each bit
  for (i = 0; i < 8; i++) {
    // Determine if lsb is '0' or '1' and transmit corresponding
    // waveform on the bus.
    temp = data & 0x01;
    if (temp) {
      vOwBitWrite1 ();
    }
    else {
      vOwBitWrite0 ();
    }
    // Right shift the data to get next bit.
    data >>= 1;
  }
}

//------------------------------------------------------------------------------
uint8_t 
ucOwRead (void) {
  uint8_t data;
  uint8_t i;

  // Clear the temporary input variable.
  data = 0x00;

  // Do once for each bit
  for (i = 0; i < 8; i++) {
    // Shift temporary input variable right.
    data >>= 1;
    // Set the msb if a '1' value is read from the bus.
    // Leave as it is ('0') else.
    if (bOwBitRead ()) {
      // Set msb
      data |= 0x80;
    }
  }
  return data;
}

//------------------------------------------------------------------------------
void 
vOwReadRomId (xOwDevice * device) {
  uint8_t bytesLeft = 8;
  uint8_t * id = device->id;

  // Send the READ ROM command on the bus.
  vOwWrite (OW_ROM_READ);

  // Do 8 times.
  while (bytesLeft > 0) {
    // Place the received data in memory.
    *id++ = ucOwRead ();
    bytesLeft--;
  }
}

//------------------------------------------------------------------------------
void 
vOwMatch (const xOwDevice * device) {
  uint8_t bytesLeft = 8;
  const uint8_t * id = device->id;

  // Send the MATCH ROM command.
  vOwWrite (OW_ROM_MATCH);

  // Do once for each byte.
  while (bytesLeft > 0) {
    // Transmit 1 byte of the ID to match.
    vOwWrite (*id++);
    bytesLeft--;
  }
}
#endif /* defined(OW_DRIVER_POLLED_PIO) || defined(OW_DRIVER_POLLED_UART) */
/* ========================================================================== */
