/**
 * @file test_temt.c
 * @brief Test unitaire capteur TEMT
 * @author Copyright © 2011-2014 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * @version $Id$
 * Revision History ---
 *    20140306 - Initial version by epsilonRT
 */
#include <avrio/led.h>
#include <avrio/delay.h>
#include <avrio/serial.h>
#include <avrio/temt.h>
#include "avrio-board-temt.h"

/* constants ================================================================ */
#define TEST_TITLE "\nTest unitaire capteur TEMT\n"

/*
 * Baudrate de la liaison série
 */
#define TEST_BAUDRATE 38400

/* internal public functions ================================================ */
void vStdOutInit (void);

/* main ===================================================================== */
int
main (void) {
  xTemtSensor xSensor;
  xTemtSetting xLumSettings[] = TEMT_LUM_SETTINGS;
  xTemtSetting xTempSetting = TEMT_TEMP_SETTING;
  xTemtRaw xRaw;
  xTemtValue xValue;

  vLedInit();
  vStdOutInit();
  vAdcInit();
  vAdcSetAutoscale (0);
  vTemtInit (&xSensor, 0, xLumSettings, 8, &xTempSetting);

  for (;;) {

    vTemtGetRaw (&xSensor, &xRaw);
    vTemtRawToValue (&xSensor, &xRaw, &xValue);
    printf ("Scale(%d)> Lum: R=%05u\tV=%.03f\tLux=%.02f * Temp: R=%05u\tT=%.02f\n",
      xSensor.xLumSensor.ucAdcScale,
      xRaw.usLum, ADC_MEASUREMENT(xRaw.usLum, 4.95), xValue.dLum,
      xRaw.usTemp, xValue.dTemp
      );

    vLedToggle (LED_LED1);
    delay_ms (1000);
  }

  return 0;
}

/* internal public functions ================================================ */

// -----------------------------------------------------------------------------
void
vStdOutInit (void) {

  vSerialInit (TEST_BAUDRATE / 100, SERIAL_DEFAULT + SERIAL_RW);
  stdout = &xSerialPort;
  printf (TEST_TITLE);
}
/* ========================================================================== */
