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
 * @file test_mutex.c
 * @brief Tests unitaires du module \ref mutex_module "Mutex".
 *

   ========================================================================== */
#include <avrio/led.h>
#include <avrio/delay.h>
#include <avrio/mutex.h>

/* ========================================================================== */
/**
 * @addtogroup test_group
 * @{
 */

/* constants ================================================================ */
/* 
 * Validation des tests à effectuer
 */
#define TEST_BASE
#define TEST_MUTEXBIT

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
#ifdef TEST_BASE
  // ----------------------------------------------------------------------------
static void
prvvTestBase (void) {
  xMutex xMyMutex = MUTEX_INITIALIZER;

  vMutexLock (&xMyMutex);
  vAssert (xMutexTryLock (&xMyMutex) != 0);
  vMutexUnlock (&xMyMutex);
  vAssert (xMutexTryLock (&xMyMutex) == 0);
}
#else
#  define prvvTestBase()
#endif

#ifdef TEST_MUTEXBIT
  // ----------------------------------------------------------------------------
static void
prvvTestMutexBit (void) {
  xMutex xMyMutex = MUTEX_INITIALIZER;
  uint8_t ucMask = 1;

  while (ucMask) {

    vMutexLockBit (&xMyMutex, ucMask);
    vAssert (xMutexTryLockBit (&xMyMutex, ucMask) != 0);
    vAssert (xMutexBitLocked (&xMyMutex, ucMask) == true);
    vMutexUnlockBit (&xMyMutex, ucMask);
    vAssert (xMutexBitLocked (&xMyMutex, ucMask) == false);
    vMutexUntilBitUnlocked (&xMyMutex, ucMask);
    vAssert (xMutexTryLockBit (&xMyMutex, ucMask) == 0);
    ucMask <<= 1;
  }

}
#else
#  define prvvTestMutexBit()
#endif

/* main ===================================================================== */
int
main (void) {

  vLedInit ();

  for (;;) {

    // Test 1: Mutex octet 
    prvvTestBase ();

    // Test 2: Mutex bit
    prvvTestMutexBit ();

    vLedToggle (LED_LED1);
    delay_ms (150);
  }
  return 0;
}

/** @} */
/* ========================================================================== */
