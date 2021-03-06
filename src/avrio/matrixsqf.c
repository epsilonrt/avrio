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
 * @file matrixsqf.c
 * @brief Matrice carrée flottante (Implémentation)
 *
 * Revision History ------------------------------------------------------------
 *    20121218 - Initial version by epsilonRT
 */
#include <math.h>
#include "matrixsqf.h"

/* internal public functions ================================================ */

// -----------------------------------------------------------------------------
void 
vMatrixSqfMul (float * xOut, const float * xIn1, const float * xIn2, 
               uint8_t ucOrder) {
  
  for (uint8_t ucX = 0; ucX < ucOrder; ucX++) {
  
    for (uint8_t ucY = 0; ucY < ucOrder; ucY++) {
    
      xOut[ucX * ucOrder + ucY] = 0.0;
      
      for (uint8_t ucW = 0; ucW < 3; ucW++) {
      
        xOut[ucX * ucOrder + ucY] += xIn1[ucX * ucOrder + ucW] * xIn2[ucW * ucOrder + ucY];
      } 
    }
  }
}

// -----------------------------------------------------------------------------
void 
vMatrixSqfAdd (float * xOut, const float * xIn1, const float * xIn2, 
               uint8_t ucOrder) {

  for (uint8_t ucX = 0; ucX < ucOrder; ucX++) {
  
    for (uint8_t ucY = 0; ucY < ucOrder; ucY++) {
    
      xOut[ucX * ucOrder + ucY] = xIn1[ucX * ucOrder + ucY] + xIn2[ucX * ucOrder + ucY];
    }
  }
}

// -----------------------------------------------------------------------------
void 
vMatrixSqfSub (float * xOut, const float * xIn1, const float * xIn2, 
               uint8_t ucOrder) {

  for (uint8_t ucX = 0; ucX < ucOrder; ucX++) {
  
    for (uint8_t ucY = 0; ucY < ucOrder; ucY++) {
    
      xOut[ucX * ucOrder + ucY] = xIn1[ucX * ucOrder + ucY] - xIn2[ucX * ucOrder + ucY];
    }
  }
}

void 
vMatrixSqfScale (float * xOut, const float * xIn, float fConst, uint8_t ucOrder) {

  for (uint8_t ucX = 0; ucX < ucOrder; ucX++) {
  
    for (uint8_t ucY = 0; ucY < ucOrder; ucY++) {
    
      xOut[ucX * ucOrder + ucY] = xIn[ucX * ucOrder + ucY] * fConst;
    }
  }
}


// -----------------------------------------------------------------------------
void 
vMatrixSqfMulK (float * xMat, float fConst, uint8_t ucOrder) {

  vMatrixSqfScale (xMat, xMat, fConst, ucOrder);
}

// -----------------------------------------------------------------------------
void 
vMatrixSqfDivK (float * xMat, float fConst, uint8_t ucOrder) {

  vMatrixSqfScale (xMat, xMat, 1.0 / fConst, ucOrder);
}

// -----------------------------------------------------------------------------
void 
vMatrixSqfClear (float * xIn1, uint8_t ucOrder) {

  for (uint8_t ucX = 0; ucX < ucOrder; ucX++) {
  
    for (uint8_t ucY = 0; ucY < ucOrder; ucY++) {
    
      xIn1[ucX * ucOrder + ucY] = 0;
    }
  }
}

// -----------------------------------------------------------------------------
bool 
bMatrixSqfIsCleared (const float * xIn1, uint8_t ucOrder) {

  for (uint8_t ucX = 0; ucX < ucOrder; ucX++) {
  
    for (uint8_t ucY = 0; ucY < ucOrder; ucY++) {
    
      if (xIn1[ucX * ucOrder + ucY])
        return false;
    }
  }
  return true;
}

// -----------------------------------------------------------------------------
bool 
bMatrixSqfEq (const float * xIn1, const float * xIn2, uint8_t ucOrder) {

  for (uint8_t ucX = 0; ucX < ucOrder; ucX++) {
  
    for (uint8_t ucY = 0; ucY < ucOrder; ucY++) {
    
      if (xIn1[ucX * ucOrder + ucY] != xIn2[ucX * ucOrder + ucY])
        return false;
    }
  }
  return true;
}

// -----------------------------------------------------------------------------
void 
vMatrixSqfCopy (float * xDst, const float * xSrc, uint8_t ucOrder) {

  for (uint8_t ucX = 0; ucX < ucOrder; ucX++) {
  
    for (uint8_t ucY = 0; ucY < ucOrder; ucY++) {
    
      xDst[ucX * ucOrder + ucY] = xSrc[ucX * ucOrder + ucY];
    }
  }
}

/* ========================================================================== */
