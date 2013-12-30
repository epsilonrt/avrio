/**
 * @file test_ledrgb.c
 * @brief Tests unitaires du module \ref ledrgb_module "Leds RGB".
 * @author Copyright © 2011-2012 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * @version $Id$
   ========================================================================== */
#include <avrio/delay.h>
#include <avrio/led.h>
#include <avrio/ledrgb.h>
#include <avr/pgmspace.h>

/* constants ================================================================ */
#define COLOR_LIST_SIZE 16

/* private variables ======================================================== */
static const uint32_t ulColorList[COLOR_LIST_SIZE] PROGMEM = {
  RGB_BLACK,
  RGB_WHITE,
  RGB_RED,
  RGB_LIME,
  RGB_BLUE,
  RGB_YELLOW,
  RGB_CYAN,
  RGB_MAGENTA,
  RGB_SILVER,
  RGB_GRAY,
  RGB_MAROON,
  RGB_OLIVE,
  RGB_GREEN,
  RGB_PURPLE,
  RGB_TEAL,
  RGB_NAVY
};
  
/* private functions ======================================================== */
// -----------------------------------------------------------------------------
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

// -----------------------------------------------------------------------------
static void
vTestLeds (xLedRgbMask xLed) {
  uint32_t ulColor;
  bool bNone = (xLed == LEDRGB_NO_LED);

  if (bNone)
    xLed = 1;
  
  for (uint8_t ucColor = 0; ucColor < COLOR_LIST_SIZE; ucColor++) {
  
    memcpy_P (&ulColor, &ulColorList[ucColor], sizeof(uint32_t));
    vLedRgbSetColor (xLed, ulColor);
    if (bNone)
      xLed <<= 1;
    vLedToggle (LED_LED1);
    delay_ms (50);
  }
}

/* internal public functions ================================================ */

/* main ===================================================================== */
int
main (void) {
  xLedRgbMask xLed;
  
  vLedInit();
  vAssert (iLedRgbInit() == 0);

  for (;;) {

    // Test 1: changement de couleur de toutes les leds
    vTestLeds (LEDRGB_ALL_LEDS);
    
    // Test 2: changement de couleur led par led
    xLed = 1;
    while (xLed) {
    
      vTestLeds (xLed);
      xLed <<= 1;    
    }
    
    // Test 3: une couleur par led
    vTestLeds (0);
    
    // Test 4: Variation de luminosité globale
    for (uint8_t ucDimming = 0; ucDimming < 255; ucDimming++) {
    
      vLedRgbSetGlobalDimming (ucDimming);
      vLedToggle (LED_LED1);
      delay_ms (50);
    }

    // Test 5: Clignotement global (période 1 s à 12.5 %)
    vLedRgbSetGlobalBlinking (1000, 32);
    delay_ms (SECONDS(10));
    
    // Test 6: Changement de mode de 8 leds sur 16
    vLedRgbSetMode (0x5555, MODE_BRIGHT);
    delay_ms (SECONDS(10));
    
    // Test 7: Test des erreurs
    vLedRgbSetMode (LEDRGB_ALL_LEDS, MODE_BLINK);
    vLedRgbSetGlobalDimming (255);
    delay_ms (SECONDS(10));
    vAssert (xLedRgbError() == 0);
    
    // Fin des tests
  }
  return 0;
}

/* ========================================================================== */
