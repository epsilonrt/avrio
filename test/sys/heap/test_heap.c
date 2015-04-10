/**
 * @file test_heap.c
 * @brief Tests unitaires du module \ref heap_module "Heap".
 * @author Copyright © 2015 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * @version $Id$
   ========================================================================== */
#include <stdio.h>
#include <avr/pgmspace.h>
#include <avrio/delay.h>
#include <avrio/led.h>
#include <avrio/serial.h>
#include <avrio/assert.h>
#include <avrio/heap.h>

/* constants ================================================================ */
#define SERIAL_BAUDRATE 9600
#define SERIAL_SETTINGS (SERIAL_DEFAULT + SERIAL_WR)

#define TEST_LEN 7
#define ALLOC_SIZE 113
#define TEST_LEN2 8
#define ALLOC_SIZE2 (CONFIG_HEAP_SIZE / TEST_LEN2)

/* private variables ======================================================== */
HEAP_DEFINE_BUF (buffer, CONFIG_HEAP_SIZE);
static xHeap heap;
static size_t s;
const PROGMEM char sSuccess[]  = "Success\n";
static int iTestCount;

/* internal public functions ================================================ */
void vAllocMemTest (size_t size, size_t nmemb);
void vMallocMemTest (size_t size, size_t nmemb);
void vCallocMemTest (size_t size, size_t nmemb);
/* main ===================================================================== */
int
main (void)  {

  vLedInit();
  vSerialInit (SERIAL_BAUDRATE / 100, SERIAL_SETTINGS);
  stdout = &xSerialPort;
  stderr = &xSerialPort;

  vHeapInit (&heap, buffer, sizeof (buffer));

  for (;;) {
    iTestCount = 1;
    vLedSet (LED_LED1);
    fprintf_P (stderr, PSTR ("\nHeap Test\nHeap Size: %d\n"), CONFIG_HEAP_SIZE);

    fprintf_P (stderr, PSTR ("%d- ulHeapFreeSpace() Test... "), iTestCount++);
    s = ulHeapFreeSpace (&heap);
    assert (s == CONFIG_HEAP_SIZE);
    fprintf_P (stderr, sSuccess);

    // Test allocation
    vAllocMemTest (ALLOC_SIZE, TEST_LEN);
    vAllocMemTest (ALLOC_SIZE2, TEST_LEN2);

    // Test interface C ANSI
    vMallocMemTest (ALLOC_SIZE - sizeof (size_t), TEST_LEN);
    vMallocMemTest (ALLOC_SIZE2 - sizeof (size_t), TEST_LEN2);
    vCallocMemTest (ALLOC_SIZE - sizeof (size_t), TEST_LEN);
    vCallocMemTest (ALLOC_SIZE2 - sizeof (size_t), TEST_LEN2);

    /* Try to allocate the whole heap */
    fprintf_P (stderr,
               PSTR ("%d- Try to allocate the whole heap with pvHeapAllocMem()... "),
               iTestCount++);
    uint8_t * b = pvHeapAllocMem (&heap, CONFIG_HEAP_SIZE);
    assert (b);
    assert (ulHeapFreeSpace (&heap) == 0);
    fprintf_P (stderr, sSuccess);

    fprintf_P (stderr,
               PSTR ("%d- Try to reallocate the whole heap with pvHeapAllocMem()... "),
               iTestCount++);
    assert (!pvHeapAllocMem (&heap, CONFIG_HEAP_SIZE));
    fprintf_P (stderr, sSuccess);

    fprintf_P (stderr,
               PSTR ("%d- Try to write and read in the whole heap... "),
               iTestCount++);
    for (int j = 0; j < CONFIG_HEAP_SIZE; j++) {
      b[j] = j;
    }
    for (int j = 0; j < CONFIG_HEAP_SIZE; j++) {
      assert (b[j] == (j & 0xff));
    }
    fprintf_P (stderr, sSuccess);

    fprintf_P (stderr,
               PSTR ("%d- Try to free the whole heap with vHeapFreeMem()... "),
               iTestCount++);
    vHeapFreeMem (&heap, b, CONFIG_HEAP_SIZE);
    s = ulHeapFreeSpace (&heap);
    assert (s == CONFIG_HEAP_SIZE);
    fprintf_P (stderr, sSuccess);

    fprintf_P (stderr,
               PSTR ("\nAll tests are passed successfully !\n"));
    vLedClear (LED_LED1);
    delay_ms (5000);
  }
  return 0;
}

// -----------------------------------------------------------------------------
void
vAllocMemTest (size_t size, size_t nmemb) {
  // Tableau des pointeurs d'allocation
  uint8_t * a[nmemb];

  fprintf_P (stderr,
             PSTR ("%d- Allocate %d chunks of %d bytes with pvHeapAllocMem()... "),
             iTestCount++, nmemb, size);
  for (size_t i = 0; i < nmemb; i++) {

    // Alloue les zones demandées
    a[i] = pvHeapAllocMem (&heap, size);
    assert (a[i]);

    for (size_t j = 0; j < size; j++) {
      // Initialise les octets de la zone avec son index
      a[i][j] = i + 0x40;
    }
  }
  // Vérifie que l'espace libre correspond
  s = CONFIG_HEAP_SIZE - nmemb * ROUND_UP2 (size, sizeof (xMemChunk));
  assert (ulHeapFreeSpace (&heap) == s);
  fprintf_P (stderr, sSuccess);

  fprintf_P (stderr,
             PSTR ("%d- Frees %d chunks of %d bytes with vHeapFreeMem()... "),
             iTestCount++, nmemb, size);
  for (size_t i = 0; i < nmemb; i++) {
    // Lecture dans la zone pour vérifier
    for (size_t j = 0; j < size; j++) {
      //printf ("a[%d][%d] = %d\n", i, j, a[i][j]);
      assert (a[i][j] == i + 0x40);
    }
    vHeapFreeMem (&heap, a[i], size);
  }
  assert (ulHeapFreeSpace (&heap) == CONFIG_HEAP_SIZE);
  fprintf_P (stderr, sSuccess);
}

// -----------------------------------------------------------------------------
void
vMallocMemTest (size_t size, size_t nmemb) {
  // Tableau des pointeurs d'allocation
  uint8_t * a[nmemb];

  fprintf_P (stderr,
             PSTR ("%d- Allocate %d chunks of %d bytes with pvHeapMalloc()... "),
             iTestCount++, nmemb, size);
  for (size_t i = 0; i < nmemb; i++) {

    // Alloue les zones demandées
    a[i] = pvHeapMalloc (&heap, size);
    assert (a[i]);

    for (size_t j = 0; j < size; j++) {
      // Initialise les octets de la zone avec son index
      a[i][j] = i + 0x40;
    }
  }

  // Vérifie que l'espace libre correspond
  s = CONFIG_HEAP_SIZE - nmemb * ROUND_UP2 (size + sizeof (size_t), sizeof (xMemChunk));
  assert (ulHeapFreeSpace (&heap) == s);

  fprintf_P (stderr, sSuccess);

  fprintf_P (stderr,
             PSTR ("%d- Frees %d chunks of %d bytes with vHeapFree()... "),
             iTestCount++, nmemb, size);
  for (size_t i = 0; i < nmemb; i++) {
    // Lecture dans la zone pour vérifier
    for (size_t j = 0; j < size; j++) {
      //printf ("a[%d][%d] = %d\n", i, j, a[i][j]);
      assert (a[i][j] == i + 0x40);
    }
    vHeapFree (&heap, a[i]);
  }
  assert (ulHeapFreeSpace (&heap) == CONFIG_HEAP_SIZE);
  fprintf_P (stderr, sSuccess);
}

// -----------------------------------------------------------------------------
void
vCallocMemTest (size_t size, size_t nmemb) {
  // Tableau des pointeurs d'allocation
  uint8_t * a;

  fprintf_P (stderr,
             PSTR ("%d- Allocate %d elements of %d bytes with pvHeapCalloc()... "),
             iTestCount++, nmemb, size);
  a = pvHeapCalloc (&heap, size, nmemb);
  assert (a);

  for (size_t i = 0; i < nmemb; i++) {

    for (size_t j = 0; j < size; j++) {

      // Vérifie RAZ
      assert (a[i][j] == 0);
    }
  }

  // Vérifie que l'espace libre correspond
  s = CONFIG_HEAP_SIZE - ROUND_UP2 ( (nmemb * size) + sizeof (size_t), sizeof (xMemChunk));
  assert (ulHeapFreeSpace (&heap) == s);
  fprintf_P (stderr, sSuccess);

  fprintf_P (stderr,
             PSTR ("%d- Frees %d elements of %d bytes with vHeapFree()... "),
             iTestCount++, nmemb, size);
  vHeapFree (&heap, a);
  assert (ulHeapFreeSpace (&heap) == CONFIG_HEAP_SIZE);
  fprintf_P (stderr, sSuccess);
}

/* ========================================================================== */
