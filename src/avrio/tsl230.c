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

/* private functions ======================================================== */

/* internal public functions ================================================ */

// -----------------------------------------------------------------------------
void
vTsl230Init (void) {

  vTsl230PinInit();
  vTsl230SetSensitivity (TSL230_DEFAULT_SENSITIVITY);
  vTsl230SetScale (TSL230_DEFAULT_SCALE);
  vICounterInit (&xTsl230.xCounter, TSL230_INT);
  vICounterSetWindow (&xTsl230.xCounter, 1000);
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
#endif /* AVRIO_TSL230_ENABLE defined */
/* ========================================================================== */
