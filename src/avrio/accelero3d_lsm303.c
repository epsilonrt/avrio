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
 * @file accelero3d_lsm303.c
 * @brief Compas/Accelero 3 axes LSM303 de ST Microelectronics (Implémentation)
 *
 * Revision History ------------------------------------------------------------
 *    20121218 - Initial version by epsilonRT
 */
#include "avrio-config.h"

#ifdef AVRIO_ACCELERO3D_ENABLE
/* ========================================================================== */
#  include <avrio/accelero3d.h>
#  include <avrio/twi.h>
#  include <avrio/delay.h>
#  include <avrio/lsm303.h>
#  include "avrio-board-lsm303.h"

/* macros =================================================================== */
#define LSM303_CTRL_REG1_INIT (LSM303_ODR_A|0x27)
#define LSM303_CTRL_REG2_INIT 0
#define LSM303_CTRL_REG3_INIT 0
#define LSM303_CTRL_REG5_INIT 0
#define LSM303_CTRL_REG4_INIT (_BV(LSM303_BLE)|LSM303_FULL_SCALE_A)

#if LSM303_FULL_SCALE_A == LSM303_FS_A_2G
# define LSM303_ACC_LSB            (2.0 * 2.0 / 4096.0)

#elif LSM303_FULL_SCALE_A == LSM303_FS_A_4G
# define LSM303_ACC_LSB            (4.0 * 2.0 / 4096.0)

#elif LSM303_FULL_SCALE_A == LSM303_FS_A_8G
# define LSM303_ACC_LSB            (8.0 * 2.0 / 4096.0)

#else
#error LSM303_FULL_SCALE_A: invalid value.
#endif

/* public variables ========================================================= */
const float fAccelero3dLsb = LSM303_ACC_LSB;

/* internal public functions ================================================ */
// -----------------------------------------------------------------------------
int 
iAccelero3dDeviceInit (void) {
  eTwiStatus eError = TWI_SUCCESS;
  uint8_t ucData;
  xTwiFrame xFrame = { .pxData = &ucData, .xLen = 1 };
  
#if LSM303_DEVICE == LSM303DLM_DEVICE
  xFrame.xAddr = LSM303_ADDRESS_M;
  eError = (int) eTwiMem8Read (LSM303_WHO_AM_I_M, &xFrame);
  
  if ((eError == TWI_SUCCESS) && (ucData != LSM303_ADDRESS_M)) {
  
    eError = TWI_ERROR_INVALID_DEVICE;
  }
#else
  xFrame.xAddr = LSM303_ADDRESS_A;
  eError = (int) eTwiMem8Read (LSM303_CTRL_REG1_A, &xFrame);
#endif
  
  if (eError == TWI_SUCCESS) {

    xFrame.xAddr = LSM303_ADDRESS_A;
    ucData = LSM303_CTRL_REG2_INIT;
    eError =  eTwiMem8Write (LSM303_CTRL_REG2_A, &xFrame);
    ucData = LSM303_CTRL_REG3_INIT;
    eError =  eTwiMem8Write (LSM303_CTRL_REG3_A, &xFrame);
    ucData = LSM303_CTRL_REG4_INIT;
    eError =  eTwiMem8Write (LSM303_CTRL_REG4_A, &xFrame);
    ucData = LSM303_CTRL_REG5_INIT;
    eError =  eTwiMem8Write (LSM303_CTRL_REG5_A, &xFrame);
    ucData = LSM303_CTRL_REG1_INIT;
    eError =  eTwiMem8Write (LSM303_CTRL_REG1_A, &xFrame);
  }
  return (int) eError;
}

// -----------------------------------------------------------------------------
int 
iAccelero3dReadRaw (int * iAcc) {
  eTwiStatus eError;
  uint8_t ucData[6];
  xTwiFrame xFrame = { .xAddr = LSM303_ADDRESS_A, .pxData = ucData, .xLen = 6 };
  
  eError = eTwiMem8Read (LSM303_OUT_X_L_A | _BV(LSM303_AUTOINC), &xFrame);
  if (eError == TWI_SUCCESS) {
    int iMes[3];
    
    iMes[0] = ((int)((ucData[0] << 8) + ucData[1])) >> 4;
    iMes[1] = ((int)((ucData[2] << 8) + ucData[3])) >> 4;
    iMes[2] = ((int)((ucData[4] << 8) + ucData[5])) >> 4;

#if LSM303_AXIS_ORDER == LSM303_AXIS_ORDER_YXZ
    iAcc[0] = iMes[1];
    iAcc[1] = iMes[0];
    iAcc[2] = iMes[2];
#elif LSM303_AXIS_ORDER == LSM303_AXIS_ORDER_YZX
    iAcc[0] = iMes[1];
    iAcc[1] = iMes[2];
    iAcc[2] = iMes[0];
#elif LSM303_AXIS_ORDER == LSM303_AXIS_ORDER_ZYX
    iAcc[0] = iMes[2];
    iAcc[1] = iMes[1];
    iAcc[2] = iMes[0];
#else
    iAcc[0] = iMes[0];
    iAcc[1] = iMes[1];
    iAcc[2] = iMes[2];
#endif

#if LSM303_AXIS_SIGN_A & _BV(2)
    iAcc[0] *= -1;
#endif
#if LSM303_AXIS_SIGN_A & _BV(1)
    iAcc[1] *= -1;
#endif
#if LSM303_AXIS_SIGN_A & _BV(0)
    iAcc[2] *= -1;
#endif
  }
 
  return (int)eError;
}

/* ========================================================================== */
#endif /* AVRIO_ACCELERO3D_ENABLE defined */
