/*
 * Exemple capteur de pression HSCMAND015PASA5
 */
#include <avrio/led.h>
#include <avrio/delay.h>
#include <avrio/serial.h>
#include <avrio/spi.h>
#include <avrio/hsc.h>

/* constants ================================================================ */
#define TEST_BAUDRATE 38400

/* internal public functions ================================================ */
void vStdOutInit (void);
void vSensorSelect (bool bEnable);
void vAssert (bool bTest);

/* main ===================================================================== */
int
main (void) {
  int iError;
  xHscSensor xSensor;
  xHscRaw xRaw;
  xHscValue xValue;

  vLedInit();
  vStdOutInit();
  vSpiSetSsAsOutput();        // Master Only.
  vSpiMasterInit (SPI_DIV16); // Fsclk 800 KHz max.
  iHscInitSpiSensor (&xSensor,  PSI_TO_PA(0)/100.0f,
                                PSI_TO_PA(15)/100.0f, 0,
                                vSensorSelect);

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

/* internal public functions ================================================ */

// -----------------------------------------------------------------------------
void
vStdOutInit (void) {

  vSerialInit (TEST_BAUDRATE / 100, SERIAL_DEFAULT + SERIAL_RW);
  stdout = &xSerialPort;
  printf ("\nTest unitaire HSC\n");
}

// -----------------------------------------------------------------------------
void vSensorSelect (bool bEnable) {

  if (bEnable)
    vSpiSetSs();
  else
    vSpiClearSs();
}

/* ========================================================================== */
