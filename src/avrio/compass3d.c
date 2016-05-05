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
 * @file compass3d.c
 * @brief Compas magnétique 3 axes
 *
 * Revision History ------------------------------------------------------------
 *    20121220 - Initial version by epsilonRT
 */
#include "avrio-config.h"

#ifdef AVRIO_COMPASS3D_ENABLE
/* ========================================================================== */
#include <math.h>
#include <string.h>
#include <avr/pgmspace.h>
#include <avrio/compass3d.h>
#include <avrio/accelero3d.h>
#include <avrio/eeprom.h>

/* constants ================================================================ */
#define ROLL  0
#define PITCH 1
#define YAW   2

/* structures =============================================================== */
typedef struct xCompass3dCalibrationEE {

  xCompass3dCalibration xCalibration;
  uint8_t ucCrc;
} xCompass3dCalibrationEE;

/* public functions ========================================================= */
extern int iCompass3dDeviceInit (void);

/* private variables ======================================================== */
static xCompass3dCalibration xCalibration;
static xCompass3dCalibrationEE xCalibrationEE EEMEM;

/* private functions ======================================================== */

// -----------------------------------------------------------------------------
static void 
vSaveCalibration (void) {

  vEepromSaveBlock (&xCalibration, &xCalibrationEE, sizeof(xCompass3dCalibration));
}
  
/* internal public functions ================================================ */

// -----------------------------------------------------------------------------
int 
iCompass3dInit (void) {

  int iError = iCompass3dDeviceInit();
  
  if (iError == 0) {
  
    if (iEepromLoadBlock (&xCalibration, &xCalibrationEE, 
                                          sizeof(xCompass3dCalibration)) < 0) {
    
      vCompass3dClearCalibration();
      iError = COMPASS3D_NOT_CALIBRATED;
    }
  }
  return iError;
}

// -----------------------------------------------------------------------------
void 
vCompass3dSetCalibration (const xCompass3dCalibration * pxCalibration) {

  memcpy (&xCalibration, pxCalibration, sizeof(xCompass3dCalibration));
  vSaveCalibration();
}

// -----------------------------------------------------------------------------
void 
vCompass3dClearCalibration (void) {

  memset (&xCalibration, 0, sizeof(xCompass3dCalibration));
  vSaveCalibration();
}

// -----------------------------------------------------------------------------
void 
vCompass3dCalibration (xCompass3dCalibration * pxCalibration) {

  memcpy (pxCalibration, &xCalibration, sizeof(xCompass3dCalibration));
}

// -----------------------------------------------------------------------------
int 
iCompass3dRead (float * fMag) {
  int iRaw[3];
  int iError;

  iError = iCompass3dReadRaw (iRaw);

  if (iError == 0) {
  
    for (uint8_t i = 0; i < 3; i++) {
    
      fMag[i] = (float)(iRaw[i] - xCalibration.iMin[i]) / 
                (float)(xCalibration.iMax[i] - xCalibration.iMin[i]) * 2.0f - 1.0f;
    }
  }
  
  return iError;
}

// -----------------------------------------------------------------------------
int 
iCompass3dCalibrate (FILE * xOutStream, xButMask xBut) {
  int iError;
  int iMag[3];
  xCompass3dCalibration xCal = {  .iMin = { INT16_MAX,  INT16_MAX,  INT16_MAX}, 
                                  .iMax = { INT16_MIN , INT16_MIN , INT16_MIN } };

  //                           0123456789012345  0123456789012345
  fprintf_P (xOutStream, PSTR("Calibrate compas\nPress button....\n"));

  while (xButGet (xBut) == BUTTON_NO_BUTTON);
    ; // attente appui bouton
  while (xButGet (xBut) != BUTTON_NO_BUTTON);
    ; // attente relachement bouton

  //                           0123456789012345  0123456789012345
  fprintf_P (xOutStream, PSTR("Calibrating.....\nMove the device!\n"));
  
  while (xButGet (xBut) == BUTTON_NO_BUTTON) {

    iError = iCompass3dReadRaw (iMag);
    if (iError < 0) {
    
      vCompass3dClearCalibration();
      return iError;
    }
    
    for (uint8_t i = 0; i < 3; i++) {
    
      if (iMag[i] < xCal.iMin[i]) 
        xCal.iMin[i] = iMag[i];
      if (iMag[i] > xCal.iMax[i]) 
        xCal.iMax[i] = iMag[i];
    }
  }
  vCompass3dSetCalibration (&xCal);
  
  return 0;
}

// -----------------------------------------------------------------------------
int 
iCompass3dHeading (const float * fAtt, const float * fMag, float fMagDeclinaison) {
  float fHeadingRad;
  float fAttRad[3];
  
  vVector3fCopy (fAttRad, fAtt);
  vVector3fMulK (fAttRad, M_PI / 180.0f);
  
  fHeadingRad = fCompass3dHeadingRadian (fAttRad, fMag, DEG_TO_RAD(fMagDeclinaison));

  return (int)RAD_TO_DEG(fHeadingRad);
}

// -----------------------------------------------------------------------------
float 
fCompass3dHeadingRadian (const float * fAtt, const float * fMag, float fMagDeclinaison) {
  float fMx, fMy;
  float fHeading;
  
  // fAtt[ROLL] = roll
  // fAtt[PITCH] = pitch
  fMx = fMag[0] * cos (fAtt[PITCH]) + 
        fMag[1] * sin (fAtt[ROLL]) * sin (fAtt[PITCH]) -
        fMag[2] * cos (fAtt[ROLL]) * sin (fAtt[PITCH]);
        
  fMy = fMag[1] * cos (fAtt[ROLL]) + fMag[2] * sin (fAtt[ROLL]);
  
  fHeading = atan2 (-fMy, fMx) + fMagDeclinaison;
  while (fHeading < 0) 
    fHeading += 2.0f * M_PI;

  return fHeading;
}

// -----------------------------------------------------------------------------
// Returns the number of degrees from the From xVector3f projected into
// the horizontal plane is away from north.
// 
// Description of iHeading algorithm: 
// Shift and scale the magnetic reading based on calibration data to
// to find the North xVector3f. Use the acceleration readings to
// determine the Down xVector3f. The cross product of North and Down
// vectors is East. The vectors East and North form a basis for the
// horizontal plane. The From xVector3f is projected into the horizontal
// plane and the angle between the projected xVector3f and north is
// returned.
int 
iCompass3dHeadingFrom (const float * fFrom) {
  int iHeading, iError;
  float fAcc[3], fMag[3], fEast[3], fNorth[3];
  float fAccMean[3] = {0,0,0};
  float fMagMean[3] = {0,0,0};
  
  for (uint8_t ucCount = 0; ucCount < 5; ucCount++) {

    iError = iAccelero3dRead (fAcc);
    if (iError < 0)
      return iError;
    iError = iCompass3dRead  (fMag);
    if (iError < 0)
      return iError;
    
    vVector3fAdd (fAccMean, fAccMean, fAcc);
    vVector3fAdd (fMagMean, fMagMean, fMag);
  }
  
  vVector3fDivK (fAccMean, 5);
  vVector3fDivK (fMagMean, 5);

  vVector3fNormalize(fAccMean);
  //vVector3fNormalize(fMagMean);

  // compute fEast and fNorth
  vVector3fCross ( fEast, fMagMean, fAccMean);
  vVector3fNormalize (fEast);
  vVector3fCross (fNorth, fAccMean, fEast);
  vVector3fNormalize (fNorth);

  // compute iHeading
  iHeading = round (atan2 (fVector3fDot(fEast, fFrom), 
                           fVector3fDot(fNorth, fFrom)) * 180 / M_PI);
  if (iHeading < 0) 
    iHeading += 360;
  return iHeading;
}

/* ========================================================================== */
#endif /* AVRIO_COMPASS3D_ENABLE defined */
