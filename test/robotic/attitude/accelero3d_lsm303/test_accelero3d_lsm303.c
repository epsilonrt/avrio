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
 * @file test_accelero.c
 * @brief Test unitaire ACCELERO
 *
 * Revision History ------------------------------------------------------------
 *    20120303 - Initial version by epsilonRT
 */
#include <stdio.h>
#include <avr/pgmspace.h>

#include <avrio/delay.h>
#include <avrio/led.h>
#include <avrio/lcd.h>
#include <avrio/button.h>
#include <avrio/twi.h>
#include <avrio/accelero3d.h>
#include <math.h>
#include <avrio/lufa/serial.h>

/* constants ================================================================ */
#define LPF_CONSTANT 0.1
#define CALIBRATE_SAMPLES 128
#define CALIBRATE_SAMPLE_DELAY 20

/* macros =================================================================== */
#define TILT_PITCH(tilt) ((tilt) * 127.0 / 90.0)

/* extern variables ========================================================= */
/* Temps entre 2 mesures en ms */
extern const uint16_t usAccelero3dLoopDelay;
/* Valeur du Lsb du gyro */
extern const float fAccelero3dLsb;

/* private variables ======================================================== */
static const PROGMEM uint8_t pucBarPatterns[] = {
  0x1F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, /* 0. ./images/bloc0.png */
  0x1F, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x1F, /* 1. ./images/bloc1.png */
  0x1F, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x1F, /* 2. ./images/bloc2.png */
  0x1F, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1F, /* 3. ./images/bloc3.png */
  0x1F, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1F, 0x1F, /* 4. ./images/bloc4.png */
  0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F  /* 5. ./images/bloc5.png */
};

/* internal public functions ================================================ */
void vCalibrateAccelero (void);
void vAssert (bool test);
void vLcdAssert (const char * pcErrorMsg, int8_t iError);

/* main ===================================================================== */
int
main (void) {
  float iAcc[3];
  float xEuler[3]; // z constante du filtre passe-bas
  uint16_t usChars;
  int iError;
  
  vLedInit();
  // Leds dispo pour Assert()
  
  vTwiInit ();
  iError = eTwiSetSpeed (400);
  vAssert  (iError == TWI_SUCCESS);

  iError = iLcdInit();
  vAssert (iError == 0);

  ucLcdBacklightSet (32);
  stderr = &xLcd;
  vLcdGotoXY (12, 1);
  vLcdBargraphInit (pucBarPatterns);
  // Lcd dispo pour test

  vSerialInit();
  stdout = &xUsbSerial;
  stdin = &xUsbSerial;

  vButInit();

  vLcdClear();
  fprintf_P (stderr, PSTR("Accelero3d Test\nPress any key..."));
  while (xButGet (BUTTON_ALL_BUTTONS) == 0)
    vSerialUsbTask();

  vLcdClear();
  iError = iAccelero3dInit();
  //                0123456789012345
  vLcdAssert (PSTR("Acc. Error:%d\n"), iError);
  if (iError == ACCELERO3D_NOT_CALIBRATED) {
  
    vCalibrateAccelero();
  }
   
  for (;;) {

    usChars = usSerialHit();
    if (usChars) {
      int c;

      while ((c = getchar()) > 0) {
      
        // Flush, nothing to do, but it is necessary to flush received chars
      }
    }
    
    if (xButGet (BUTTON_ALL_BUTTONS)) {
    
      vCalibrateAccelero();
    }
    else {
      static uint8_t ucCount;
      int iPitch;
      int iRoll;

      iError = iAccelero3dRead (iAcc);
      //                0123456789012345
      vLcdAssert (PSTR("Acc. Error:%d\n"), iError);

      printf_P (PSTR("%f,%f,%f"), iAcc[0], iAcc[1], iAcc[2]);
      
      vAccelero3dAttitude (xEuler, iAcc, LPF_CONSTANT);
      iPitch = (int) TILT_PITCH (xEuler[1]);
      iRoll  = (int) TILT_PITCH (xEuler[0]);
      
      printf_P (PSTR(",%d,%d\n"), iRoll, iPitch);
      vLcdClearLine (1);
      fprintf_P (stderr, PSTR("%cR=%+.3d"), (ucCount++ & _BV(1) ? '|' : '-'), iRoll);
      vLcdGotoXY (8,1);
      fprintf_P (stderr, PSTR("P=%+.3d"), iPitch);
    }
      
    vSerialUsbTask();
    delay_ms(50);
  }
  return 0;
}

/* internal public functions ================================================ */
// -----------------------------------------------------------------------------
void
vAssert (bool test) {

  if (test == false) {

    for (;;) {

      vLedSet (LED_LED3);
      delay_ms (25);
      vLedClear (LED_LED3);
      delay_ms (100);
    }
  }
}

// -----------------------------------------------------------------------------
void 
vCalibrateAccelero (void) {
  uint16_t usSample;
  uint8_t ucIndex;
  int iAcc[3];
  int32_t lSum[3] = {0, 0, 0};
  int iZero[3];
  
  vAccelero3dZero (iZero);
  vLcdClearLine(1);
  fprintf_P (stderr, PSTR("%+.2d %+.2d %+.2d"), iZero[0],  iZero[1],  iZero[2]);
  for (usSample = 0; usSample < CALIBRATE_SAMPLES; usSample++) {
  
    iAccelero3dReadRaw (iAcc);
    for (ucIndex = 0; ucIndex < 3; ucIndex++) {
    
      lSum[ucIndex] += iAcc[ucIndex];
    }

    vLcdBargraph (usSample, CALIBRATE_SAMPLES, xLcdWidth()-12);
    delay_ms (CALIBRATE_SAMPLE_DELAY);
  }
  
  for (ucIndex = 0; ucIndex < 3; ucIndex++) {
  
    lSum[ucIndex] /= CALIBRATE_SAMPLES;
  }
  
  lSum[2] -= (int)round(1.0 / fAccelero3dLsb);
  vVector3iCopyLong (iZero, lSum);
  vAccelero3dSetZero (iZero);
  vAccelero3dZero (iZero);
  
  //---------------
  vLcdClearLine(1);
  fprintf_P (stderr, PSTR("%+.2d %+.2d %+.2d"), iZero[0],  iZero[1],  iZero[2]);
  while (xButGet (BUTTON_ALL_BUTTONS) == 0)
    vSerialUsbTask();
  while (xButGet (BUTTON_ALL_BUTTONS) != 0)
    vSerialUsbTask();
}

// -----------------------------------------------------------------------------
void 
vLcdAssert (const char * pcErrorMsg, int8_t iError) {

  if (iError < 0) {
  
    vLcdClear();
    fprintf_P (stderr, pcErrorMsg, iError);
    for(;;); // STOP
  }
}

/* ========================================================================== */
