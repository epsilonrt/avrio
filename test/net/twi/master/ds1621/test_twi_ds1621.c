/**
 * @file test_twi_ds1621.c
 * @brief Test unitaire DS1621
 * @author Copyright © 2011-2012 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * @version $Id$
 * Revision History ---
 *    20120303 - Initial version by epsilonRT
 */
#include <avrio/led.h>
#include <avrio/delay.h>
#include <avrio/ds1621.h>
#include <avrio/serial.h>
#include <avrio/term.h>
#include <stdio.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

/* ========================================================================== */

/* constants ================================================================ */
#define TEST_BAUDRATE 38400

#define TEST_DS1621 DS1621_BASE

#define LED_PULSE LED_LED1
#define LED_TH LED_LED2

#define TH  PD4
#define TH_PORT PORTD
#define TH_PIN  PIND
#define TH_DDR  DDRD

#define TH_vect PCINT2_vect
#define TH_FLAG PCIE2
#define TH_MSK  PCINT20

/* private functions ======================================================== */

// ------------------------------------------------------------------------------
ISR(TH_vect) {

  if (bit_is_set(TH_PIN, TH)) {

    vLedSet(LED_TH);
  }
  else  {

    vLedClear(LED_TH);
  }
}

// ------------------------------------------------------------------------------
static inline void
vThInit(void) {

  TH_PORT &=  ~_BV(TH); /* no pull-up res */
  TH_DDR  &=  ~_BV(TH);
  PCMSK2 |= _BV(TH_MSK);
  PCICR  |= _BV(TH_FLAG);
  sei();
}

// ------------------------------------------------------------------------------
static void
vAssert (bool test) {

  if (test == false) {

    for (;;) {

      vLedSet (LED_PULSE);
      delay_ms (50);
      vLedClear (LED_PULSE);
      delay_ms (150);
    }
  }
}

// ------------------------------------------------------------------------------
static void
vAssertLastError (void) {
  eTwiStatus eError;

  eError = eDs1621LastError();
  vAssert (eError == TWI_SUCCESS);
}

/* main ===================================================================== */
int
main (void) {
  eTwiStatus eError;
  int16_t iTemp, iTh = 0, iTl = 0, iT;
  uint8_t ucCounter, ucSlope, ucStatus;
  bool xIsDone, xThOrTlChanged = false;
  float fTemp;

  vLedInit ();
  vThInit();
  vSerialInit (TEST_BAUDRATE / 100, SERIAL_DEFAULT + SERIAL_RW);
  stdout = &xSerialPort;
  stdin = &xSerialPort;
  printf_P (PSTR("\nTest unitaire DS1621\n"));

  vTwiInit ();
  eError = eTwiSetSpeed (100);
  vAssert (eError == TWI_SUCCESS);

  vDs1621Init (TEST_DS1621, ONESHOT | POL);
  vAssertLastError();

  ucStatus = ucDs1621GetStatus (TEST_DS1621);
  vAssertLastError();
  vAssert ((ucStatus & (ONESHOT | POL)) == (ONESHOT | POL));

  for (;;) {


    // Test 1 - Mesure One shot
    // Le temps d'allumage LED_PULSE correspond au temps de mesure
    // Mesure ~ 560 ms pour 750 max.
    vLedSet (LED_PULSE);
    vDs1621Start(TEST_DS1621);
    vAssertLastError();

    do {

      xIsDone = xDs1621IsDone(TEST_DS1621);
      vAssertLastError();
      // avrSleep();
    } while (xIsDone == false);
    vLedClear (LED_PULSE);

    // Test 2 - Lecture mesure
    iTemp = iDs1621GetTemp (TEST_DS1621);
    vAssertLastError();
    printf_P (PSTR("\tRead Temp. = %.1f C\n"), iTemp / 10.0);

    // Test 3 - Correction mesure avec COUNTER et SLOPE
    // cf page 4 du datasheet
    ucCounter = ucDs1621GetCounter (TEST_DS1621);
    vAssertLastError();

    ucSlope = ucDs1621GetSlope (TEST_DS1621);
    vAssertLastError();

    fTemp = ((float)iTemp / 10.0) - 0.25 + (float)(ucSlope - ucCounter) / (float)ucSlope;
    printf_P (PSTR("\tPrecise Temp. = %.2f C\n"), fTemp);

    // Test 4 - Lecture des températures de seuils haut et bas
    iTl = iDs1621GetTl (TEST_DS1621);
    vAssertLastError();
    iTh = iDs1621GetTh (TEST_DS1621);
    vAssertLastError();
    printf_P (PSTR("TL Temp.   = %.1f C\tTH Temp. = %.1f C\n"), iTl / 10.0, iTh / 10.0);

    // Test 5 - Lecture flags THF et THL
    ucStatus = ucDs1621GetStatus (TEST_DS1621);
    vAssertLastError();
    printf_P (PSTR("TLF        = %d\t\tTHF      = %d\n\n"), (ucStatus & TLF ? 1 : 0), (ucStatus & THF ? 1 : 0));


    // Test 6 - Clear des flags THF et THL
    vDs1621ClrFlags (TEST_DS1621, TAF); // Clear all flags
    vAssertLastError();
    ucStatus = ucDs1621GetStatus (TEST_DS1621);
    vAssertLastError();
    vAssert ((ucStatus & TAF) == 0);

    if (usSerialHit()) {

      (void) getchar();
      printf_P(PSTR("\nTH en 1/10C (%03d):"),iTh);
      iTh = iTermGetDec(stdin, 3);
      printf_P(PSTR("\nTL en 1/10C (%03d):"),iTl);
      iTl = iTermGetDec(stdin, 3);
      putchar('\n');
      xThOrTlChanged = true;
    }
    else {

      delay_ms (1000);
    }

    if (xThOrTlChanged) {

      // Test 7 - Modif des températures de seuils haut et bas
      vDs1621SetTl (TEST_DS1621, iTl);
      vAssertLastError();
      iT = iDs1621GetTl (TEST_DS1621);
      vAssertLastError();
      vAssert ( iTl == iT );

      vDs1621SetTh (TEST_DS1621, iTh);
      vAssertLastError();
      iT = iDs1621GetTh (TEST_DS1621);
      vAssertLastError();
      vAssert ( iTh == iT );

      printf_P(PSTR("Set TH = %d - TL = %d\n"), iTh, iTl);
      xThOrTlChanged = false;
    }
  }
  return 0;
}

/* ========================================================================== */
