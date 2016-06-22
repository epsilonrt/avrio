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
 * @file gpio.c
 * @brief Port GPIO (implémentation)
 */
#include <avrio/gpio.h>

#ifdef AVRIO_GPIO_ENABLE
#include <avrio/delay.h>

/* public variables ========================================================= */
struct xGpioContext gpio = { .pin = GPIO_PINS };

/* private functions ======================================================== */

// -----------------------------------------------------------------------------
void __attribute__ ( (naked)) __attribute__ ( (section (".init7")))
vGpioInit (void) {

  for (uint8_t p = 0; p < GPIO_SIZE; p++) {

    vDpSetMode (&gpio.pin[p], gpio.pin[p].mode);
  }
}

/* internal public functions ================================================ */

// -----------------------------------------------------------------------------
void
vGpioPoll (void) {

  if ( (gpio.mask) && (gpio.callback)) {
    uGpioMask values = uGpioReadAll (gpio.mask);

    if (values != gpio.values) {
      // l'état des entrées a changé ?

      delay_ms (GPIO_DEBOUNCE_DELAY); // Anti-rebond

      if (values == uGpioReadAll (gpio.mask)) {

        // le changement est confirmé
        uGpioMask mask = 1;
        uGpioMask diff = values ^ gpio.values;
        int p = 0;

        // Recherche des bits modifiés et décodage des fronts
        while (diff) {

          if (diff & mask) {

            // Ce bit a changé
            eDpEdge edge;

            if (values & mask) {

              // Il est passé à 1
              edge = eEdgeRising;  // front montant
            }
            else {

              // Il est passé à 0
              edge = eEdgeFalling; // front descendant
            }
            diff &= ~mask;  // clear du bit traité

            if (gpio.pin[p].edge & edge) {

              // Le front est valide
              gpio.callback (values, p, edge, gpio.udata);
            }

          }
          mask <<= 1;
          p++;
        }

        gpio.values = values;
      }
    }
  }
}

// -----------------------------------------------------------------------------
void
vGpioSetPinEdge (uint8_t ucPin, eDpEdge eEdge) {

  if (ucPin < GPIO_SIZE) {

    gpio.pin[ucPin].edge = eEdge;
    if (eEdge == eEdgeNone) {

      gpio.mask &= ~_BV (ucPin);
    }
    else {

      gpio.mask |= _BV (ucPin);
    }
  }
}

// -----------------------------------------------------------------------------
void
vGpioWriteAll (uGpioMask uPinMask, bool bValue) {
  uGpioMask bit = 1;
  uint8_t p = 0;

  while ( (uPinMask) && (p < GPIO_SIZE)) {

    if (uPinMask & bit) {
      
      vGpioWrite (p, bValue);
      uPinMask &= ~bit; // bit modifié
    }
    
    bit <<= 1;
    p++;
  }
}

// -----------------------------------------------------------------------------
void
vGpioToggleAll (uGpioMask uPinMask) {
  uGpioMask bit = 1;
  uint8_t p = 0;

  while ( (uPinMask) && (p < GPIO_SIZE)) {

    if (uPinMask & bit) {
      
      vGpioToggle(p);
      uPinMask &= ~bit; // bit modifié
    }
    
    bit <<= 1;
    p++;
  }
}

// -----------------------------------------------------------------------------
uGpioMask
uGpioReadAll (uGpioMask uPinMask) {
  uGpioMask values = 0;
  uGpioMask bit = 1;
  uint8_t p = 0;

  while ( (uPinMask) && (p < GPIO_SIZE)) {

    if (uPinMask & bit) {
      
      if (bGpioRead (p)) {
        
        values |= bit;
      }
      uPinMask &= ~bit; // bit lu
    }
    
    bit <<= 1;
    p++;
  }

  return values;
}

/* ========================================================================== */
#endif /* AVRIO_GPIO_ENABLE defined */
