/**
 * @file test_queue.c
 * @brief Tests unitaires du module \ref queue_module "Pile FIFO".
 * @author Copyright © 2011-2012 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * @version $Id$
   ========================================================================== */
#include <avrio/led.h>
#include <avrio/button.h>
#include <avrio/delay.h>
#include <avrio/queue.h>
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

/* private variables ======================================================== */
QUEUE_STATIC_DECLARE (xStaticQ, QUEUE_SIZE);

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

/* main ===================================================================== */
int
main (void) {
  QUEUE_DECLARE (xQ, QUEUE_SIZE);

  vLedInit ();

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

    vLedToggle (LED_LED1);
    delay_ms (150);
  }
  return 0;
}

/** @} */
/* ========================================================================== */
