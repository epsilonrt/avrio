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
 * @file quaternionf.c
 * @brief Quatfernion (Implémentation)
 *
 * Revision History ------------------------------------------------------------
 *    20121218 - Initial version by epsilonRT
 */
#include <math.h>
#include "quaternionf.h"

/* internal public functions ================================================ */

// -----------------------------------------------------------------------------
double 
fQuatfNorm (const xQuatf * xQ) {

  return (xQ->a * xQ->a) + (xQ->b * xQ->b) + (xQ->c * xQ->c) + (xQ->d * xQ->d);
}

// -----------------------------------------------------------------------------
void 
vQuatfNormalize (xQuatf * xQ) {

  vQuatfDivK (xQ, fQuatfNorm(xQ));
}

// -----------------------------------------------------------------------------
bool 
bQuatfEq (const xQuatf * xQ1, const xQuatf *xQ2) {

  if (xQ1->a != xQ2->a)
    return false;
  if (xQ1->b != xQ2->b)
    return false;
  if (xQ1->c != xQ2->c)
    return false;
  if (xQ1->d != xQ2->d)
    return false;
  return true;
}

// -----------------------------------------------------------------------------
void 
vQuatfCopy (xQuatf * xDst, const xQuatf *xSrc) {

  xDst->a = xSrc->a;
  xDst->b = xSrc->b;
  xDst->c = xSrc->c;
  xDst->d = xSrc->d;
}

// -----------------------------------------------------------------------------
void 
vQuatfAdd   (  const xQuatf *xQ1, 
                  const xQuatf *xQ2, 
                  xQuatf *xQr  ) {

  xQr->a = xQ1->a + xQ2->a;
  xQr->b = xQ1->b + xQ2->b;
  xQr->c = xQ1->c + xQ2->c;
}

// -----------------------------------------------------------------------------
void 
vQuatfSub   (const xQuatf *xQ1, const xQuatf *xQ2, xQuatf *xQr  ) {

  xQr->a = xQ1->a - xQ2->a;
  xQr->b = xQ1->b - xQ2->b;
  xQr->c = xQ1->c - xQ2->c;
  xQr->d = xQ1->d - xQ2->d;
}

// -----------------------------------------------------------------------------
void 
vQuatfMulK (xQuatf * xQ, double fConst) {

  xQ->a *= fConst;
  xQ->b *= fConst;
  xQ->c *= fConst;
  xQ->d *= fConst;
}

// -----------------------------------------------------------------------------
void 
vQuatfDivK (xQuatf * xQ, double fConst) {

  xQ->a /= fConst;
  xQ->b /= fConst;
  xQ->c /= fConst;
  xQ->d /= fConst;
}

/* ========================================================================== */
