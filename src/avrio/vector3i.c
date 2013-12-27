/**
 * @file vector3i.c
 * @brief Vecteur tridimensionnel (Implémentation)
 * @author Copyright © 2012 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * @version $Id$
 * Revision History ---
 *    20121218 - Initial version by epsilonRT
 */
#include <math.h>
#include "vector3i.h"

/* constants ================================================================ */
#define ORDER 3

/* internal public functions ================================================ */

// -----------------------------------------------------------------------------
// !!!!  Fonctions NON itératives - doivent être modifiées si ORDER modifié !!!!
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
void 
vVector3iCross    (  int * xOut,
                  const int * xIn1, 
                  const int * xIn2 )  {

  xOut[0] = (xIn1[1] * xIn2[2]) - (xIn1[2] * xIn2[1]);
  xOut[1] = (xIn1[2] * xIn2[0]) - (xIn1[0] * xIn2[2]);
  xOut[2] = (xIn1[0] * xIn2[1]) - (xIn1[1] * xIn2[0]);
}

// -----------------------------------------------------------------------------
// !!!!                    Fonctions itératives                             !!!!
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
int 
iVector3iDot (const int * xIn1, const int * xIn2) {
  int iDot = 0;
  
  for (uint8_t i = 0; i < ORDER; i++) {
  
    iDot += xIn1[i] * xIn2[i];
  }
  return iDot;
}

// -----------------------------------------------------------------------------
void 
vVector3iClear (int * xIn1) {

  for (uint8_t i = 0; i < ORDER; i++) {
  
    xIn1[i] = 0;
  }
}

// -----------------------------------------------------------------------------
bool 
bVector3iIsCleared (const int * xIn1) {

  for (uint8_t i = 0; i < ORDER; i++) {
  
    if (xIn1[i])
      return false;
  }
  return true;
}

// -----------------------------------------------------------------------------
bool 
bVector3iEq (const int * xIn1, const int * xIn2) {

  for (uint8_t i = 0; i < ORDER; i++) {
  
    if (xIn1[i] != xIn2[i])
      return false;
  }
  return true;
}

// -----------------------------------------------------------------------------
void 
vVector3iCopy (int * xDst, const int * xSrc) {

  for (uint8_t i = 0; i < ORDER; i++) {
  
    xDst[i] = xSrc[i];
  }
}

// -----------------------------------------------------------------------------
void 
vVector3iCopyLong (int * xDst, const long * xSrc) {

  for (uint8_t i = 0; i < ORDER; i++) {
  
    xDst[i] = (int)xSrc[i];
  }
}

// -----------------------------------------------------------------------------
void 
vVector3iAdd   (  int * xOut,
                  const int * xIn1, 
                  const int * xIn2 ) {

  for (uint8_t i = 0; i < ORDER; i++) {
  
    xOut[i] = xIn1[i] + xIn2[i];
  }
}

// -----------------------------------------------------------------------------
void 
vVector3iSub   (  int * xOut,
                  const int * xIn1, 
                  const int * xIn2 ) {

  for (uint8_t i = 0; i < ORDER; i++) {
  
    xOut[i] = xIn1[i] - xIn2[i];
  }
}

// -----------------------------------------------------------------------------
void 
vVector3iScale (  int * xOut ,
                  const int * xIn1, 
                  int iConst ) {

  for (uint8_t i = 0; i < ORDER; i++) {
  
    xOut[i] = xIn1[i] * iConst;
  }
}

// -----------------------------------------------------------------------------
void 
vVector3iMulK (int * xVect, int iConst) {

  vVector3iScale (xVect, xVect, iConst);
}

// -----------------------------------------------------------------------------
void 
vVector3iDivK (int * xVect, int iConst) {

  for (uint8_t i = 0; i < ORDER; i++) {
  
    xVect[i] /= iConst;
  }
}

/* ========================================================================== */
