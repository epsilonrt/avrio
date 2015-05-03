/**
 * Copyright © 2011-2015 Pascal JEAN aka epsilonRT. All rights reserved.
 *
 * This file is part of AvrIO.
 *
 * AvrIO is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * AvrIO is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with AvrIO.  If not, see <http://www.gnu.org/licenses/lgpl.html>
 *
 * @file test_hsc.c
 * @brief Exemple capteur de pression HSCMAND015PASA5
 *
 *
 * Copyright © 2011-2014 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * Revision History ------------------------------------------------------------
 *    20140306 - Initial version by epsilonRT
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
