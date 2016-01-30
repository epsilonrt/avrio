/*
 * Exemple capteur de pression HSC MAND015PASA5 (SPI - 5V - 15PSI)
 */
#include <avrio/led.h>
#include <avrio/delay.h>
#include <avrio/serial.h>
#include <avrio/twi.h>
#include <avrio/hsc.h>
#include <avrio/assert.h>

/* constants ================================================================ */
#define TEST_BAUDRATE 38400

void vLedAssert (int i);

/* main ===================================================================== */
int
main (void) {
  int iError;
  xHscSensor xSensor;
  xHscRaw xRaw;
  xHscValue xValue;

  vLedInit();
  /*
   * Init du terminal d'affichage des messages
   */
  vSerialInit (TEST_BAUDRATE / 100, SERIAL_DEFAULT + SERIAL_RW + SERIAL_RTSCTS);
  stdout = &xSerialPort;
  stderr = &xSerialPort;
  sei();
  
  printf ("\nTest unitaire HSC I2C\n");
  
  vTwiInit ();
  iError = eTwiSetSpeed (100);
  assert (iError == 0);
  iError = iHscInitTwiSensor (&xSensor,  0, 1600, HSC_DEFAULT_TWIADDR);
  assert (iError == 0);

  for (;;) {

    iError = iHscGetRaw (&xSensor, &xRaw);
    if (iError) {

      printf ("Sensor Error: %d\n", iError);
    }
    else {

      vHscRawToValue (&xSensor, &xRaw, &xValue);
      printf ("Press %.02f Temp %.02f\n", xValue.dPress, xValue.dTemp);
    }
    vLedToggle (LED_LED1);
    delay_ms (500);
  }

  return 0;
}

/* ========================================================================== */
