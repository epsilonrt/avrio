#include <avrio/led.h>
#include <avrio/delay.h>
#include <avrio/crc.h>
#include <string.h>

#define TEST_STR "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define TEST_CRC 0xC452

/* private functions ======================================================== */
// ------------------------------------------------------------------------------
static void
vAssert (bool test) {

  if (test == false) {

    for (;;) {

      vLedSet (LED_LED1);
      delay_ms (50);
      vLedClear (LED_LED1);
      delay_ms (150);
    }
  }
}

/* internal public functions ================================================ */
int
main (void) {
  uint16_t usCrc = CRC_CCITT_INIT_VAL;

  vLedInit ();

  for (;;) {

    usCrc = usCrcCcitt (CRC_CCITT_INIT_VAL, TEST_STR, strlen (TEST_STR));
    vAssert (usCrc == TEST_CRC);
    vLedToggle (LED_LED1);
    delay_ms (1000);
  }
  return 0;
}

/* ========================================================================== */
