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
 * @file accelero3d.c
 * @brief Acceleromètre 3 axes (Implémentation commune)
 *
 * Revision History ------------------------------------------------------------
 *    20121221 - Initial version by epsilonRT
 */
#include "avrio-config.h"

#ifdef AVRIO_ACCELERO3D_ENABLE
/* ========================================================================== */
#  include <math.h>
#  include <avrio/accelero3d.h>
#  include <avrio/eeprom.h>
#  include <avrio/delay.h>

/* constants ================================================================ */
#define CALIBRATE_SAMPLE_DELAY 20

/* structures =============================================================== */
typedef struct xAccelero3dZeroEE {

  int iZero[3];
  uint8_t ucCrc;
} xAccelero3dZeroEE;

/* extern variables ========================================================= */
/* Valeur du Lsb de l'accelro */
extern const float fAccelero3dLsb;

/* public functions ========================================================= */
extern int iAccelero3dDeviceInit (void);

/* private variables ======================================================== */
static int iAcceleroZero[3];
static xAccelero3dZeroEE xAcceleroZeroEE EEMEM;

/* private functions ======================================================== */

// -----------------------------------------------------------------------------
static void 
vAccelero3dSaveZero (void) {

  vEepromSaveBlock (&iAcceleroZero, &xAcceleroZeroEE, sizeof(iAcceleroZero));
}

/* internal public functions ================================================ */

// -----------------------------------------------------------------------------
int 
iAccelero3dInit (void) {

  int iError = iAccelero3dDeviceInit();
  
  if (iError == 0) {
  
    if (iEepromLoadBlock (&iAcceleroZero, &xAcceleroZeroEE, sizeof(iAcceleroZero)) < 0) {
    
      vVector3iClear (iAcceleroZero);
      vAccelero3dSaveZero();
      iError = ACCELERO3D_NOT_CALIBRATED;
    }
  }
  return iError;
}

// -----------------------------------------------------------------------------
int 
iAccelero3dRead (float * fAcc) {
  int iError;
  int iRaw[3];
 
  iError = iAccelero3dReadRaw (iRaw);

  if (iError == 0) {
  
    for (uint8_t i = 0; i < 3; i++) {
    
      fAcc[i] = (float)(iRaw[i] - iAcceleroZero[i]) * fAccelero3dLsb;
    }
  }
 
  return iError;
}

// -----------------------------------------------------------------------------
int 
iAccelero3dCalibrate (uint16_t usNumOfSamples) {
  int i;
  uint8_t j;
  int iMes[3];
  int32_t lSum[3] = {0, 0, 0};
  int iError = 0;
  
  for (i = 0; i < usNumOfSamples; i++) {
  
    iError = iAccelero3dReadRaw (iMes);
    if (iError < 0) {
    
      vVector3iClear (iAcceleroZero);
      vAccelero3dSaveZero();
      return iError;
    }
    for (j = 0; j < 3; j++) {
    
      lSum[j] += iMes[j];
    }
    delay_ms (CALIBRATE_SAMPLE_DELAY);
  }
  
  for (j = 0; j < 3; j++) {
  
    lSum[j] /= usNumOfSamples;
  }
  
  lSum[2] -= (int)round(1.0 / fAccelero3dLsb);
  vVector3iCopyLong (iAcceleroZero, lSum);
  vAccelero3dSaveZero();
  
  return iError;
}

// -----------------------------------------------------------------------------
void 
vAccelero3dZero (int * iZero) {

  vVector3iCopy (iZero, iAcceleroZero);
}

// -----------------------------------------------------------------------------
void 
vAccelero3dSetZero (const int * iZero) {

  vVector3iCopy (iAcceleroZero, iZero);
  vAccelero3dSaveZero();
}

// -----------------------------------------------------------------------------
// Calcul effectué conformément à la note d'application AN3461 de Freescale 
// Semiconductor (http://cache.freescale.com/files/sensors/doc/app_note/AN3461.pdf)
void 
vAccelero3dAttitude (float * fAtt, const float * fAcc, float fA) {
  static float fAccLpFilter[3];
  static float fAttLpFilter[3];
  static float fAlpha = 1.0;
  float fAttTemp[2];
  
  if (fA != fAlpha) {
  
    if ((fA > 0) && (fA <= 1.0)) {
    
      fAlpha = fA; // changement coefficient valide
      fA = 0; // => RAZ du filtre
    }
  }
  
  if (fA == 0) {
    
    // Remise à zéro du filtre
    vVector3fClear (fAccLpFilter);
    vVector3fClear (fAttLpFilter);
  }
  
  // Filtrage passe-bas accelération
  for (uint8_t i = 0; i < 3; i++) {
  
    fAccLpFilter[i] = fAcc[i] * fAlpha + (fAccLpFilter[i] * (1.0 - fAlpha));
  }
  
	// Calcul roulis / piqué
	fAttTemp[0]  = (atan2 ( fAccLpFilter[1], fAccLpFilter[2]) * 180.0) / M_PI;
	fAttTemp[1] = (atan2 (-fAccLpFilter[0], 
                         sqrt(fAccLpFilter[1]*fAccLpFilter[1] + 
                              fAccLpFilter[2]*fAccLpFilter[2])) * 180.0) / M_PI;

  // Filtrage passe-bas attitude
  for (uint8_t i = 0; i < 2; i++) {
    
    fAttLpFilter[i] = fAttTemp[i] * fAlpha + (fAttLpFilter[i] * (1.0 - fAlpha));
    fAtt[i] = fAttLpFilter[i];
  }
}

/* ========================================================================== */
#endif /* AVRIO_ACCELERO3D_ENABLE defined */

