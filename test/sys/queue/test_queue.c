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
 * @file test_queue.c
 * @brief Tests unitaires du module \ref queue_module "Pile FIFO".
 *

   ========================================================================== */
#include <avrio/led.h>
#include <avrio/delay.h>
#include <avrio/queue.h>
#include <avrio/task.h>
#include <string.h>

/* ========================================================================== */
/**
 * @addtogroup test_group
 * @{
 */

/* constants ================================================================ */
#define QUEUE_SIZE  16
#define FIRST_VALUE 0x41

/*
 * Validation des tests à effectuer
 */
#define TEST_BASE
#define TEST_DYNAMIC
#define TEST_EXTENDED
#define TEST_LOCKED

/* private variables ======================================================== */
QUEUE_STATIC_DECLARE (xStaticQ, QUEUE_SIZE);

/* private functions ======================================================== */
// ------------------------------------------------------------------------------
static void
vAssert (bool test) {

  if (test == false) {

    for (;;) {

      vLedSet (LED_LED1);
      delay_ms (25);
      vLedClear (LED_LED1);
      delay_ms (75);
    }
  }
}

/* internal public functions ================================================ */
#ifdef TEST_BASE
  // ----------------------------------------------------------------------------
static void
prvvTestBase (xQueue * pxQueue) {
  uint8_t ucByte = FIRST_VALUE;
  uint8_t ucCheck = FIRST_VALUE;
  uint16_t usWord = 0x1234;
  size_t xSize;

  vQueueFlush (pxQueue);

  xSize = xQueueSize (pxQueue);
  vAssert (xSize == QUEUE_SIZE);

  xSize = xQueueLength (pxQueue);
  vAssert (xSize == 0);

  xSize = xQueueFree (pxQueue);
  vAssert (xSize == QUEUE_SIZE);

  vAssert (xQueueIsEmpty (pxQueue));
  vAssert (!xQueueIsFull (pxQueue));

  while (!xQueueIsFull (pxQueue)) {

    vQueuePush (pxQueue, ucByte++);
  }

  ucByte = ucQueueRead (pxQueue, xQueueLength (pxQueue));
  vAssert (ucByte == (FIRST_VALUE + xQueueLength (pxQueue) - 1));

  while (!xQueueIsEmpty (pxQueue)) {

    ucByte = ucQueuePull (pxQueue);
    vAssert (ucCheck++ == ucByte);
  }

  vQueuePushWord (pxQueue, usWord);
  vAssert (usQueueReadWord (pxQueue, 0) == usWord);
  vAssert (usQueuePullWord (pxQueue) == usWord);

  vQueuePushWord (pxQueue, usWord);
  vQueueDropBytes (pxQueue, sizeof (usWord));
  xSize = xQueueLength (pxQueue);
  vAssert (xSize == 0);
}
#else
#  define prvvTestBase(p)
#endif

#ifdef TEST_EXTENDED
  // ----------------------------------------------------------------------------
static void
prvvTestExtended (xQueue * pxQueue) {
  uint8_t ucByte = FIRST_VALUE;
  uint8_t ucIndex;
  size_t xSize;
  uint8_t ucBytes[QUEUE_SIZE];
  char pcString[] = "Le test est bon !";
  const char *pcEnd;

  // Test 1: ucQueuePullAll()
  while (!xQueueIsFull (pxQueue)) {

    vQueuePush (pxQueue, ucByte++);
  }

  xSize = xQueuePullAll (pxQueue, ucBytes);
  vAssert (xSize == QUEUE_SIZE);
  for (ucIndex = 0; ucIndex < QUEUE_SIZE; ucIndex++) {

    vAssert ((FIRST_VALUE + ucIndex) == ucBytes[ucIndex]);
  }

  // Test 2: Vide la pile
  vQueueFlush (pxQueue);
  vAssert (xQueueIsEmpty (pxQueue));

  // Test 3: ucQueuePushBytes()
  xSize = xQueuePushBytes (pxQueue, ucBytes, QUEUE_SIZE);
  vAssert (xSize == 0);
  vAssert (memcmp (pxQueue->pxFirst, ucBytes, QUEUE_SIZE) == 0);

  // Test 4: pcQueuePushString()
  vQueueFlush (pxQueue);
  pcEnd = pcQueuePushString (pxQueue, pcString);
  vAssert (*pcEnd == '!');
  vAssert (memcmp (pxQueue->pxFirst, pcString, QUEUE_SIZE) == 0);
}
#else
#  define prvvTestExtended(p)
#endif

#if defined(TEST_DYNAMIC) && defined(AVRIO_QUEUE_MALLOC_ENABLE)
  // ----------------------------------------------------------------------------
static void
prvvTestDynamic (void) {
  xQueue *pxQueue;

  pxQueue = pxQueueNew (QUEUE_SIZE);
  vAssert (pxQueue != 0);

  prvvTestBase (pxQueue);
  prvvTestExtended (pxQueue);

  vQueueDelete (pxQueue);
}
#else
#  define prvvTestDynamic()
#endif

#ifdef TEST_LOCKED
static xTaskHandle xTaskPush;

// ----------------------------------------------------------------------------
static void
vTaskPush (xTaskHandle xTaskPush) {
  static uint8_t ucCount;

  bool bIsFull = xQueueIsFull(&xStaticQ);

  if (!bIsFull) {

    vQueuePush (&xStaticQ, ucCount++);
  }
  vTaskStart (xTaskPush);
}

// ----------------------------------------------------------------------------
static void
prvvTestLockedInit (void) {

  xTaskPush = xTaskCreate (xTaskConvertMs (50), vTaskPush);
}

// ----------------------------------------------------------------------------
static void
prvvTestLocked (void) {
  static uint8_t ucCount;
  uint8_t ucByte;
  bool bIsFull, bIsEmpty;


  vQueueFlush (&xStaticQ);
  vTaskStart (xTaskPush);
  do  {

    if (xQueueTryLock (&xStaticQ, QUEUE_LOCK_FREE) == 0) {

      bIsFull = xQueueIsFull(&xStaticQ);
      vQueueUnlock (&xStaticQ, QUEUE_LOCK_FREE);
    }
    vLedToggle (LED_LED1);
    delay_ms (200);
  } while (!bIsFull);

  do {

    if (xQueueTryLock (&xStaticQ, QUEUE_LOCK_FREE) == 0) {

      bIsEmpty = xQueueIsEmpty (&xStaticQ);
      if (!bIsEmpty) {

        ucByte = ucQueuePull (&xStaticQ);
        vAssert (ucByte == ucCount++);
      }
      else {

        vTaskStop (xTaskPush);
      }
      vQueueUnlock (&xStaticQ, QUEUE_LOCK_FREE);
    }
  } while (!bIsEmpty);
}
#else
#  define prvvTestLocked(p)
#  define prvvTestLockedInit()

#endif

/* main ===================================================================== */
int
main (void) {
  QUEUE_DECLARE (xQ, QUEUE_SIZE);

  vLedInit ();
  prvvTestLockedInit();

  for (;;) {

    // Test 1: Test de base sur pile fifo stockée sur pile système
    prvvTestBase (&xQ);

    // Test 2: Test de base sur pile fifo stockée sur le tas
    prvvTestBase (&xStaticQ);

    // Test 3: Test des fonctions étendues sur pile fifo stockée sur le tas
    prvvTestExtended (&xStaticQ);

    // Test 4: Test de base et étendus sur pile fifo stockée en mémoire
    // dynamique
    prvvTestDynamic ();

    // Test 5:
    prvvTestLocked();

    vLedToggle (LED_LED1);
    delay_ms (1000);
  }
  return 0;
}

/** @} */
/* ========================================================================== */
