/**
 * @file gyro3d_l3g.c
 * @brief Gyroscope 3 axes L3G4200D de ST Microelectronics (Implémentation)
 * @author Copyright © 2012 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * @version $Id$
 * Revision History ---
 *    20121218 - Initial version by epsilonRT
 */
#include "avrio-config.h"

#ifdef AVRIO_GYRO3D_ENABLE
/* ========================================================================== */
#  include <avrio/gyro3d.h>
#  include <avrio/l3g.h>
#  include <avrio/twi.h>
#  include <avrio/delay.h>
#  include "avrio-board-l3g.h"
#  include <math.h>

/* macros =================================================================== */
#define L3G_CTRL_REG1_INIT (L3G_ODR|L3G_LPBW|0x0F)
#define L3G_CTRL_REG2_INIT 0
#define L3G_CTRL_REG3_INIT 0
#define L3G_CTRL_REG5_INIT 0

#ifdef L3G_RADIANS
#define L3G_UNITY(a) ((a) / 180.0 * M_PI)
#else
#define L3G_UNITY(a)  (a)
#endif

#if L3G_FULL_SCALE == 250
# define L3G_LSB            L3G_UNITY(250.0 * 2.0 / 65536.0)
# define L3G_CTRL_REG4_INIT  0

#elif L3G_FULL_SCALE == 500
# define L3G_LSB            L3G_UNITY(500.0 * 2.0 / 65536.0)
# define L3G_CTRL_REG4_INIT  0x10

#elif L3G_FULL_SCALE == 2000
# define L3G_LSB            L3G_UNITY(2000.0 * 2.0 / 65536.0)
# define L3G_CTRL_REG4_INIT  0x20

#else
#error L3G_FULL_SCALE: invalid value.
#endif

/* public variables ========================================================= */
const float fGyro3dLsb = L3G_LSB;

/* internal public functions ================================================ */
// -----------------------------------------------------------------------------
int 
iGyro3dDeviceInit (void) {
  eTwiStatus eError;
  uint8_t ucData;
  xTwiFrame xFrame = { .xAddr = L3G_ADDRESS, .pxData = &ucData, .xLen = 1 };

  delay_ms (10);
  eError = eTwiMem8Read (L3G_WHO_AM_I, &xFrame);
    
  if (eError == TWI_SUCCESS) {

    if ((ucData != 0xD3) && (ucData != 0xD4)) {
    
      eError = TWI_ERROR_INVALID_DEVICE;
    }
    else {
    
      ucData = L3G_CTRL_REG2_INIT;
      eError = eTwiMem8Write (L3G_CTRL_REG2, &xFrame);
      ucData = L3G_CTRL_REG3_INIT;
      eError = eTwiMem8Write (L3G_CTRL_REG3, &xFrame);
      ucData = L3G_CTRL_REG4_INIT;
      eError = eTwiMem8Write (L3G_CTRL_REG4, &xFrame);
      ucData = L3G_CTRL_REG5_INIT;
      eError = eTwiMem8Write (L3G_CTRL_REG5, &xFrame);
      ucData = L3G_CTRL_REG1_INIT;
      eError = eTwiMem8Write (L3G_CTRL_REG1, &xFrame);
      delay_ms(250);
    }
  }
  return (int)eError;
}

// -----------------------------------------------------------------------------
int 
iGyro3dReadRaw (int * xRate) {
  eTwiStatus eError;
  uint8_t ucData[6];
  xTwiFrame xFrame = { .xAddr = L3G_ADDRESS, .pxData = ucData, .xLen = 6 };
    
  eError = eTwiMem8Read (L3G_OUT_X_L | _BV(L3G_AUTOINC), &xFrame);
  if (eError == TWI_SUCCESS) {
    int xV[3];
    
    xV[0] = (int)(ucData[1] << 8 | ucData[0]);
    xV[1] = (int)(ucData[3] << 8 | ucData[2]);
    xV[2] = (int)(ucData[5] << 8 | ucData[4]);

#if L3G_AXIS_ORDER == L3G_AXIS_ORDER_YXZ
    xRate[0] = xV[1];
    xRate[1] = xV[0];
    xRate[2] = xV[2];
#elif L3G_AXIS_ORDER == L3G_AXIS_ORDER_YZX
    xRate[0] = xV[1];
    xRate[1] = xV[2];
    xRate[2] = xV[0];
#elif L3G_AXIS_ORDER == L3G_AXIS_ORDER_ZYX
    xRate[0] = xV[2];
    xRate[1] = xV[1];
    xRate[2] = xV[0];
#else
    xRate[0] = xV[0];
    xRate[1] = xV[1];
    xRate[2] = xV[2];
#endif

#if L3G_AXIS_SIGN & _BV(2)
    xRate[0] *= -1;
#endif
#if L3G_AXIS_SIGN & _BV(1)
    xRate[1] *= -1;
#endif
#if L3G_AXIS_SIGN & _BV(0)
    xRate[2] *= -1;
#endif
  }
 
  return eError;
}
/* ========================================================================== */
#endif /* AVRIO_GYRO3D_ENABLE defined */
