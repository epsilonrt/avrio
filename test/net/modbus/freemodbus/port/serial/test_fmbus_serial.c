
/* ----------------------- Platform includes -------------------------------- */
#include <avrio/delay.h>
#include <avrio/led.h>

/* ----------------------- Modbus includes ---------------------------------- */
#include "mb.h"
#include "mbport.h"

/* ----------------------- Start implementation ----------------------------- */
extern BOOL xMBPortSerialReady (void);

/* private functions ======================================================== */
#define BUFFER_SIZE 32
static unsigned int uiRxCnt;
static uint8_t buffer[BUFFER_SIZE];

void
prvvError (void) {

  for (;;) {

    vLedToggle (LED_LED1);
    delay_ms (50);
  }
}

/* internal public functions ================================================ */
// ------------------------------------------------------------------------------
int
main (void) {

  vLedInit();

  /* Initialize COM device 0 with 38400 baud, 8 data bits and no parity. */
  if (xMBPortSerialInit (0, 38400, 8, MB_PAR_NONE) == FALSE) {

    prvvError ();
  }
  sei ();

  // Valide le transmetteur et envoi l'alphabet A..Z
  vMBPortSerialEnable (FALSE, TRUE);
  // Attends le fin de la transmission
  while (xMBPortSerialReady () == FALSE)
    ;
  // Valide le récepteur et bascule LED1 à chaque caractère reçu
  vMBPortSerialEnable (TRUE, FALSE);

  for (;;) {
    // Ne fait rienvv
  }
}

// ------------------------------------------------------------------------------
void
vUARTRxReadyISR (void) {
  static uint8_t ucIndex;

  (void) xMBPortSerialGetByte ((CHAR *)&buffer[ucIndex % BUFFER_SIZE]);
  uiRxCnt++;
  ucIndex++;
  vLedToggle (LED_LED1);
}

// ------------------------------------------------------------------------------
void
vUARTTxReadyISR (void) {
  static char cChar = 'A';

  if (cChar <= 'Z') {

    (void) xMBPortSerialPutByte (cChar++);
    vLedToggle (LED_LED1);
  }
  else {

    vMBPortSerialEnable (FALSE, FALSE);
  }
}

/* ========================================================================== */
