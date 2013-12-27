/**
 * @file test_mutex.c
 * @brief Tests unitaires du module \ref mutex_module "Mutex".
 * @author Copyright © 2011-2012 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * @version $Id$
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
