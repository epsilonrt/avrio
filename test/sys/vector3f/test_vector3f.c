/**
 * @file .c
 * @brief Tests unitaires du module \ref menu_module "Menu".
 * @author Copyright _ 2011-2012 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>

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
