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
 * @file file.h
 * @brief Extension du système de fichiers de la libc
 */
#include <avrio/dpin.h>

/* macros =================================================================== */
#define DP_PORT(p) (p->port)
#define DP_PIN(p)  (p->port - 2)
#define DP_DDR(p)  (p->port - 1)

/* private functions ======================================================== */

// -----------------------------------------------------------------------------
static void
vDpWriteBit (volatile uint8_t * r, uint8_t ucPin, bool bValue) {

  uint8_t ucPort = *r;

  if (bValue) {

    ucPort |= _BV (ucPin);
  }
  else {

    ucPort &= ~_BV (ucPin);
  }
  *r = ucPort;
}

// -----------------------------------------------------------------------------
static bool
bDpReadBit (volatile uint8_t * r, uint8_t ucPin) {

  return (*r & _BV (ucPin)) != 0;
}

/* internal public functions ================================================ */

// -----------------------------------------------------------------------------
void
vDpSetMode (xDPin * p, eDpMode eMode) {

  if (p->port) {

    vDpWriteBit (DP_PORT (p), p->pin, (eMode & 2) != 0);
    vDpWriteBit (DP_DDR (p), p->pin, eMode & 1);
  }
}

// -----------------------------------------------------------------------------
void vDpWrite (xDPin * p, bool bValue) {

  if (p->port) {

    vDpWriteBit (DP_PORT (p), p->pin, bValue);
  }
}

// -----------------------------------------------------------------------------
bool bDpRead (const xDPin * p) {

  if (p->port) {

    return bDpReadBit (DP_PIN (p), p->pin);
  }
  return false;
}

/* ========================================================================== */
