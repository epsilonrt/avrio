/**
 * @file test_kalman.c
 * @brief Test unitaire KALMAN
 * @author Copyright _ 2011-2012 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * Revision History ------------------------------------------------------------
 *    20120303 - Initial version by epsilonRT
 */
#include <stdio.h>
#include <math.h>
#include <avrio/delay.h>
#include <avrio/lcd.h>
#include <avrio/twi.h>
#include <avrio/task.h>
#include <avrio/accelero3d.h>
#include <avrio/gyro3d.h>
#include <avrio/kalman.h>

static xKalmanFilter xRollFilter, xPitchFilter;
static ticks_t xPrevTime;

/* constants ================================================================ */
#define LPF_CONSTANT 0.1

/* internal public functions ================================================ */
void vStdOutInit (void);
void vUpdate (void);

/* main ===================================================================== */
int
main (void) {

  vTwiInit ();
  eTwiSetSpeed (400);
  vStdOutInit();
  
  iAccelero3dInit();
  iAccelero3dCalibrate(512);
  
  iGyro3dInit();
  iGyro3dCalibrate(512);
  
  vKalmanInit (&xRollFilter);
  vKalmanInit (&xPitchFilter);
  xRollFilter.fAngle = 0;
  xPitchFilter.fAngle = 0;
  xPrevTime = xTaskSystemTime();

  for (;;) {

    vUpdate ();
    delay_ms(10);
  }
  return 0;
}

/* internal public functions ================================================ */

// -----------------------------------------------------------------------------
void 
vStdOutInit (void) {

  iLcdInit();
  ucLcdBacklightSet (63);
  stdout = &xLcd;
}

// -----------------------------------------------------------------------------
void 
vUpdate (void) {
  float xPitchRoll[3], xAcc[3], xRate[3];
  ticks_t xCurrentTime;

  iAccelero3dRead (xAcc);
  vAccelero3dAttitude (xPitchRoll, xAcc, LPF_CONSTANT);
  
  xCurrentTime = xTaskSystemTime();
  
  iGyro3dRead (xRate);

  xCurrentTime -= xPrevTime;
  vKalmanProcess (&xPitchFilter, xPitchRoll[0], xRate[0], xCurrentTime);
  vKalmanProcess (&xRollFilter, xPitchRoll[1], xRate[1], xCurrentTime);

  vLcdClear();
  printf ("%+.1f %+.1f %+.2f\n%+.1f %+.1f", (double)xAcc[0], (double)xAcc[1], (double)xAcc[2], 
          (double)xPitchFilter.fAngle, (double)xRollFilter.fAngle);
  xPrevTime = xCurrentTime;
}

/* ========================================================================== */
