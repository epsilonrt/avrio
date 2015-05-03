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
#include <avrio/vector3f.h>

#define ORDER 3

const float xVect[ORDER] = {1,2,3};
float xIn[ORDER];
float xOut[ORDER];
bool isTrue;
float fValue;

/* main ===================================================================== */
int
main (void) {


  for (;;) {
  
    vVector3fCopy (xIn, xVect);
    
    vVector3fCross (xOut, xIn, xIn);  // [0, 0, 0]
    fValue = fVector3fDot (xIn, xIn); // 14
    fValue = fVector3fAbs (xIn);      // sqrt(14) ~ 3.74166
    
    vVector3fAdd (xOut, xIn, xIn);
    vVector3fSub (xOut, xIn, xIn);
    
    vVector3fMulK (xIn, 4.0);
    vVector3fDivK (xIn, 4.0);
    vVector3fNormalize (xIn); // [0.26726124, 0.53452247, 0.80178368]
    
    vVector3fCopy (xIn, xVect);
    isTrue = bVector3fEq (xIn, xVect);
    vVector3fClear (xIn);
    isTrue = bVector3fIsCleared (xIn);
  }
  return 0;
}

/* ========================================================================== */
