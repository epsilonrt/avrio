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
#include "ow-ds2890.h"
#warning This source code has not been verified

/* constants ================================================================ */
#define DS2890_WRITE_CONTROL_REGISTER   0X55
#define DS2890_RELEASE_CODE             0x96
#define DS2890_WRITE_POSITION           0x0f

/* structures =============================================================== */
/* types ==================================================================== */
/* private variables ======================================================== */
/* private functions ======================================================== */
/* public variables ========================================================= */
/* internal public functions ================================================ */

//------------------------------------------------------------------------------
int 
iDs2890SetPosition (xOwDevice * device, uint8_t position) {
  
  // Reset, presence.
  if (!bOwReset ()) {
    
    return -1;
  }
  
  //Match id.
   vOwMatch (device);

  // Send Write control register command.
  vOwWrite (DS2890_WRITE_CONTROL_REGISTER);

  // Write 0x4c to control register to enable charge pump.
  vOwWrite (0x4c);

  // Check that the value returned matches the value sent.
  if (ucOwRead () != 0x4c) {
    
    return -1;
  }

  // Send release code to update control register.
  vOwWrite (DS2890_RELEASE_CODE);
  

  // Check that zeros are returned to ensure that the operation was
  // successful.
  if (ucOwRead () == 0xff) {
    
    return;
  }

  // Reset, presence.
  if (!bOwReset ()) {
    
    return;
  }

  // Match id.
   vOwMatch (device);

  // Send the Write Position command.
  vOwWrite (DS2890_WRITE_POSITION);

  // Send the new position.
  vOwWrite (position);

  // Check that the value returned matches the value sent.
  if (ucOwRead () != position) {
    return;
  }

  // Send release code to update wiper position.
  vOwWrite (DS2890_RELEASE_CODE);

  // Check that zeros are returned to ensure that the operation was
  // successful.
  if (ucOwRead () == 0xff) {
    return;
  }
}
/* ========================================================================== */
