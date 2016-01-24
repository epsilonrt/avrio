/*
 * Ce test écrit une suite d'octets de valeurs croissantes (0x00 à 0xFF) dans
 * le secteur de numéro TEST_SECTOR, puis relit ce secteur en vérifiant que
 * les données lues correpondent à celles écrites.
 * Si cela fonctionne, la LED1 clignote régulièrement, sinon elle flashe
 * rapidement.
 */
#include <string.h>
#include <avrio/led.h>
#include <avrio/button.h>
#include <avrio/mmc.h>
#include <avrio/spi.h>
#include <avrio/delay.h>

/* constants ================================================================ */
#define TEST_SECTOR 0

/* private variables ======================================================== */
static uint8_t ucBuffer[MMC_SECTOR_SIZE];

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
  uint8_t * pucBuffer = ucBuffer;
  uint16_t usIndex;
  
  vLedInit();
  vButInit();
  vMmcInit();
  vSpiSetSsAsInput();
  vSpiMasterInit (SPI_DIV2);

  for (;;) {
  
    while (xMmcIsDetected() == false) {
    
      vLedToggle (LED_LED1);
      delay_ms(2000);
    }
    vAssert (iMmcOpen() == MMC_SUCCESS);
    
    // Test 1 - Ecriture d'un secteur
    pucBuffer = ucBuffer;
    // Init. du buffer
    for (usIndex = 0; usIndex < MMC_SECTOR_SIZE; usIndex++) {
    
      *pucBuffer++ = (uint8_t) usIndex;
    }
    // Ecriture
    pucBuffer = ucBuffer;
    vAssert (iMmcWriteSector (TEST_SECTOR, pucBuffer, 1));

    // Test 2 - Lecture d'un secteur
    pucBuffer = ucBuffer;
    // Clear du buffer
    memset (pucBuffer, 0, MMC_SECTOR_SIZE);
    // Lecture
    vAssert (iMmcReadSector  (TEST_SECTOR, pucBuffer, 1));
    // Vérif. du buffer
    pucBuffer = ucBuffer;
    for (usIndex = 0; usIndex < MMC_SECTOR_SIZE; usIndex++) {
    
      vAssert (*pucBuffer++ == (uint8_t) usIndex);
    }
    
    if (xButGet(BUTTON_BUTTON1)) {
      
      // Arrêt du test...
      vLedClear(LED_LED1);
      while (xButGet(BUTTON_BUTTON1))
        ; // Attente relachement
      delay_ms(200);
      vMmcClose();
      while (xButGet(BUTTON_BUTTON1) == 0)
        ; // Attente appui
    }
    vLedToggle (LED_LED1);
    delay_ms (500);
  }
  
  return 0;
}

/* ========================================================================== */
