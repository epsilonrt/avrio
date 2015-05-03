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
