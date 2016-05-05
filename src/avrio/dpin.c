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
