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
 * @file .c
 * @brief Tests unitaires du module \ref menu_module "Menu".
 *

   ========================================================================== */
#include <avrio/matrixsqf.h>

#define ORDER 3

const float xMat[ORDER][ORDER] = { {1,2,3}, {4,5,6}, {7,8,9} };
float xIn[ORDER][ORDER];
float xOut[ORDER][ORDER];
bool isTrue;

/* main ===================================================================== */
int
main (void) {


  for (;;) {
  
    vMatrixSqfCopy (MSQ(xIn), MSQ(xMat), ORDER);
    
    vMatrixSqfAdd (MSQ(xOut), MSQ(xIn), MSQ(xIn), ORDER);
    vMatrixSqfSub (MSQ(xOut), MSQ(xIn), MSQ(xIn), ORDER);
    vMatrixSqfMul (MSQ(xOut), MSQ(xIn), MSQ(xIn), ORDER);
    
    vMatrixSqfMulK (MSQ(xIn), 4.0, ORDER);
    vMatrixSqfDivK (MSQ(xIn), 4.0, ORDER);
    
    isTrue = bMatrixSqfEq (MSQ(xIn), MSQ(xMat), ORDER);
    vMatrixSqfClear (MSQ(xIn), ORDER);
    isTrue = bMatrixSqfIsCleared (MSQ(xIn),ORDER);
  }
  return 0;
}

/* ========================================================================== */
