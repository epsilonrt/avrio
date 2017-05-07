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
#include "ow-ds1820.h"

/* constants ================================================================ */
#define DS1820_START_CONVERSION         0x44
#define DS1820_READ_SCRATCHPAD          0xbe

/* internal public functions ================================================ */
extern bool bOwBitRead(void);

// -----------------------------------------------------------------------------
int16_t 
iDs1820Read (xOwDevice * device) {
  uint8_t buf[9];
  
  // Reset, presence.
  if (bOwReset () == 0) {
    return DS1820_ERROR; // Error
  }
  // Match the id found earlier.
  vOwMatch (device);
  // Send start conversion command.
  vOwWrite (DS1820_START_CONVERSION);
  // Wait until conversion is finished.
  // Bus line is held low until conversion is finished.
  while (bOwBitRead () == false) {

  }
  // Reset, presence.
  if (bOwReset () == 0) {
    return DS1820_ERROR; // Error
  }
  // Match id again.
  vOwMatch (device);
  // Send READ SCRATCHPAD command.
  vOwWrite (DS1820_READ_SCRATCHPAD);

  for (uint8_t i = 0; i < 9; i++) {

    buf[i] = ucOwRead();
  }

  if (iOwCheckCRC8 (buf, 8) != 0) {
    
    return DS1820_ERROR; // Error
  }

  return buf[0] + (buf[1] << 8);
}

/* ========================================================================== */
