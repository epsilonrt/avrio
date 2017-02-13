/*
 * Exemple accès carte MMC/SD
 *
 * Ce test écrit écrit un octet dans un registre et relit cet octet en
 * vérifiant que la valeur lue est identique à celle écrite. \n
 * Si cela fonctionne, la LED1 clignote régulièrement, sinon elle flashe
 * rapidement.
 */
#include <avrio/led.h>
#include <avrio/spi.h>
#include <avrio/delay.h>

/* constants ================================================================ */
#define TEST_BYTE 0x55

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

/* main ===================================================================== */
int
main (void) {
  uint8_t ucByte;

  vLedInit();
  vSpiSetSsAsOutput();
  vSpiMasterInit (SPI_DIV2);

  for (;;) {

    vSpiSetSs();
    vSpiMasterWrite (TEST_BYTE);
    ucByte = ucSpiMasterRead();
    vSpiClearSs();

    vAssert (ucByte == TEST_BYTE);
    vLedToggle (LED_LED1);
    delay_ms (500);
  }

  return 0;
}

/* ========================================================================== */
