#include <avrio/led.h>
#include <avrio/delay.h>
#include <avrio/serial.h>
#include <avrio/icounter.h>

/* constants ================================================================ */
#define TEST_BAUDRATE     38400
#define TEST_SETUP        (SERIAL_DEFAULT + SERIAL_RW)
#define TEST_INT          INT1

/* private variables ======================================================== */
static double dFreq;

/* main ===================================================================== */
int
main (void) {
  xICounter xMyMeter;
  xICounter * fm = &xMyMeter;

  vLedInit ();
  ATOMIC_BLOCK (ATOMIC_FORCEON) {
    vSerialInit (TEST_BAUDRATE / 100, TEST_SETUP);
    stdout = &xSerialPort;
    vICounterInit (fm, TEST_INT);
    vICounterSetWindow (fm, 1000);
  }
  printf ("Frequency Meter Test\nWindow=%u ms\n", fm->usWindow);

  for (;;) {

    if (bICounterIsComplete (fm)) {
      dFreq = dICounterFreq (fm);
      printf ("%.1f\n", dFreq);
      delay_ms (100);
      vICounterStart (fm);
    }
  }
  return 0;
}
/* ========================================================================== */
