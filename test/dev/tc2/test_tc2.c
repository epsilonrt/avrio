/**
 * @file test_serial.c
 * @brief Test unitaire liaison série asynchrone, version à 2 ports simultanés
 *
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
 */
#include <avrio/tc.h>
#include <avrio/led.h>
#include <avrio/delay.h>
#include <avrio/mutex.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <avr/version.h>
// Pour TC_TXBUFSIZE
#include "avrio-board-tc.h"

/* constants ================================================================ */
/*
 * Validation des tests
 * Il suffit d'ajouter les valeurs correspondantes aux tests souhaités
 * et de mettre cette valeur à TCn_TESTS
 */
#define TEST_DEBUG        1 /* Transmission de la lettre 'U' */
#define TEST_ALPHABET     2 /* Envoi de l'alphabet A -> Z */
#define TEST_PONG         4 /* Attente d'un caractère puis renvoi */
#define TEST_TXASYNC      8 /* Envoi de l'alphabet si appui BP Nord */

#define TC0_TESTS        (12)
#define TC1_TESTS        (2)

/*
 * Configuration Port 0
 */
#define TC0_BAUDRATE     115200
#define TC0_DATABIT      SERIAL_DATABIT_8 // 5 à 9 bits
#define TC0_PARITY       SERIAL_PARITY_NONE // NONE, EVEN, ODD
#define TC0_STOPBIT      SERIAL_STOPBIT_ONE // 1 ou 2
#define TC0_FLOWCTL      SERIAL_FLOW_RTSCTS
//#define TC0_FLOWCTL      SERIAL_FLOW_NONE
//#define TC0_FLAGS         (O_RDWR | O_NONBLOCK | O_HDUPLEX)
#define TC0_FLAGS         (O_RDWR | O_NONBLOCK)
//#define TC0_FLAGS         (O_RDWR)

/*
 * Configuration Port 1
 */
#define TC1_BAUDRATE     38400
#define TC1_DATABIT      SERIAL_DATABIT_8 // 5 à 9 bits
#define TC1_PARITY       SERIAL_PARITY_NONE // NONE, EVEN, ODD
#define TC1_STOPBIT      SERIAL_STOPBIT_ONE // 1 ou 2
//#define TC1_FLOWCTL      SERIAL_FLOW_RTSCTS
#define TC1_FLOWCTL      SERIAL_FLOW_NONE
//#define TC1_FLAGS         (O_RDWR | O_NONBLOCK | O_HDUPLEX)
#define TC1_FLAGS         (O_RDWR | O_NONBLOCK)

/*
 * Pause dans la boucle principale
 */
#define LOOP_DELAY   0 // Valeur en ms

/* private variables ======================================================== */
static   FILE * tc[2];
static uint8_t tests[2] = {TC0_TESTS, TC1_TESTS};
static xMutex xButNorth = MUTEX_INITIALIZER;
static xMutex xButWest = MUTEX_INITIALIZER;

static xSerialIos xIos0 = {
  .baud = TC0_BAUDRATE, .dbits = TC0_DATABIT, .parity = TC0_PARITY,
  .sbits = TC0_STOPBIT, .flow = TC0_FLOWCTL
};

static xSerialIos xIos1 = {
  .baud = TC1_BAUDRATE, .dbits = TC1_DATABIT, .parity = TC1_PARITY,
  .sbits = TC1_STOPBIT, .flow = TC1_FLOWCTL
};

/* internal public functions ================================================ */
void vTestDebug (uint8_t i);
void vTestAlphabet (uint8_t i);
void vTestPong (uint8_t i);
void vTestTxAsync (uint8_t i);
void vLedAssert (int i);

/* main ===================================================================== */
int
main (void) {

  vLedInit();

  // Bouton poussoir NORD et EST
  vMutexLock (&xButNorth);
  vMutexLock (&xButWest);
  PORTE |= _BV (6) | _BV (4);
  EICRB |= _BV (ISC61) | _BV (ISC41); // front descendant
  EIMSK |= _BV (INT6) | _BV (INT4);


  tc[1] = xFileOpen ("tty1", TC1_FLAGS, &xIos1);
  vLedAssert (tc[1] != NULL);
  tc[0] = xFileOpen ("tty0", TC0_FLAGS, &xIos0);
  vLedAssert (tc[0] != NULL);
  sei();

  for (;;) {

    for (int i = 0; i < 2; i++) {
      if (tests[i] & TEST_DEBUG) {

        vTestDebug (i);
      }
      if (tests[i] & TEST_ALPHABET) {

        vTestAlphabet (i);
      }
      if (tests[i] & TEST_PONG) {

        vTestPong (i);
      }
      if (tests[i] & TEST_TXASYNC) {

        vTestTxAsync (i);
      }
#if LOOP_DELAY > 0
      delay_ms (LOOP_DELAY);
#endif
    }
  }
  return 0;
}

/* internal public functions ================================================ */

/* -----------------------------------------------------------------------------
 * Test de debug
 * Envoi d'un U
 */
void
vTestDebug (uint8_t i) {
  int iErr;
  int c;

  c = 0x55;
  iErr = fputc (c, tc[i]);
  if (iErr == c) {

    vLedToggle (xLedGetMask (i));
  }
}

/* -----------------------------------------------------------------------------
 * Envoi de l'alphabet A -> Z
 */
void
vTestAlphabet (uint8_t i) {
  int iErr;
  int c, z, s;

  vLedSet (xLedGetMask (i));

  if (i & 1) {

    c = 'Z';
    s = 'A';
    z = -1;
  }
  else {

    c = 'A';
    s = 'Z';
    z = 1;
  }

  for (;;) {

    iErr = fputc (c, tc[i]);
    if (iErr == c) {

      // Caractère transmis avec succès, on passe au suivant
      if (c == '\n') {

        // dernier caractère, on sort de la boucle
        break;
      }
      else {

        // Si pas à la fin, on passe au suivant, sinon on envoit le dernier
        if (c != s) {

          c += z;
        }
        else {

          c = '\n';
        }
      }
    }
  }

  vLedClear (xLedGetMask (i));
}

// -----------------------------------------------------------------------------
void
vTestTxAsync (uint8_t i) {

  if (xMutexTryLock (&xButNorth) == 0) {

    vTestAlphabet (0);
  }
  if (xMutexTryLock (&xButWest) == 0) {

    vTestAlphabet (1);
  }
}

/* -----------------------------------------------------------------------------
 * Test Pong
 * Boucle infinie d'attente d'un caractère puis renvoi
 */
void
vTestPong (uint8_t i) {
  int c;

  c = fgetc (tc[i]);
  if (c != EOF) {
    int iErr;

    iErr = fputc (c, tc[i]);
    vLedAssert (iErr == c);
    vLedToggle (xLedGetMask (i));
  }
}

/* -----------------------------------------------------------------------------
 * Vérifie que la condition passée est vraie, sinon fait clignoter rapidement
 * la led 1 à l'infinie
 */
void
vLedAssert (int i) {

  if (!i) {

    for (;;) {

      vLedSet (LED_LED8);
      delay_ms (50);
      vLedClear (LED_LED8);
      delay_ms (100);
    }
  }
}

//------------------------------------------------------------------------------
ISR (INT4_vect) {

  vMutexUnlock (&xButNorth);
}

//------------------------------------------------------------------------------
ISR (INT6_vect) {

  vMutexUnlock (&xButWest);
}

/* ========================================================================== */
