#include <avrio/led.h>
#include <avrio/delay.h>
#include <avrio/serial.h>
#include <avrio/tcapture.h>

/* constants ================================================================ */
#define TIM TCAPTURE1
#define TEST_TSL230
#define TEST_BAUDRATE     38400
#define TEST_SETUP        (SERIAL_DEFAULT + SERIAL_RW)

#ifdef TEST_TSL230
#include <avrio/tsl230.h>
#include "avrio-board-tsl230.h"
#endif

/* private variables ======================================================== */
static double dFreq;
static uint16_t usValue;

/* main ===================================================================== */
int
main (void) {

  vLedInit ();
  vSerialInit (TEST_BAUDRATE / 100, TEST_SETUP);
  stdout = &xSerialPort;

#ifdef TEST_TSL230
  vTsl230PinInit();
  vTsl230PinSetSensitivity (eTsl230Sensitivity10);
  vTsl230PinSetScale (eTsl230Scale1);
#endif

  vTCaptureInit();
  vTCaptureSetNoiseFilter (TIM, true);
  vTCaptureSetPrescaler (TIM, eTCaptureDiv64);
  sei();
  printf ("Timer Capture Test\nValue\tFreq\n");

  for (;;) {

    if (bTCaptureIsComplete (TIM)) {

      usValue = usTCaptureValue (TIM);
      dFreq = dTCaptureValueToFreq (TIM, usValue);
      printf ("%u\t%.3f\n", usValue, dFreq);
      delay_ms (100);
      vTCaptureStart (TIM);
    }
    vLedToggle (LED_LED1);
  }
  return 0;
}
/* ========================================================================== */
