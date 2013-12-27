/**
 * @file test_imu6d.c
 * @brief Test unitaire IMU6D
 * @author Copyright _ 2011-2012 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * @version $Id$
 * Revision History ---
 *    20120303 - Initial version by epsilonRT
 */
#include <stdio.h>
#include <math.h>

#include <avrio/delay.h>
#include <avrio/led.h>
#include <avrio/button.h>
#include <avrio/twi.h>
#include <avrio/task.h>
#include <avrio/mutex.h>
#include <avrio/accelero3d.h>
#include <avrio/gyro3d.h>
#include <avrio/imu6d.h>

#include <avrio/lufa/serial.h>

/* constants ================================================================ */
#define MUTEX_SAMPLE 0x80
#define MUTEX_FILTER 0x40

#define SAMPLE_INTERVAL 5
#define SAMPLES         4

//Gravity at Earth's surface in m/s/s
#define g0 9.812865328
//Convert from radians to degrees.
#define RADIAN_DEGREE_FACTOR (180.0 / M_PI)
//Convert from degrees to radians.
#define DEGREE_RADIAN_FACTOR (M_PI / 180.0)

/* private variables ======================================================== */
static xImu6dFilter xImu6d;
static xMutex xImu6dMutex;

/* internal public functions ================================================ */
void vStdOutInit (void);
void vImu6dTask (xTaskHandle xImu6dTaskHandle);
void vImu6dFilter (void);

/* main ===================================================================== */
int
main (void) {
  xTaskHandle xImu6dTaskHandle = 0;
  uint16_t usChars;

  vTwiInit();
  eTwiSetSpeed (400);
  vLedInit();
  vButInit();
  vStdOutInit();
  
  if (!iAccelero3dInit()) {
  
    printf("Accelero OK\nCalibrating...\n");
    iAccelero3dCalibrate(512);
  }
  else {
  
    printf("Accelero Error !\n");
  }
  if (!iGyro3dInit()) {
  
    printf("Gyro OK\nCalibrating...\n");
    iGyro3dCalibrate(512);
  }
  else {
  
    printf("Gyro Error !\n");
  }
  
  vImu6dInit (&xImu6d, SAMPLE_INTERVAL * SAMPLES, 0.3);
  xImu6dTaskHandle = xTaskCreate (xTaskConvertMs(SAMPLE_INTERVAL), vImu6dTask);
  vTaskStart (xImu6dTaskHandle);

  for (;;) {

    usChars = usSerialHit();
    if (usChars) {
      int c;

      while ((c = getchar()) > 0) {
      
        // Flush, nothing to do, but it is necessary to flush received chars
      }
    }
    
    if (xButGet (BUTTON_ALL_BUTTONS)) {
    
      iAccelero3dCalibrate(512);
      iGyro3dCalibrate(512);
      vImu6dReset (&xImu6d);
    }
    else {
    
      vImu6dFilter ();
      if (xMutexTryLockBit(&xImu6dMutex, MUTEX_FILTER) == 0) {

       /* printf ("%f,%f,%f\n",
                fImu6dRoll  (&xImu6d), 
                fImu6dPitch (&xImu6d), 
                fImu6dYaw   (&xImu6d));*/
        printf ("%e,%e,%e\n",
                xImu6d.xSEq.a, 
                xImu6d.xSEq.b, 
                xImu6d.xSEq.c);
      }
    
    }
    vSerialUsbTask();
  }
  return 0;
}

/* internal public functions ================================================ */

// -----------------------------------------------------------------------------
void 
vStdOutInit (void) {

  vSerialInit();
  stdout = &xUsbSerial;
  stdin = &xUsbSerial;
}

// -----------------------------------------------------------------------------
void 
vImu6dTask (xTaskHandle xImu6dTaskHandle) {

  vMutexUnlockBit (&xImu6dMutex, MUTEX_SAMPLE);
  vTaskStart (xImu6dTaskHandle);
}

// -----------------------------------------------------------------------------
void 
vImu6dFilter (void) {
  static uint8_t ucCounter;
  static float xRate[3], xAcc[3];
  
  if (xMutexTryLockBit(&xImu6dMutex, MUTEX_SAMPLE) == 0) {
  
    if (ucCounter < SAMPLES) {
      float xVect[3];
      
      iAccelero3dRead (xVect);
      vVector3fAdd (xAcc, xVect, xAcc);
      iGyro3dRead (xVect);
      vVector3fAdd (xRate, xVect, xRate);
      ucCounter++;
    }
    
    if (ucCounter >= SAMPLES) {
    
      vVector3fDivK (xAcc,  SAMPLES);
      vVector3fDivK (xRate, SAMPLES);
      vVector3fMulK (xAcc, g0);
      vVector3fMulK (xRate, DEGREE_RADIAN_FACTOR);
      vImu6dUpdateFilter (&xImu6d, xAcc, xRate);
      vImu6dComputeEuler (&xImu6d);
      vVector3fClear (xRate);
      vVector3fClear (xAcc);
      ucCounter = 0;
      vMutexUnlockBit (&xImu6dMutex, MUTEX_FILTER);
    }
  }
}

/* ========================================================================== */
