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
 * @file test_memdebug.c
 * @brief Tests unitaires du module \ref memdebug_module "Statistiques Mémoire".
 * @author Andy Brown
 * @copyright http://andybrown.me.uk/ws/terms-and-conditions
   ========================================================================== */
#include <stdio.h>
#include <stdlib.h>
#include <avrio/serial.h>
#include <avrio/debug.h>
#include <avrio/memdebug.h>

/* constants ================================================================ */
#define SERIAL_BAUDRATE 9600
#define SERIAL_SETTINGS (SERIAL_DEFAULT + SERIAL_WR)

/* internal public functions ================================================ */
void shuffle (void **array, int n);
void showmem(void);
void dotest (bool shuffle);

/* main ===================================================================== */
int
main (void) {
  vSerialInit (SERIAL_BAUDRATE / 100, SERIAL_SETTINGS);
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
