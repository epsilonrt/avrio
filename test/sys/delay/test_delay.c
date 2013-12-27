/* Copyright © 2009-2012 epsilonRT. All rights reserved.
 *  $Id$ */
/* ========================================================================== */
#include <avrio/led.h>
#include <avrio/delay.h>

/* internal public functions ================================================ */
int
main (void) {

  vLedInit ();

  for (;;) {

    /* Génère un signal rectangulaire de période 400 ms */
    delay_ms (1000);
    vLedToggle (LED_LED1);
  }
  return 0;
}

/* ========================================================================== */
