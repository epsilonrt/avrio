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
 * @file gyro3d.c
 * @brief Gyroscope 3 axes (Implémentation commune)
 *
 * Revision History ------------------------------------------------------------
 *    20121220 - Initial version by epsilonRT
 */
#include "avrio-config.h"

#ifdef AVRIO_GYRO3D_ENABLE
/* ========================================================================== */
#  include <math.h>
#  include <string.h>
#  include <avrio/gyro3d.h>
#  include <avrio/task.h>
#  include <avrio/eeprom.h>
#  include "avrio-board-kernel.h"

/* constants ================================================================ */
#define RK_ORDER 4
#define AXE_X 0
#define AXE_Y 1
#define AXE_Z 2
#define CALIBRATE_SAMPLE_DELAY  20

/* structures =============================================================== */
typedef struct xGyro3dZeroEE {

  int iZero[3];
  uint8_t ucCrc;
} xGyro3dZeroEE;

/* extern variables ========================================================= */
/* Valeur du Lsb du gyro */
extern const float fGyro3dLsb;

/* public functions ========================================================= */
extern int iGyro3dDeviceInit (void);

/* private variables ======================================================== */
static int iGyroZero[3];
static xGyro3dZeroEE xGyroZeroEE EEMEM;

static uint8_t ucBufferIndex;
static float fBuffer[3][RK_ORDER];
static float xLpFilterRate[3];
static ticks_t xPrevRateTime;

/* private functions ======================================================== */

// -----------------------------------------------------------------------------
static inline void 
vPushSample (float xSample[3], float fDeltaT) {

  for (uint8_t i = 0; i < 3; i++) {
  
    fBuffer[i][ucBufferIndex]   = xSample[i] * fDeltaT;
  }
  ucBufferIndex++;
  ucBufferIndex %= RK_ORDER;
}

// -----------------------------------------------------------------------------
static float 
fRungeKuttaTerm (uint8_t ucAxe) {
  float fTerm = 0;
  uint8_t ucIndex = ucBufferIndex, ucCount;
  const float fCoeff[RK_ORDER] = { 1.0, 2.0, 2.0, 1.0 };
  
  // runge-kutta 4 term
  // 1/6 ( vali-3 + 2 vali-2 + 2 vali-1 + vali )
  for (ucCount = 0; ucCount < RK_ORDER; ucCount++) {
  
    ucIndex--;
    ucIndex %= RK_ORDER;
    fTerm += fBuffer[ucAxe][ucIndex] * fCoeff[ucCount];
  }
  return fTerm / 6.0;
}

/* internal public functions ================================================ */

// -----------------------------------------------------------------------------
int 
iGyro3dInit (void) {

  int iError = iGyro3dDeviceInit();
  if (iEepromLoadBlock (&iGyroZero, &xGyroZeroEE, sizeof(iGyroZero)) < 0) {
  
    vVector3iClear (iGyroZero);
    vGyro3dSaveZero();
    iError = GYRO3D_NOT_CALIBRATED;
  }
  return iError;
}

// -----------------------------------------------------------------------------
int 
iGyro3dRead (float * xRate) {
  int iError;
  int xRaw[3];
  uint8_t j;
 
  iError = iGyro3dReadRaw (xRaw);

  if (iError == 0) {
  
    for (j = 0; j < 3; j++) {
    
      xRate[j] = (float)(xRaw[j] - iGyroZero[j]) * fGyro3dLsb;
    }
  }
 
  return iError;
}

// -----------------------------------------------------------------------------
int 
iGyro3dCalibrate (uint16_t usNumOfSamples) {
  int i;
  int8_t j;
  int iMes[3];
  int32_t lSum[3] = {0, 0, 0};
  int iError = 0;
  
  for (i = 0; i < usNumOfSamples; i++) {
  
    iError = iGyro3dReadRaw (iMes);
    if (iError < 0) {
    
      vVector3iClear (iGyroZero);
      vGyro3dSaveZero();
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
  
  vVector3iCopyLong (iGyroZero, lSum);
  vGyro3dSaveZero();
  
  return iError;
}

// -----------------------------------------------------------------------------
void 
vGyro3dSaveZero (void) {

  vEepromSaveBlock (&iGyroZero, &xGyroZeroEE, sizeof(iGyroZero));
}

// -----------------------------------------------------------------------------
void 
vGyro3dZero (int * iZero) {

  vVector3iCopy (iZero, iGyroZero);
}

// -----------------------------------------------------------------------------
void 
vGyro3dSetZero (const int * iZero) {

  vVector3iCopy (iGyroZero, iZero);
  vGyro3dSaveZero();
}

/**************************** Calcul d'attitude *******************************/

// -----------------------------------------------------------------------------
int 
iGyro3dAttitude (float * xAtt, const float * xRate, ticks_t xRateTime, float fA) {
  static float fAlpha = 1.0;

  if (fA != fAlpha) {
  
    if ((fA > 0) && (fA <= 1.0)) {
    
      fAlpha = fA; // changement coefficient valide
      fA = 0; // => RAZ du filtre
    }
  }
  
  if (xPrevRateTime) {
    float fDeltaS;
    
    // Intégration...
    // Calcul de l'écart de temps entre les 2 mesures
    fDeltaS = (float)((int)xRateTime - (int)xPrevRateTime) / 
              (float)AVRIO_KERNEL_TICK_RATE; 
    xPrevRateTime = xRateTime;

    // Filtrage passe-bas vitesse angulaire
    for (uint8_t i = 0; i < 3; i++) {
    
      xLpFilterRate[i] = xRate[i] * fAlpha + (xLpFilterRate[i] * (1.0 - fAlpha));
    }
    
    // Intégration de Runge-Kutta d'ordre 4
    vPushSample (xLpFilterRate, fDeltaS); // empile la valeur dans le buffer de calcul
    for (uint8_t i = 0; i < 3; i++) {
    
      xAtt[i] += fRungeKuttaTerm (i);
    }
  }
  else {
  
    xPrevRateTime = xRateTime;
    vVector3fClear (xAtt);
  }
  
  return 0;
}

// -----------------------------------------------------------------------------
void 
vGyro3dAttidudeClear (float * xAtt) {

  vVector3fClear (xAtt);
  vVector3fClear (xLpFilterRate);
  ucBufferIndex = 0;
  memset (fBuffer, 0, sizeof(float) * 3 * RK_ORDER);
}

/* ========================================================================== */
#endif /* AVRIO_GYRO3D_ENABLE defined */
