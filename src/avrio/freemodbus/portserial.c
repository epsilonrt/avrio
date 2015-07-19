/* Copyright _ 2009-2012 epsilonRT. All rights reserved.
 *  $Id$ */
/* ========================================================================== */
#include "avrio-config.h"

#ifdef AVRIO_MODBUS_ENABLE
/* ========================================================================== */
#include "avrio-board-modbus.h"
#include <avrio/serial_private.h>

/* ----------------------- AVR includes ------------------------------------- */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include <avrio/led.h>
#include <avrio/task.h>

/* ----------------------- Platform includes -------------------------------- */
#include "port.h"

/* ----------------------- Modbus includes ---------------------------------- */
#include "mb.h"
#include "mbport.h"

/* ----------------------- Defines ------------------------------------------ */

/* ----------------------- AVR platform specifics --------------------------- */
#undef SERIAL_BAUD_X1
#undef SERIAL_BAUD_X2
#define SERIAL_BAUD_X1(usBaud) (AVRIO_CPU_FREQ / (16UL * usBaud) - 1)
#define SERIAL_BAUD_X2(usBaud) (AVRIO_CPU_FREQ / (8UL * usBaud) - 1)

/* ---------------------- Platform specifics -------------------------------- */

#if defined(MB_SERIAL_LEDCOM) && defined(MB_SERIAL_LEDCOM_DELAY)
/* private variables ======================================================== */
static xTaskHandle xMBLedComTimer;
/* ========================================================================== */
#endif

// -----------------------------------------------------------------------------
INLINE void
vMBLedComSet (void) {

#if defined(MB_SERIAL_LEDCOM) && defined(MB_SERIAL_LEDCOM_DELAY)
  vLedSet (MB_SERIAL_LEDCOM);
  vTaskStart (xMBLedComTimer);
#endif
}

// -----------------------------------------------------------------------------
INLINE void
vMBLedComClr (void) {

#if defined(MB_SERIAL_LEDCOM) && defined(MB_SERIAL_LEDCOM_DELAY)
  vLedClear (MB_SERIAL_LEDCOM);
  vTaskStop (xMBLedComTimer);
#endif
}

#if defined(MB_SERIAL_LEDCOM) && defined(MB_SERIAL_LEDCOM_DELAY)
/* ========================================================================== */
static void
vMBLedComTask (xTaskHandle xUnUsed) {

  vMBLedComClr ();
}
/* ========================================================================== */
#endif

// -----------------------------------------------------------------------------
INLINE void
vMBLedComInit (void) {

#if defined(MB_SERIAL_LEDCOM) && defined(MB_SERIAL_LEDCOM_DELAY)
  xMBLedComTimer = xTaskCreate (MB_SERIAL_LEDCOM_DELAY, vMBLedComTask);
#endif
}

// -----------------------------------------------------------------------------
INLINE void
vMBLedErrSet (void) {

#ifdef MB_SERIAL_LEDERR
  vLedSet (MB_SERIAL_LEDERR);
#endif
}

// -----------------------------------------------------------------------------
INLINE void
vMBLedErrClr (void) {

#ifdef MB_SERIAL_LEDERR
  vLedClear (MB_SERIAL_LEDERR);
#endif
}

// -----------------------------------------------------------------------------
INLINE void
vMBLedErrInit (void) {

#ifdef MB_SERIAL_LEDERR
#endif
}

// -----------------------------------------------------------------------------
INLINE void
vMBLedInit (void) {
#if defined(MB_SERIAL_LEDERR) || (defined(MB_SERIAL_LEDCOM) && defined(MB_SERIAL_LEDCOM_DELAY))
  vLedInit();
#endif
}

#ifdef MB_SERIAL_DEBUG
/* private variables ======================================================== */
static volatile uint8_t ucMBSerialStatus;

/* external public functions ================================================ */
extern void vUARTRxReadyISR (void);
extern void vUARTTxReadyISR (void);

/* constants ================================================================ */
#define READY 0

/* macros =================================================================== */
#define READY_SET() do {\
    ucMBSerialStatus |= _BV(READY);\
  } while(0)

#define READY_CLR() do {\
    ucMBSerialStatus &= ~_BV(READY);\
  } while(0)

#else
#define READY_SET()
#define READY_CLR()
#endif

/* private functions ======================================================== */

// ------------------------------------------------------------------------------
ISR (USART_UDRE_vect) {

#ifndef MB_SERIAL_DEBUG
  vMBLedComSet ();
  pxMBFrameCBTransmitterEmpty ();
#else
  vUARTTxReadyISR ();
#endif
}

// ------------------------------------------------------------------------------
ISR (USART_RXC_vect) {

#ifndef MB_SERIAL_DEBUG
  vMBLedComSet ();
  pxMBFrameCBByteReceived ();
#else
  vUARTRxReadyISR ();
#endif
}

// ------------------------------------------------------------------------------
ISR (USART_TXC_vect) {

  vTxEnClear ();
  vMBLedComClr ();
  // UCSRB &= ~ (_BV (TXEN) | _BV (TXCIE));
  READY_SET ();
}

/* internal public functions ================================================ */

// ------------------------------------------------------------------------------
void
vMBPortSerialEnable (BOOL xRxEnable, BOOL xTxEnable) {
  UCSRB |= _BV (TXEN);

  if (xRxEnable) {

    UCSRB |= _BV (RXEN) | _BV (RXCIE);
  }
  else {

    UCSRB &= ~ (_BV (RXEN) | _BV (RXCIE));
  }

  if (xTxEnable) {

    // UCSRA |= _BV (TXC); /* clear TXC */
    READY_CLR ();
    vTxEnSet ();
    UCSRB |= _BV (UDRIE);  // start transmitting
  }
  else {

    UCSRB &= ~ _BV (UDRIE);
    UCSRB |=   _BV (TXCIE);
  }

  vMBLedComClr ();
  vMBLedErrClr ();
}

// ------------------------------------------------------------------------------
BOOL
xMBPortSerialInit (UCHAR ucPORT __attribute__ ( (unused)),
                   ULONG ulBaudRate, UCHAR ucDataBits, eMBParity eParity) {
  LONG lUBRR;
  UCHAR ucUCSRC = UCSRC_SEL;

  switch (eParity) {

    case MB_PAR_EVEN:
      ucUCSRC |= _BV (UPM1);
      break;
    case MB_PAR_ODD:
      ucUCSRC |= _BV (UPM1) | _BV (UPM0);
      break;
    case MB_PAR_NONE:
      ucUCSRC |= USBS; // Remark : the use of no parity requires 2 stop bits.
      break;
    default:
      return FALSE;
  }

  switch (ucDataBits) {

    case 8:
      ucUCSRC |= _BV (UCSZ0) | _BV (UCSZ1);
      break;
    case 7:
      ucUCSRC |= _BV (UCSZ1);
      break;
    default:
      return FALSE;
  }
  UCSRC = ucUCSRC;

  lUBRR = SERIAL_BAUD_X1 (ulBaudRate);
  if (lUBRR <= 0) {

    lUBRR = SERIAL_BAUD_X2 (ulBaudRate);
    UCSRA |= _BV (U2X);
  }
  if ( (lUBRR <= 0) || (lUBRR > 65535)) {

    return FALSE;
  }
  UBRRL = lUBRR & 0xFF;
  UBRRH = lUBRR >> 8;

  vTxEnInit ();
  vMBLedInit();
  vMBLedErrInit ();
  vMBLedComInit ();
  vMBPortSerialEnable (FALSE, FALSE);
  READY_SET ();
  return TRUE;
}

// ------------------------------------------------------------------------------
BOOL
xMBPortSerialPutByte (CHAR cByte) {

  UDR = cByte;
  return TRUE;
}

// ------------------------------------------------------------------------------
BOOL
xMBPortSerialGetByte (CHAR * pcByte) {

  *pcByte = UDR;
  return TRUE;
}

#ifdef MB_SERIAL_DEBUG
// ------------------------------------------------------------------------------
BOOL
xMBPortSerialReady (void) {
  uint8_t ucValue;

  ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {
    ucValue = ucMBSerialStatus;
  }
  return (ucValue & _BV (READY)) != 0;
}
#endif

#endif /* AVRIO_MODBUS_ENABLE defined */
/* ========================================================================== */
