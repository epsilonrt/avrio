/**
 * @file ntc.c
 * @brief NTC thermistor (Implementation)
 *
 * Copyright © 2015 epsilonRT. All rights reserved.
 * Copyright © 2007, 2013 - SoftQuadrat GmbH, Germany
 * Contact: thermistor (at) softquadrat.de
 * Web site: thermistor.sourceforge.net
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
 */
#include <math.h>
#include "ntc.h"

/* constants ================================================================ */
#define TABS (-273.15)

/* private functions ======================================================== */
/*
 * Evaluates p(x) for a polynom p.
 * Calculates the value of polynom p at x accordings to
 * Horners schema.
 * @param p polynom.
 * @param x value to be inserted into the polynom.
 * @return calculated polynom value.
 */
static double
poly(double x, int degree, double p[]) {
  double retval = 0.0;
  int i;

  for (i = degree; i >= 0; i--) {

    retval = retval * x + p[i];
  }
  return retval;
}

/* internal public functions ================================================ */
// -----------------------------------------------------------------------------
double
dNtcTempToRes (double dT, double dCoeff[]) {
  double r;
  double u, v, p, q, b, c, d;

  dT = dT - TABS;
  d = (dCoeff[0] - 1.0 / dT) / dCoeff[3];
  c = dCoeff[1] / dCoeff[3];
  b = dCoeff[2] / dCoeff[3];
  q = 2.0 / 27.0 * b * b * b - 1.0 / 3.0 * b * c + d;
  p = c - 1.0 / 3.0 * b * b;
  v = - pow(q / 2.0 + sqrt(q * q / 4.0 + p * p * p / 27.0), 1.0 / 3.0);
  u =   pow(-q / 2.0 + sqrt(q * q / 4.0 + p * p * p / 27.0), 1.0 / 3.0);
  r  = exp(u + v - b / 3.0);
  return r;
}

// -----------------------------------------------------------------------------
double
dNtcResToTemp(double dR, double dCoeff[])
{
  double ti;

  ti = poly(log(dR), 3, dCoeff);
  ti = 1.0 / ti + TABS;
  return ti;
}

/* ========================================================================== */
