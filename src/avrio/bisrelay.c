/**
 * @file bisrelay.h
 * @brief Relais bistables (Implémentation)
 * @author Copyright © 2014 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
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
