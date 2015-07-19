/* Copyright _ 2009-2012 epsilonRT. All rights reserved.
 *  $Id$ */

/* ----------------------- Modbus includes ---------------------------------- */
#include "mb.h"
#include "mbport.h"

/* ----------------------- Start implementation ----------------------------- */
int
main (void) {
  // Led clignote toutes les secondes 20 * 50 µs = 1 ms
  xMBPortTimersInit (20000);
  vMBPortTimersEnable ();
  for (;;) {

  }
}

/* ========================================================================== */
