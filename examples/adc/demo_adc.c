/**
 * @file demo_adc.c
 * @brief Exemple ADC 
 * @author Copyright © 2011-2012 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * @version $Id$
 * Revision History ---
 *    20120303 - Initial version by epsilonRT
 */
#include <avrio/led.h>
#include <avrio/delay.h>
#include <avrio/adc.h>
#include <stdio.h>

/* ATTENTION
 * avrio-board-adc.h doit être adapté en fonction de la fréquence d'horloge
 * choisie et en fonction de la tension de référence choisie.
 */
 
/* constants ================================================================ */
//#define STDOUT_LCD
#define STDOUT_SERIAL
#define TEST_BAUDRATE 115200

/* internal public functions ================================================ */
void vStdOutInit (void);
void vPrintValue (uint8_t ucIndex, uint16_t usAdc);

#define ADC_CHAN_QUANTITY  4
static const uint8_t ucAdcChan[ADC_CHAN_QUANTITY] = {0, 1, 2, 3};

/* main ===================================================================== */
int
main (void) {
  uint16_t usAdc;
  uint8_t ucIndex;
  
  vStdOutInit();
  vAdcInit();
   
  for (;;) {

    for (ucIndex = 0; ucIndex < ADC_CHAN_QUANTITY; ucIndex++) {
    
      usAdc  = usAdcReadAverage (ucAdcChan[ucIndex]);
      vPrintValue (ucIndex, usAdc);      
    }
    vLedToggle (LED_LED1);
    delay_ms (50);
  }
  return 0;
}

#if defined(STDOUT_LCD)
#include <avrio/lcd.h>

// -----------------------------------------------------------------------------
void 
vStdOutInit (void) {

  iLcdInit();
  ucLcdBacklightSet (63);
  stdout = &xLcd;
}

// -----------------------------------------------------------------------------
void 
vPrintValue (uint8_t ucIndex, uint16_t usAdc) {
  static xLcdCoord xLcdX, xLcdY;

      if (ucIndex == 0) {
      
        xLcdY = 0;
        xLcdX = 0;
      }
      else {
      
        xLcdX += 8;
        if (xLcdX >= xLcdWidth()) {
        
          xLcdY++;
          xLcdX = 0;
        }
      }
      
      vLcdGotoXY (xLcdX, xLcdY);
      printf ("A%01d=%04d", ucIndex, usAdc);
}
#elif defined(STDOUT_SERIAL)
#include <avrio/serial.h>

// -----------------------------------------------------------------------------
void 
vStdOutInit (void) {

  vSerialInit (TEST_BAUDRATE / 100, SERIAL_DEFAULT + SERIAL_RW);
  stdout = &xSerialPort;
  printf ("\nExemple ADC\n");
}

// -----------------------------------------------------------------------------
void 
vPrintValue (uint8_t ucIndex, uint16_t usAdc) {

      if ((ucIndex == 0) || ((ucIndex % 4) == 0)) {
      
        putchar('\n');
      }
      else {
      
        putchar(' ');
      }
      printf ("A%01d=%04d", ucIndex, usAdc);
}
#endif
/* ========================================================================== */
