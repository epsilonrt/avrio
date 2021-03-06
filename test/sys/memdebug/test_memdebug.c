#include <stdio.h>
#include <stdlib.h>
#include <avrio/serial.h>
#include <avrio/debug.h>
#include <avrio/memdebug.h>

/* constants ================================================================ */
#define SERIAL_BAUDRATE 9600
#define SERIAL_IOS (SERIAL_DEFAULT + SERIAL_WR)

/* internal public functions ================================================ */
void shuffle (void **array, int n);
void showmem(void);
void dotest (bool shuffle);

/* main ===================================================================== */
int
main (void) {
  vSerialInit (SERIAL_BAUDRATE / 100, SERIAL_IOS);
  stdout = &xSerialPort;
  dprintf("Memory Debug Test\n");

  dotest (false);
  dotest (true);

  return 0;
}

/* internal public functions ================================================ */

// -----------------------------------------------------------------------------
void
dotest (bool shuffle_) {
  void *ptrs[20];
  uint16_t i;

  dprintf ("allocating\n");
  showmem();
  for (i = 0; i < sizeof (ptrs) / sizeof (ptrs[0]); i++) {

    ptrs[i] = malloc (20);
    showmem();
  }

  if (shuffle_) {

    shuffle (ptrs, sizeof (ptrs) / sizeof (ptrs[0]));
  }

  dprintf ("freeing\n");
  for (i = 0; i < sizeof (ptrs) / sizeof (ptrs[0]); i++) {

    free (ptrs[i]);
    showmem();
  }
}


// -----------------------------------------------------------------------------
void showmem() {

  dprintf ("%04u %04u %04u : used/free/large\n",
           ulMemoryUsed(),
           ulFreeMemory(),
           ulLargestAvailableMemoryBlock()
          );
}

// -----------------------------------------------------------------------------
static inline int
  rand_int (int n) {
  int limit = RAND_MAX - RAND_MAX % n;
  int rnd;

  do {
    rnd = rand();
  }
  while (rnd >= limit);
  return rnd % n;
}

// -----------------------------------------------------------------------------
void
  shuffle (void **array, int n) {
  int i, j;
  void *tmp;

  for (i = n - 1; i > 0; i--) {
    j = rand_int (i + 1);
    tmp = array[j];
    array[j] = array[i];
    array[i] = tmp;
  }
}

/* ========================================================================== */
