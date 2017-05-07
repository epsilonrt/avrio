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
#ifdef OW_DRIVER_POLLED_PIO

#include <util/atomic.h>
#include <util/delay.h>
#include "polled.h"
#include "avrio-board-ow.h"

/* internal public functions ================================================ */

//------------------------------------------------------------------------------
void
vOwInit (void) {

  vOwBusRelease();
  // The first rising edge can be interpreted by a slave as the end of a
  // Reset pulse. Delay for the required reset recovery time (H) to be
  // sure that the real reset is interpreted correctly.
  _delay_ms (OWS_DELAY_H_STD_MODE / 1000.);
}

//------------------------------------------------------------------------------
void
vOwBitWrite1 (void) {

  ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {
    // Drive bus low and delay.
    vOwBusPullLow();
    _delay_us (OWS_DELAY_A_STD_MODE);

    // Release bus and delay.
    vOwBusRelease();
    _delay_us (OWS_DELAY_B_STD_MODE);
  }
}

//------------------------------------------------------------------------------
void
vOwBitWrite0 (void) {

  ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {

    // Drive bus low and delay.
    vOwBusPullLow();
    _delay_us (OWS_DELAY_C_STD_MODE);

    // Release bus and delay.
    vOwBusRelease();
    _delay_us (OWS_DELAY_D_STD_MODE);
  }
}

//------------------------------------------------------------------------------
bool
bOwBitRead (void) {
  bool bitsRead;

  ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {

    // Drive bus low and delay.
    vOwBusPullLow();
    _delay_us (OWS_DELAY_A_STD_MODE);

    // Release bus and delay.
    vOwBusRelease();
    _delay_us (OWS_DELAY_E_STD_MODE);

    // Sample bus and delay.
    bitsRead = bOwBusRead();
    _delay_us (OWS_DELAY_F_STD_MODE);
  }

  return bitsRead;
}

//------------------------------------------------------------------------------
bool
bOwReset (void) {
  bool presenceDetected;

  ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {

    // Drive bus low and delay.
    vOwBusPullLow();
    _delay_ms (OWS_DELAY_H_STD_MODE / 1000.);

    // Release bus and delay.
    vOwBusRelease();
    _delay_us (OWS_DELAY_I_STD_MODE);

    // Sample bus to detect presence signal and delay.
    presenceDetected = ! bOwBusRead();
    _delay_ms (OWS_DELAY_J_STD_MODE / 1000.);
  }

  return presenceDetected;
}

/* ========================================================================== */
#endif /* OW_DRIVER_POLLED_PIO defined */
