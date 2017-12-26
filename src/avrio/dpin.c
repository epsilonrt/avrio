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
 * @file dpin.c
 * @brief Broche numérique
 */
#include <avrio/dpin.h>

/* macros =================================================================== */
#define DP_PORT(p) (p->port)
#define DP_PIN(p)  (p->port - 2)
#define DP_DDR(p)  (p->port - 1)

#if defined(__AVR_ATtiny20__ ) || defined(__AVR_ATtiny40__) || defined(__AVR_ATtiny1634__)
#define DPIN_PUE 1
#endif

#ifdef DPIN_PUE
// Registre PUE pour valider les résistances de pull-up
#ifdef DPIN_PUE_FIELD
// Cas particulier du registre PUE non collé au registre PORT
#define DP_PUE(p)  (p->pue)
#else
// Registre PUE collé au registre PORT (le plus courant)
#define DP_PUE(p)  (p->port + 1)
#endif

#define DP_SETPULLUP(p,mode) vDpWriteBit(DP_PUE(p),p->pin,((mode)==eModeInputPullUp))
#define DP_IS_PORTBIT_SET(mode) ((mode)==eModeOutputHigh)

#else
// Pas de registre PUE, cas le plus courant
#define DP_SETPULLUP(p,mode)
#define DP_IS_PORTBIT_SET(mode) (((mode)&2)!=0)
#endif


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
static void
vDpToggleBit (volatile uint8_t * r, uint8_t ucPin) {

  *r ^= _BV (ucPin);
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

    DP_SETPULLUP (p,eMode);
    vDpWriteBit (DP_PORT (p), p->pin, DP_IS_PORTBIT_SET(eMode));
    vDpWriteBit (DP_DDR (p), p->pin, eMode & 1);
    p->mode = eMode;
  }
}

// -----------------------------------------------------------------------------
void
vDpWrite (xDPin * p, bool bValue) {

  if (p->port) {

    vDpWriteBit (DP_PORT (p), p->pin, (p->mode == eModeOutputHigh) ? ! bValue : bValue);
  }
}

// -----------------------------------------------------------------------------
void
vDpToggle (xDPin * p) {

  if (p->port) {

    vDpToggleBit (DP_PORT (p), p->pin);
  }
}

// -----------------------------------------------------------------------------
bool bDpRead (const xDPin * p) {

  if (p->port) {
    bool bValue = bDpReadBit (DP_PIN (p), p->pin);

    return (p->mode & 2) ? ! bValue : bValue;
  }
  return false;
}

/* ========================================================================== */
