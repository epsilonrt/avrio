/**
 * @file tsl230.c
 * @brief Capteur de luminosité TSL230 (Implémentation)
 *
  * Copyright © 2015 Pascal JEAN aka epsilonRT. All rights reserved.
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
 */
#include "avrio-config.h"

#if defined(AVRIO_TSL230_ENABLE)
/* ========================================================================== */
#include "tsl230.h"

// Responsivity @ Sensitivity = eTsl230Sensitivity100 & Scale = eTsl230Scale100
#define TSL230_RE 790.

/* public variables ========================================================= */
xTsl230Context xTsl230;

#ifndef TSL230_INT

/* private functions ======================================================== */
// -----------------------------------------------------------------------------
static void
vTimerInit (void) {

  vTsl230CounterInit();
}

// -----------------------------------------------------------------------------
static void
vTimerClear (void) {

  vTsl230CounterClear();
}

// -----------------------------------------------------------------------------
static void
vTimerEnable (bool En) {

  vTsl230CounterEnable (En);
}

// -----------------------------------------------------------------------------
static uint16_t
usTimerRead (void) {

  return usTsl230CounterRead();
}

/* public variables ========================================================= */
xCounterOps xTsl230CounterOps = {
  .init = vTimerInit,
  .clear = vTimerClear,
  .enable = vTimerEnable,
  .read = usTimerRead
};
#endif

/* private functions ======================================================== */

/* internal public functions ================================================ */

// -----------------------------------------------------------------------------
void
vTsl230Init (void) {

  vTsl230PinInit();
  vTsl230SetSensitivity (TSL230_DEFAULT_SENSITIVITY);
  vTsl230SetScale (TSL230_DEFAULT_SCALE);

#ifdef TSL230_INT
  vICounterInit (&xTsl230.xCounter, TSL230_INT);
  vICounterSetWindow (&xTsl230.xCounter, TSL230_DEFAULT_WINDOW);
#else
  vCounterInit (&xTsl230.xCounter, &xTsl230CounterOps);
  vCounterSetWindow (&xTsl230.xCounter, TSL230_DEFAULT_WINDOW);
#endif

  xTsl230.dDarkFreq = TSL230_DEFAULT_DARK_FREQ;
  xTsl230.dResponsivity = TSL230_DEFAULT_RESPONSITIVITY;
  vTsl230Enable();
}

// -----------------------------------------------------------------------------
double
dTsl230FreqToIrradiance (double dFreq) {
  double dRe = TSL230_RE;

  switch (xTsl230.eSensitivity) {
    case eTsl230Sensitivity1:
      dRe /= 100;
      break;
    case eTsl230Sensitivity10:
      dRe /= 10;
      break;
    case eTsl230Sensitivity100:
      break;
    default :
      return -1.;
  }

  switch (xTsl230.eScale) {
    case eTsl230Scale1:
      dRe /= 100;
      break;
    case eTsl230Scale10:
      dRe /= 10;
      break;
    case eTsl230Scale50:
      dRe /= 2;
      break;
    case eTsl230Scale100:
      break;
    default :
      return -1.;
  }
  return (dFreq - xTsl230.dDarkFreq) / dRe;
}

// -----------------------------------------------------------------------------
void
vTsl230SetRange (uint16_t usRange) {
  eTsl230Sensitivity n = -1;
  eTsl230Sensitivity p = eTsl230GetSensitivity();

  switch (usRange) {

    case 0:
      n = eTsl230Off;
      break;
    case 20:
      n = eTsl230Sensitivity100;
      break;
    case 200:
      n = eTsl230Sensitivity10;
      break;
    case 2000:
      n = eTsl230Sensitivity1;
      break;
    default:
      break;
  }
  
  if ( (n >= 0) && (n != p)) {

    vTsl230SetSensitivity (n);
    if ( (p == eTsl230Off) && (n != eTsl230Off)) {
      // Délai de démarrage
      delay_us (110);
    }
  }
}

// -----------------------------------------------------------------------------
uint16_t
usTsl230Range (void) {

  switch (eTsl230GetSensitivity()) {

    case eTsl230Sensitivity100:
      return 20;
    case eTsl230Sensitivity10:
      return 200;
    case eTsl230Sensitivity1:
      return 2000;
    default:
      break;
  }
  return 0;
}

// -----------------------------------------------------------------------------
uint16_t
usTsl230AutoRange (void) {
  uint16_t usRange = 2000;
  double im;

  for (;;) {

    vTsl230SetRange (usRange);
    vTsl230Start();
    vTsl230WaitForComplete();
    im = dTsl230Irradiance() / 100.;
    if ( (im < ( (double) usRange * 0.09)) && (usRange > 20)) {

      usRange /= 10;
    }
    else {

      break;
    }
  }
  return usRange;
}

// -----------------------------------------------------------------------------
double
dTsl230ReadFreq (bool bAutoRange) {

  if (bAutoRange) {

    (void) usTsl230AutoRange();
  }
  vTsl230Start();
  vTsl230WaitForComplete();
  return dTsl230Freq();
}

#endif /* AVRIO_TSL230_ENABLE defined */
/* ========================================================================== */
