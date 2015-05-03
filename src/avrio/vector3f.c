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
 * @file vector3f.c
 * @brief Vecteur tridimensionnel (Implémentation)
 *
 * Revision History ------------------------------------------------------------
 *    20121218 - Initial version by epsilonRT
 */
#include <math.h>
#include "vector3f.h"

/* constants ================================================================ */
#define ORDER 3

/* internal public functions ================================================ */

// -----------------------------------------------------------------------------
// !!!!  Fonctions NON itératives - doivent être modifiées si ORDER modifié !!!!
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
void 
vVector3fCross (  float * xOut ,
                  const float * xIn1, 
                  const float * xIn2 ) {

  xOut[0] = (xIn1[1] * xIn2[2]) - (xIn1[2] * xIn2[1]);
  xOut[1] = (xIn1[2] * xIn2[0]) - (xIn1[0] * xIn2[2]);
  xOut[2] = (xIn1[0] * xIn2[1]) - (xIn1[1] * xIn2[0]);
}

// -----------------------------------------------------------------------------
// !!!!                    Fonctions itératives                             !!!!
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
float 
fVector3fDot (const float * xIn1, const float * xIn2) {
  float fDot = 0;
  
  for (uint8_t i = 0; i < ORDER; i++) {
  
    fDot += xIn1[i] * xIn2[i];
  }
  return fDot;
}

// -----------------------------------------------------------------------------
void 
vVector3fClear (float * xIn1) {

  for (uint8_t i = 0; i < ORDER; i++) {
  
    xIn1[i] = 0;
  }
}

// -----------------------------------------------------------------------------
bool 
bVector3fIsCleared (const float * xIn1) {

  for (uint8_t i = 0; i < ORDER; i++) {
  
    if (xIn1[i])
      return false;
  }
  return true;
}

// -----------------------------------------------------------------------------
bool 
bVector3fEq (const float * xIn1, const float * xIn2) {

  for (uint8_t i = 0; i < ORDER; i++) {
  
    if (xIn1[i] != xIn2[i])
      return false;
  }
  return true;
}

// -----------------------------------------------------------------------------
void 
vVector3fCopy (float * xDst, const float * xSrc) {

  for (uint8_t i = 0; i < ORDER; i++) {
  
    xDst[i] = xSrc[i];
  }
}

// -----------------------------------------------------------------------------
void 
vVector3fAdd   (  float * xOut ,
                  const float * xIn1, 
                  const float * xIn2 ) {

  for (uint8_t i = 0; i < ORDER; i++) {
  
    xOut[i] = xIn1[i] + xIn2[i];
  }
}

// -----------------------------------------------------------------------------
void 
vVector3fSub   (  float * xOut ,
                  const float * xIn1, 
                  const float * xIn2 ) {

  for (uint8_t i = 0; i < ORDER; i++) {
  
    xOut[i] = xIn1[i] - xIn2[i];
  }
}

// -----------------------------------------------------------------------------
void 
vVector3fScale (  float * xOut ,
                  const float * xIn1, 
                  float fConst ) {

  for (uint8_t i = 0; i < ORDER; i++) {
  
    xOut[i] = xIn1[i] * fConst;
  }
}

// -----------------------------------------------------------------------------
float 
fVector3fAbs (const float * xIn1) {

  return sqrt (fVector3fDot (xIn1, xIn1));
}

// -----------------------------------------------------------------------------
void 
vVector3fNormalize (float * xIn1) {

  vVector3fDivK (xIn1, fVector3fAbs (xIn1));
}

// -----------------------------------------------------------------------------
void 
vVector3fMulK (float * xVect, float fConst) {

  vVector3fScale (xVect, xVect, fConst);
}

// -----------------------------------------------------------------------------
void 
vVector3fDivK (float * xVect, float fConst) {

  vVector3fScale (xVect, xVect, 1.0 / fConst);
}

/* ========================================================================== */
