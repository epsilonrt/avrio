/**
 * @file compass3d_lsm303.c
 * @brief Compas 3 axes LSM303 de ST Microelectronics (Implémentation)
 * @author Copyright © 2012 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * @version $Id$
 * Revision History ---
 *    20121218 - Initial version by epsilonRT
 */
#include "avrio-config.h"

#ifdef AVRIO_COMPASS3D_ENABLE
/* ========================================================================== */
#  include <math.h>
#  include <avrio/compass3d.h>
#  include <avrio/accelero3d.h>
#  include <avrio/twi.h>
#  include <avrio/delay.h>
#  include <avrio/lsm303.h>
#  include "avrio-board-lsm303.h"

/* macros =================================================================== */
#define LSM303_CRA_REG_M_INIT (LSM303_ODR_M)
#define LSM303_CRB_REG_M_INIT (LSM303_FULL_SCALE_M)
#define LSM303_MR_REG_M_INIT  (0)

/* internal public functions ================================================ */

// -----------------------------------------------------------------------------
int 
iCompass3dDeviceInit (void) {
  eTwiStatus eError = TWI_SUCCESS;
  uint8_t ucData;
  xTwiFrame xFrame = { .xAddr = LSM303_ADDRESS_M, .pxData = &ucData, .xLen = 1 };
 
#if LSM303_DEVICE == LSM303DLM_DEVICE
  eError = eTwiMem8Read (LSM303_WHO_AM_I_M, &xFrame);
  
  if ((eError == TWI_SUCCESS) && (ucData != LSM303_ADDRESS_M)) {
  
    eError = TWI_ERROR_INVALID_DEVICE;
  }
#else
  eError = eTwiMem8Read (LSM303_CRA_REG_M, &xFrame);
#endif
  
  if (eError == TWI_SUCCESS) {

    ucData = LSM303_CRA_REG_M_INIT;
    eError =  eTwiMem8Write (LSM303_CRA_REG_M, &xFrame);
    ucData = LSM303_CRB_REG_M_INIT;
    eError =  eTwiMem8Write (LSM303_CRB_REG_M, &xFrame);
    ucData = LSM303_MR_REG_M_INIT;
    eError =  eTwiMem8Write (LSM303_MR_REG_M, &xFrame);
  }
  return (int)eError;
}

// -----------------------------------------------------------------------------
int 
iCompass3dReadRaw (int * iMag) {
  eTwiStatus eError;
  uint8_t ucData[6];
  xTwiFrame xFrame = { .xAddr = LSM303_ADDRESS_M, .pxData = ucData, .xLen = 6 };

  eError = eTwiMem8Read (LSM303_OUT_X_H_M, &xFrame);
  
  if (eError == TWI_SUCCESS) {
    int iMes[3];
  
    
    iMes[0] = ((int)((ucData[0] << 8) + ucData[1]));
#if LSM303_DEVICE == LSM303DLH_DEVICE
    iMes[1] = ((int)((ucData[2] << 8) + ucData[3]));
    iMes[2] = ((int)((ucData[4] << 8) + ucData[5]));
#else
    iMes[2] = ((int)((ucData[2] << 8) + ucData[3]));
    iMes[1] = ((int)((ucData[4] << 8) + ucData[5]));
#endif

#if LSM303_AXIS_ORDER == LSM303_AXIS_ORDER_YXZ
    iMag[0] = iMes[1];
    iMag[1] = iMes[0];
    iMag[2] = iMes[2];
#elif LSM303_AXIS_ORDER == LSM303_AXIS_ORDER_YZX
    iMag[0] = iMes[1];
    iMag[1] = iMes[2];
    iMag[2] = iMes[0];
#elif LSM303_AXIS_ORDER == LSM303_AXIS_ORDER_ZYX
    iMag[0] = iMes[2];
    iMag[1] = iMes[1];
    iMag[2] = iMes[0];
#else
    iMag[0] = iMes[0];
    iMag[1] = iMes[1];
    iMag[2] = iMes[2];
#endif
#if LSM303_AXIS_SIGN_M & _BV(2)
    iMag[0] *= -1;
#endif
#if LSM303_AXIS_SIGN_M & _BV(1)
    iMag[1] *= -1;
#endif
#if LSM303_AXIS_SIGN_M & _BV(0)
    iMag[2] *= -1;
#endif
  }
 
  return (int)eError;
}

/* ========================================================================== */
#endif /* AVRIO_COMPASS3D_ENABLE defined */
