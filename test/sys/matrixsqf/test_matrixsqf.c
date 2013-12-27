/**
 * @file .c
 * @brief Tests unitaires du module \ref menu_module "Menu".
 * @author Copyright _ 2011-2012 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * @version $Id$
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
