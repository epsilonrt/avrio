/* Copyright _ 2009-2012 epsilonRT. All rights reserved.
 *  $Id$ */

/* ----------------------- Modbus includes ---------------------------------- */
#include "mb.h"
#include "mbport.h"

/* ----------------------- Start implementation ----------------------------- */
int
main (void) {
  xMBPortTimersInit (20000);
  vMBPortTimersEnable ();
  for (;;) ;
}

/* ========================================================================== */
