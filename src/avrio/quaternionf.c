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
