/* Copyright _ 2009-2012 epsilonRT. All rights reserved.
 *  $Id$ */

/* ----------------------- Platform includes -------------------------------- */
#include <avrio/delay.h>

/* ----------------------- Modbus includes ---------------------------------- */
#include "mb.h"
#include "mbport.h"

/* ----------------------- Defines ------------------------------------------ */
#define LED1_PORT     PORTD
#define LED1_DDR      DDRD
#define LED1_BIT      7
#define LED2_PORT     PORTD
#define LED2_DDR      DDRD
#define LED2_BIT      6

/* ----------------------- Start implementation ----------------------------- */
BOOL xMBPortSerialReady (void);

/* private functions ======================================================== */
static unsigned int uiRxCnt;

void
prvvError (void) {

  for (;;) {

    tbi (LED2_PORT, LED2_BIT);
    delay_ms (50);
  }
}

/* internal public functions ================================================ */
// ------------------------------------------------------------------------------
int
main (void) {

  sbi (LED1_DDR, LED1_BIT);
  sbi (LED1_PORT, LED1_BIT);
  sbi (LED2_DDR, LED2_BIT);
  sbi (LED2_PORT, LED2_BIT);

  /* Initialize COM device 0 with 38400 baud, 8 data bits and no parity. */
  if (xMBPortSerialInit (0, 38400, 8, MB_PAR_NONE) == FALSE) {

    prvvError ();
  }
  sei ();

  vMBPortSerialEnable (FALSE, TRUE);  /* Enable the transmitter and transmit
                                       * the alphabet. */
  while (xMBPortSerialReady () == FALSE) ;
  vMBPortSerialEnable (TRUE, FALSE);  /* Enable the receiver, toggle LED1 for
                                       * each character received. */

  for (;;) ;
}

// ------------------------------------------------------------------------------
void
vUARTRxReadyISR (void) {

  CHAR cByte;

  (void) xMBPortSerialGetByte (&cByte);
  /* Now cByte should contain the character received. */
  uiRxCnt++;
  tbi (LED1_PORT, LED1_BIT);
}

// ------------------------------------------------------------------------------
void
vUARTTxReadyISR (void) {
  static char cChar = 'A';

  if (cChar <= 'Z') {

    (void) xMBPortSerialPutByte (cChar++);
    tbi (LED1_PORT, LED1_BIT);
  } else {

    vMBPortSerialEnable (FALSE, FALSE);
  }
}

/* ========================================================================== */
