#include <stdio.h>
#include <stdlib.h>
#include <avr/pgmspace.h>

#include <avrio/delay.h>
#include <avrio/led.h>
#include <avrio/lcd.h>
#include <avrio/button.h>
#include <avrio/twi.h>
#include <avrio/task.h>
#include <avrio/gyro3d.h>

#include <avrio/lufa/serial.h>

/* constants ================================================================ */
#define LPF_CONSTANT 0.1

#define DEG_CHAR \xD7
#define CALIBRATE_SAMPLES 128
#define CALIBRATE_SAMPLE_DELAY  20

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
void vCalibrateGyro (void);
void vAssert (bool test);
void vLcdAssert (const char * pcErrorMsg, int8_t iError);

/* main ===================================================================== */
int
main (void) {
  static float xEuler[3];
  float xRate[3];
  uint16_t usChars = 0;
  int8_t iError;

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
  fprintf_P (stderr, PSTR("Gyro3d Test\nPress any key..."));
  while (xButGet (BUTTON_ALL_BUTTONS) == 0)
    vSerialUsbTask();

  vLcdClear();
  iError = iGyro3dInit();
  //                0123456789012345
  vLcdAssert (PSTR("Gyro Error:%d\n"), iError);
  if (iError == GYRO3D_NOT_CALIBRATED) {
  
    vCalibrateGyro();
    vGyro3dAttidudeClear (xEuler);
  }
  
   
  for (;;) {

    usChars = usSerialHit();
    if (usChars) {
      int c;

      while ((c = getchar()) > 0) {
      
        // Flush, nothing to do, but it is necessary to flush received chars
      }
    }
    
    if (xButGet (BUTTON_BUTTON1)) {
    
      vCalibrateGyro();
      vGyro3dAttidudeClear (xEuler);
    }
    else {
      ticks_t xRateTime;
      static uint8_t ucCount;
      int iError;
      
      xRateTime = xTaskSystemTime();
      iError = iGyro3dRead (xRate);

      //                0123456789012345
      vLcdAssert (PSTR("Gyro Error:%d\n"), iError);
      printf_P (PSTR("%f,%f,%f"), xRate[0], xRate[1], xRate[2] );
      
      if (iGyro3dAttitude (xEuler, xRate, xRateTime, LPF_CONSTANT) == 0) {
        static int iYaw;
        
        iYaw = (int) xEuler[2];
        
        vLcdClearLine (1);
        fprintf_P (stderr, PSTR("%cYaw=%d"), (ucCount++ & _BV(1) ? '|' : '-'), iYaw);
        printf_P (PSTR(",%f,%f,%f\n"), xEuler[0], xEuler[1], xEuler[2]);
      }
    }

    vSerialUsbTask();
    delay_ms(10);
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
vCalibrateGyro (void) {
  uint16_t usSample;
  uint8_t ucIndex;
  int xRate[3];
  int32_t lSum[3] = {0, 0, 0};
  int iZero[3];
  int iError = 0;
  
  vGyro3dZero (iZero);
  vLcdClearLine(1);
  fprintf_P (stderr, PSTR("%+.2d %+.2d %+.2d"), iZero[0],  iZero[1],  iZero[2]);
  for (usSample = 0; usSample < CALIBRATE_SAMPLES; usSample++) {
  
    iError = iGyro3dReadRaw (xRate);
    //                0123456789012345
    vLcdAssert (PSTR("Gyro Error:%d\n"), iError);
    for (ucIndex = 0; ucIndex < 3; ucIndex++) {
    
      lSum[ucIndex] += xRate[ucIndex];
    }

    vLcdBargraph (usSample, CALIBRATE_SAMPLES, xLcdWidth()-12);
    delay_ms (CALIBRATE_SAMPLE_DELAY);
  }
  
  for (ucIndex = 0; ucIndex < 3; ucIndex++) {
  
    lSum[ucIndex] /= CALIBRATE_SAMPLES;
  }
  
  vVector3iCopyLong (iZero, lSum);
  vGyro3dSetZero (iZero);
  vGyro3dZero (iZero);
  
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
