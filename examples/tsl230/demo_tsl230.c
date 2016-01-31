/*
 * Démonstration utilisation capteur TSL230
 */
#include <util/atomic.h>
#include <avrio/led.h>
#include <avrio/delay.h>
#include <avrio/serial.h>
#include <avrio/tsl230.h>

/* constants ================================================================ */
#define TEST_BAUDRATE     38400
#define TEST_SETUP        (SERIAL_DEFAULT + SERIAL_RW)

/* private variables ======================================================== */
static double dFreq, dIrradiance;

/* main ===================================================================== */
int
main (void) {

  vLedInit ();
  ATOMIC_BLOCK (ATOMIC_FORCEON) {
    vSerialInit (TEST_BAUDRATE / 100, TEST_SETUP);
    stdout = &xSerialPort;
    vTsl230Init();
    vTsl230SetSensitivity (eTsl230Sensitivity10);
    vTsl230SetScale (eTsl230Scale1);
    vTsl230SetWindow (1000);
  }
  printf ("Tsl230 Demo\nScale:%d\nSensitivity:%d\nfo(Hz);Ee(uW/cm2);Ee(W/m2)\n",
          eTsl230GetScale(), eTsl230GetSensitivity());

  for (;;) {

    if (bTsl230IsComplete()) {

      dFreq = dTsl230Freq();
      dIrradiance = dTsl230FreqToIrradiance (dFreq);
      if (dIrradiance >= 0) {
        printf ("%.1f;%.1f;%.1f\n", dFreq, dIrradiance, dIrradiance / 100);
        delay_ms (100);
      }
      vTsl230Start();
    }
  }
  return 0;
}
/* ========================================================================== */
