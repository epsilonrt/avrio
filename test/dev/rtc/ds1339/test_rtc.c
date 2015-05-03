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
 * @file test_rtc.c
 * @brief Tests unitaires du module \ref rtc_module "RTC DS1339".
 *
 * Revision History ------------------------------------------------------------
 *    20120402 - Initial version by epsilonRT
 */
#include <avrio/led.h>
#include <avrio/delay.h>
#include <avrio/rtc.h>
#include <avrio/rtc_ds1339.h>
#include <avrio/serial.h>
#include <avrio/term.h>
#include <stdio.h>
#include <string.h>
#include <avr/pgmspace.h>

/* constants ================================================================ */
#define TEST_BAUDRATE 115200
#define TEST_DS1339 (int)DS1339_BASE

/* private variables ======================================================== */
static xRtcTime xSetTime, xGetTime;

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
// -----------------------------------------------------------------------------
static void
prvvSetRtc (void) {

  printf_P(PSTR("\nDate (DDMMYYYY) ? "));
  xSetTime.ucDate = (uint8_t) iTermGetDec (stdin, 2);

  putchar ('/');
  xSetTime.ucMonth = (uint8_t) iTermGetDec (stdin, 2);

  putchar ('/');
  xSetTime.usYear = (uint16_t) iTermGetDec (stdin, 4);

  printf_P(PSTR("\nJour (1=Dim..7=Sam) ? "));
  xSetTime.ucWeekDay = (uint8_t) iTermGetDec (stdin, 1);

  printf_P(PSTR("\nHeure (HHMMSS) ? "));
  xSetTime.ucHour = (uint8_t) iTermGetDec (stdin, 2);

  putchar (':');
  xSetTime.ucMinute = (uint8_t) iTermGetDec (stdin, 2);

  putchar (':');
  xSetTime.ucSecond = (uint8_t) iTermGetDec (stdin, 2);
  putchar ('\n');
  
  vAssert (iRtcSetTime (&xSetTime) == 0);
  vAssert (iRtcGetTime (&xGetTime) == 0);
  vAssert (memcmp (&xSetTime, &xGetTime, sizeof(xRtcTime)) == 0);
}

// -----------------------------------------------------------------------------
static void
prvvPrintRtc (void) {

  vAssert (iRtcGetTime (&xGetTime) == 0);
  vAssert (iRtcPrintDate (&xGetTime) > 0);
  printf_P (PSTR(" - "));
  vAssert (iRtcPrintTime (&xGetTime) > 0);
  putchar('\n');
}

/* main ===================================================================== */
int
main (void) {
  eTwiStatus eError;
  uint8_t ucWaitTime = 10; // Temps d'attente en secondes
  
  vLedInit ();
  vSerialInit (TEST_BAUDRATE / 100, SERIAL_DEFAULT + SERIAL_RW);
  stdout = &xSerialPort;
  stdin = &xSerialPort;
  printf_P (PSTR("\n\nTest unitaire RTC DS1339\n"));
  
  vTwiInit ();
  eError = eTwiSetSpeed (100);
  vAssert (eError == TWI_SUCCESS);
  
  vAssert (iRtcInit (TEST_DS1339) == 0);

  printf_P(PSTR("Date courante: ")); prvvPrintRtc();
  printf_P(PSTR("Modification Date ? (y/n) "));
  while (ucWaitTime--) {
    uint8_t ucCount = 10;

    while ((ucCount--) && (usSerialHit() == 0)) {
      
      // Boucle en attente appui d'une touche
      delay_ms(100);
    }

    if (usSerialHit() != 0) {
      char cKey;
      
      cKey = getchar();
      if ((cKey == 'y') || (cKey == 'Y')) {
      
        prvvSetRtc();
      }
      break;
    }
    putchar('.');
  }
  
  putchar('\n');
  for (;;) {

    if (usSerialHit() != 0) {
    
      (void) getchar(); // flush last char
      prvvSetRtc();
    }
    prvvPrintRtc();
    vLedToggle (LED_LED1);
    delay_ms (1000);
  }
  return 0;
}

/* ========================================================================== */
