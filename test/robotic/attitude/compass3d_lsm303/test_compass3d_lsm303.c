#include <stdio.h>
#include <avr/pgmspace.h>
#include <avrio/delay.h>
#include <avrio/led.h>
#include <avrio/lcd.h>
#include <avrio/button.h>
#include <avrio/twi.h>
#include <avrio/compass3d.h>
#include <avrio/accelero3d.h>
#include <math.h>

#include <avrio/lufa/serial.h>

/* constants ================================================================ */
#define LPF_CONSTANT 0.1
#define MAG_DECL 0
#define CALIBRATE_SAMPLES 128
#define CALIBRATE_SAMPLE_DELAY 20

/* internal public functions ================================================ */
void vCalibrateAcc (void);
void vCalibrateCompass (void);
void vAssert (bool test);
void vLcdAssert (const char * pcErrorMsg, int8_t iError);

/* extern variables ========================================================= */
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

/* main ===================================================================== */
int
main (void) {
  float xMag[3], xAcc[3], xAtt[3] = { 0, 0, 0.5 };
  uint16_t usChars;
  int iHeading;
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
  fprintf_P (stderr, PSTR("Compass3d Test\nPress any key..."));
  while (xButGet (BUTTON_ALL_BUTTONS) == 0)
    vSerialUsbTask();
  while (xButGet (BUTTON_ALL_BUTTONS) != 0)
    vSerialUsbTask();

  vLcdClear();
  iError = iAccelero3dInit();
  //                0123456789012345
  vLcdAssert (PSTR("Acc. Error:%d\n"), iError);
  if (iError == ACCELERO3D_NOT_CALIBRATED) {
  
    vCalibrateAcc();
  }

  vLcdClear();
  iError = iCompass3dInit();
  //                0123456789012345
  vLcdAssert (PSTR("Comp. Error:%d\n"), iError);
  if (iError == COMPASS3D_NOT_CALIBRATED) {
  
    vCalibrateCompass();
  }
  vLcdClear();
  
  for (;;) {

    usChars = usSerialHit();
    if (usChars) {
      int c;

      while ((c = getchar()) > 0) {
      
        // Flush, nothing to do, but it is necessary to flush received chars
      }
    }
    
    if (xButGet (BUTTON_ALL_BUTTONS) == BUTTON_NO_BUTTON) {
    
      static uint8_t ucCount;
      
      iError  = iCompass3dRead  (xMag);
      //                0123456789012345
      vLcdAssert (PSTR("Comp. Err:%d\n"), iError);
      
      iError  = iAccelero3dRead (xAcc);
      //                0123456789012345
      vLcdAssert (PSTR("Acc. Err:%d\n"), iError);
      
      vAccelero3dAttitude (xAtt, xAcc, LPF_CONSTANT);
      iHeading = iCompass3dHeading(xAtt, xMag, MAG_DECL);
      vLcdClearLine (1);
      fprintf_P (stderr, PSTR("%cHead=%d"), (ucCount++ & _BV(1) ? '|' : '-'), iHeading);        
      printf ("%f,%f,%f,%d", (double)xMag[0], (double)xMag[1], (double)xMag[2], iHeading);
    }
    else {
    
      // Bouton appuyé -> calibration
      vCalibrateAcc();
      vCalibrateCompass();
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
vCalibrateAcc (void) {
  uint16_t usSample;
  uint8_t ucIndex;
  int xAcc[3];
  int32_t lSum[3] = {0, 0, 0};
  int iZero[3];
  int iError;
  
  vAccelero3dZero (iZero);
  vLcdClearLine(1);
  fprintf_P (stderr, PSTR("%+.2d %+.2d %+.2d"), iZero[0],  iZero[1],  iZero[2]);
  for (usSample = 0; usSample < CALIBRATE_SAMPLES; usSample++) {
  
    iError = iAccelero3dReadRaw (xAcc);
    vLcdAssert (PSTR("Cal. Error:%d\n"), iError);
    for (ucIndex = 0; ucIndex < 3; ucIndex++) {
    
      lSum[ucIndex] += xAcc[ucIndex];
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
vCalibrateCompass (void) {
  xCompass3dCalibration xCalibration;
  int iError;
  
  vLcdClear();
  iError = iCompass3dCalibrate (&xLcd, BUTTON_ALL_BUTTONS);
  vLcdAssert (PSTR("Cal. Error:%d\n"), iError);
  
  vCompass3dCalibration (&xCalibration);
  vLcdClear();
  fprintf_P (stderr, PSTR("%+d,%+d,%+d\n%+d,%+d,%+d\n"), 
              xCalibration.iMin[0], xCalibration.iMin[1], xCalibration.iMin[2], 
              xCalibration.iMax[0], xCalibration.iMax[1], xCalibration.iMax[2]);
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
