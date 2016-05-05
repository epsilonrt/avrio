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
 * @file bisrelay.h
 * @brief Relais bistables (Implémentation)
 *
 * Revision History ------------------------------------------------------------
 *    2014 - Initial version by epsilonRT
 */
#include <avrio/delay.h>
#include "bisrelay.h"

/* public variables ========================================================= */
uint8_t ucBisRelayCurrent = BISRELAY_ALL;

/* private variables ======================================================== */
static const xBisRelay xBisRelays[BISRELAY_QUANTITY] PROGMEM = BISRELAY_ARRAY;

/* internal public functions ================================================ */

// -----------------------------------------------------------------------------
void
vBisRelayInit (void) {

  vBisRelayHwInit();
  vBisRelayClear (BISRELAY_ALL);
}

// -----------------------------------------------------------------------------
void
vBisRelayClear (uint8_t ucMask) {
  uint8_t ucBitChanged;

  ucBitChanged =  ~(ucBisRelayCurrent ^ ucMask) & ucMask;

  if (ucBitChanged) {
    uint8_t ucBit = 1;

    for (uint8_t i = 0; i < BISRELAY_QUANTITY; i++) {

      if (ucBitChanged & ucBit) {
        xBisRelay xRelay;

        memcpy_P (&xRelay, &xBisRelays[i], sizeof(xRelay));
        *xRelay.pPort |= xRelay.ucOff;
        delay_ms (BISRELAY_SET_TIME);
        *xRelay.pPort &= ~xRelay.ucOff;
      }
      ucBit <<= 1;
    }
    ucBisRelayCurrent &= ~ucMask;
  }
}

// -----------------------------------------------------------------------------
void
vBisRelaySet (uint8_t ucMask) {
  uint8_t ucBitChanged;

  ucBitChanged =  (ucBisRelayCurrent ^ ucMask) & ucMask;

  if (ucBitChanged) {
    uint8_t ucBit = 1;

    for (uint8_t i = 0; i < BISRELAY_QUANTITY; i++) {

      if (ucBitChanged & ucBit) {
        xBisRelay xRelay;

        memcpy_P (&xRelay, &xBisRelays[i], sizeof(xRelay));
        *xRelay.pPort |= xRelay.ucOn;
        delay_ms (BISRELAY_SET_TIME);
        *xRelay.pPort &= ~xRelay.ucOn;
      }
      ucBit <<= 1;
    }
    ucBisRelayCurrent |= ucMask;
  }
}

// -----------------------------------------------------------------------------
void
vBisRelayToggle (uint8_t ucMask) {

  if (ucMask) {
    uint8_t ucBit = 1;

    ucBisRelayCurrent ^=  ucMask;
    for (uint8_t i = 0; i < BISRELAY_QUANTITY; i++) {

      if (ucMask & ucBit) {
        xBisRelay xRelay;

        memcpy_P (&xRelay, &xBisRelays[i], sizeof(xRelay));
        if (ucBisRelayCurrent & ucBit) {

          *xRelay.pPort |= xRelay.ucOn;
          delay_ms (BISRELAY_SET_TIME);
          *xRelay.pPort &= ~xRelay.ucOn;
        }
        else {

          *xRelay.pPort |= xRelay.ucOff;
          delay_ms (BISRELAY_SET_TIME);
          *xRelay.pPort &= ~xRelay.ucOff;
        }
      }
      ucBit <<= 1;
    }
  }
}

// -----------------------------------------------------------------------------
void
vBisRelaySetAll (uint8_t ucMask) {

  vBisRelaySet (ucMask);
  vBisRelayClear (~ucMask);
}

/* ========================================================================== */
