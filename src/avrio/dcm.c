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
 * @file dcm.c
 * @brief Algorithme Direction Cosine Matrix (Implémentation)
 *

 * Implémentation basée sur le travail de Kong Wai Weng @ Cytron Technologies 
 * sur le projet easypilot.
 * Revision History ------------------------------------------------------------
 *    20130228 - Initial version by epsilonRT
 */
#include <math.h>
#include <util/atomic.h>
#include <avrio/dcm.h>
#include <avrio/delay.h>
#include <avrio/matrixsqf.h>
#include "avrio-config-dcm.h"

/* macros =================================================================== */
// Set the lower and upper limit.
#define LIMIT(i, l, u)  ((i < l)? l : ((i > u)? u : i))

// Convert the angle to 0 - 2PI rad.
#define CONVERT_0_2PI(x) {    \
  while (x > (2.0f * M_PI)) {  \
    x -= 2.0f * M_PI;      \
  }              \
  while (x < 0.0f) {      \
    x += 2.0f * M_PI;      \
  }              \
} (void) 0;

#define DCM_DT ((CFG_DCM_DT) * 1E-3)

/* structures =============================================================== */
typedef struct xDcmState {

  struct {
    uint8_t bReady:1;
    uint8_t bAccError:1;  // Pour debug
    uint8_t bRateError:1; // Pour debug
    uint8_t bMagError:1;  // Pour debug
    uint8_t unused:4;
  };
  int iError;
} xDcmState;

/* private variables ======================================================== */
static xDcmState xState;

// The Direction Cosine Matrix
static float fDcm[3][3] = {
      {1.0, 0.0, 0.0},
      {0.0, 1.0, 0.0},
      {0.0, 0.0, 1.0}
};

// Vector for corrected angular rate
static float fRateVector[3];

// Vector for proportional angular rate correction
static float fRateCorrectionP[3];

// Vector for integral angular rate correction
static float fRateCorrectionI[3];

// The attitude of the system
static xDcmAttitude xAttitude;
static xDcmAttitude xAttitudeTemp;

// Sensors reading
static float fAccVector[3];
static float fGyroVector[3];
static float fMagVector[3];

/* private functions ======================================================== */

// -----------------------------------------------------------------------------
// Read from the sensors.
static inline void
vReadSensors (void) {

  // Read from accelerometer, gyro and magnetometer.
  xState.iError = iAccelero3dRead (fAccVector);
  if (xState.iError < 0) {
  
    xState.bAccError = 1;
  }
  else {
  
    xState.iError = iGyro3dRead (fGyroVector);
    if (xState.iError < 0) {
    
      xState.bRateError = 1;
    }
    else {
    
      xState.iError = iCompass3dRead (fMagVector);
      if (xState.iError < 0) {
      
        xState.bMagError = 1;
      }
    }
  }
}
  
// -----------------------------------------------------------------------------
// Predict next state of DCM.
// This function corrects the drift in the gyroscope, by adding the proportional 
// and intergrator terms to the gyroscope readings. 
// Afterwards the DCM Matrix is updated with the new values.
static inline void
vDcmPredict (void) {
  float fDcmUpdate[3][3];
  float fDcmTemp[3][3];
  float fOmega[3];
  
  // Add the proportional and integral correction vector
  vVector3fAdd (fOmega, fGyroVector, fRateCorrectionP);
  vVector3fAdd (fRateVector, fOmega, fRateCorrectionI);
  
  // Compute the update matrix.
  fDcmUpdate[0][0] =  1.0;
  fDcmUpdate[0][1] = -fRateVector[2] * DCM_DT;  // -Z
  fDcmUpdate[0][2] =  fRateVector[1] * DCM_DT;  //  Y
  fDcmUpdate[1][0] =  fRateVector[2] * DCM_DT;  //  Z
  fDcmUpdate[1][1] =  1.0;
  fDcmUpdate[1][2] = -fRateVector[0] * DCM_DT;  // -X
  fDcmUpdate[2][0] = -fRateVector[1] * DCM_DT;  // -Y
  fDcmUpdate[2][1] =  fRateVector[0] * DCM_DT;  //  X
  fDcmUpdate[2][2] =  1.0;
  
  // Multiply the update matrix with the DCM.
  vMatrixSqfMul (MSQ(fDcmTemp), MSQ(fDcm), MSQ(fDcmUpdate), 3);
  
  // Put the result into the DCM
  vMatrixSqfCopy (MSQ(fDcm), MSQ(fDcmTemp), 3);
}
  
// -----------------------------------------------------------------------------
// This function normalizes the DCM Matrix
static inline void
vNormalize (void) {
  float fDcmTemp[2][3];
  float fError;
  
  // Compute 1/2 of the dot product between row 1 and 2 (eq. 19)
  fError = -0.5 * fVector3fDot (&fDcm[0][0], &fDcm[1][0]); // 

  // Update the first 2 rows to make it closer to orthogonal (eq. 19)
  vVector3fScale (&fDcmTemp[0][0], &fDcm[1][0], fError);
  vVector3fScale (&fDcmTemp[1][0], &fDcm[0][0], fError);
  
  vVector3fAdd (&fDcm[0][0], &fDcmTemp[0][0], &fDcm[0][0]);
  vVector3fAdd (&fDcm[1][0], &fDcmTemp[1][0], &fDcm[1][0]);
  
  //  Use the cross product of the first 2 rows to get the 3rd row  (eq. 20)
  vVector3fCross (&fDcm[2][0], &fDcm[0][0], &fDcm[1][0]);
  
  //  Rescale row 1 so that the magnitude is as close as possible to 1.  
  vVector3fNormalize (&fDcm[0][0]);
  //  Rescale row 2 so that the magnitude is as close as possible to 1.  
  vVector3fNormalize (&fDcm[1][0]);
  //  Rescale row 3 so that the magnitude is as close as possible to 1.  
  vVector3fNormalize (&fDcm[2][0]);
}

// -----------------------------------------------------------------------------
// This function uses the accelerometer and magnetometer readings to calculate 
// a reference vector used to remove the drift from the gyroscope sensor. 
static inline void
vDcmCorrectDrift (void) {
  float fRollPitchError[3];
  float fAccMagnitude;
  float fAccWeight;
  float fHeadingX;
  float fHeadingY;
  float fEarthYawError;
  float fBodyYawError[3];
  float fIntegratorMagnitude;
  uint8_t i;

  // #####################################################################
  // Correct pitch and roll drift base on acccelerometer.
  // #####################################################################

  // Calculate the magnitude of the acceleration vector.
  fAccMagnitude = fVector3fAbs (fAccVector);
  
  // Dynamic weighting of accelerometer info (reliability filter).
  // Weight for accelerometer info (<0.66G = 0.0, 1G = 1.0 , >1.33G = 0.0).
  fAccWeight = 1.0 - (3.0 * fabs (1.0 - fAccMagnitude));
  if (fAccWeight < 0.0) {
  
    fAccWeight = 0.0;
  }  

  // Cross product (fAccVector X fDcm[2]) to get the error
  vVector3fCross (fRollPitchError, fAccVector, &fDcm[2][0]);

  for (uint8_t i = 0; i < 3; i++) {
    // Limit the error.
    fRollPitchError[i] = LIMIT(fRollPitchError[i], -0.12, 0.12);
    
    // Compute the proportional correction vector.
    if (xState.bReady == 0) {
    
      fRateCorrectionP[i] = fRollPitchError[i] * CFG_DCM_ACC_KP * fAccWeight * 
                                                                          10.0f;
    }
    else {
    
      fRateCorrectionP[i] = fRollPitchError[i] * CFG_DCM_ACC_KP * fAccWeight;
    }  
    
    // Compute the integral correction vector.
    fRateCorrectionI[i] += fRollPitchError[i] * CFG_DCM_ACC_KI * DCM_DT * 
                                                                    fAccWeight;
  }  
  
  // #####################################################################
  // Calculate the magnetic heading
  // #####################################################################
  xAttitudeTemp.fMagHeading = fCompass3dHeadingRadian (xAttitude.fAtt, 
                                                  fMagVector, CFG_DCM_MAG_DECL);

  // #####################################################################
  // Correct the yaw drift base on magnetic
  // #####################################################################
    
  fHeadingX =  cos(xAttitudeTemp.fMagHeading);
  fHeadingY =  sin(xAttitudeTemp.fMagHeading);
  
  // Calculate yaw error along the Earth frame
  // Cross product (fTiltCompensatedMag Vector X fDcm[0]) to get the error
  fEarthYawError = (fDcm[0][0] * (fHeadingY)) - (fDcm[1][0] * fHeadingX);
  
  // Convert the error to the body frame.
  vVector3fScale (fBodyYawError, &fDcm[2][0], fEarthYawError);
  //vVector3fCross (fBodyYawError, fMagVector, &fDcm[0][0]);

  for (i = 0; i < 3; i++) {
  
    // Limit the body yaw error.
    fBodyYawError[i] = LIMIT(fBodyYawError[i], -50.0, 50.0);
    
    // Compute the proportional correction vector.
    fRateCorrectionP[i] += fBodyYawError[i] * CFG_DCM_MAG_KP;
    
    // Compute the integral correction vector.
    fRateCorrectionI[i] += fBodyYawError[i] * CFG_DCM_MAG_KI * DCM_DT;
  }
  // #####################################################################
  // #####################################################################

  // Here we will place a limit on the integrator so that the integrator cannot
  // ever exceed half the saturation limit of the gyros.
  fIntegratorMagnitude = fVector3fAbs (fRateCorrectionI);
  
  if (fIntegratorMagnitude > DEG_TO_RAD(300.0)) {
  
    for (i = 0; i < 3; i++) {
    
      fRateCorrectionI[i] *= 0.5 * DEG_TO_RAD(300.0) / fIntegratorMagnitude;
    }
  }  
}

// -----------------------------------------------------------------------------
// This function calculates the Euler angles using the DCM Matrix
static inline void 
vCalculateEuler (void) {

  xAttitudeTemp.fAtt[PITCH] = asin (-fDcm[2][0]);
  xAttitudeTemp.fAtt[ROLL]  = atan2(fDcm[2][1], fDcm[2][2]);
  xAttitudeTemp.fAtt[YAW]   = atan2(fDcm[1][0], fDcm[0][0]);
}

/* internal public functions ================================================ */

// -----------------------------------------------------------------------------
int 
iDcmInit (void) {
  float fGyroVectorSum[3] = {0.0, 0.0, 0.0};
  
  // Get the average value of the sensors
  for (uint8_t i = 0; i < 100; i++) {

    // Read from the sensors
    vReadSensors();
    if (xState.iError < 0)
      return xState.iError;
    
    // Accumulate the gyro reading
    vVector3fAdd (fGyroVectorSum, fGyroVectorSum, fGyroVector);
    
    // Delay for 20ms
    delay_ms (20);
  }  
  
  // Use the gyro average reading as the initial value for bias
  vVector3fDivK (fGyroVectorSum, -100.0);
  vVector3fCopy (fRateCorrectionI, fGyroVectorSum);
    
  // Normalize the reading for acclerometer
  vVector3fNormalize (fAccVector);
  
  // Normalize the reading for magnetometer
  vVector3fNormalize (fMagVector);
  
  // Fill in the DCM based on the reading from the accelerometer and magnetometer
  vVector3fCopy (&fDcm[0][0], fMagVector);
  vVector3fCopy (&fDcm[2][0], fAccVector);
  
  // 2nd Row is the cross product between 3rd row and 1st row (fDcm[2] X fDcm[0])
  vVector3fCross (&fDcm[1][0], &fDcm[2][0], &fDcm[0][0]);

  return xState.iError;
}

// -----------------------------------------------------------------------------
int 
iDcmLoop (void) {
  static uint16_t usLoopCount;
  
  // Indicate that the DCM is ready when > 5s.
  if (++usLoopCount > (CFG_DCM_READY_DELAY / DCM_DT)) {

    xState.bReady = 1;
  }	

  // Read from the sensors
  vReadSensors();
  if (xState.iError >= 0) {
  
    // Predict next state of DCM
    vDcmPredict();
    
    // Normalize the DCM
    vNormalize();
    
    // Correct the gyro drift
    vDcmCorrectDrift();
    
    // Convert the DCM to Euler angle
    vCalculateEuler();
    
    // Convert the yaw angle to 0 - 2PI
    CONVERT_0_2PI(xAttitudeTemp.fAtt[YAW]);
    
    // Calculate the actual rate
    vVector3fAdd (xAttitudeTemp.fRate, fGyroVector, fRateCorrectionI);
    
    // Copy the attitude to the private global variable.
    // We don't want to switch the context during this process
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
    
      xAttitude = xAttitudeTemp;
    }
    if (xState.bReady == 0)
      xState.iError = DCM_NOT_READY;
  }
    
  return xState.iError;
}

// -----------------------------------------------------------------------------
const 
xDcmAttitude * pxDcmAttitude (void) {

  return (const xDcmAttitude *) &xAttitude;
}

/* ========================================================================== */
