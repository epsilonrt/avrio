/* Copyright _ 2009-2012 epsilonRT. All rights reserved.
 *  $Id$ */
/* ========================================================================== */
#include "avrio-config.h"

#ifdef AVRIO_MODBUS_ENABLE
/* ========================================================================== */
#  include "avrio-board-modbus.h"

/* ----------------------- AVR includes ------------------------------------- */
#  include <avr/io.h>
#  include <avr/interrupt.h>
#  include <util/atomic.h>

/* ----------------------- Platform includes -------------------------------- */
#  include "port.h"

/* ----------------------- Modbus includes ---------------------------------- */
#  include "mb.h"
#  include "mbport.h"

/* ----------------------- Defines ------------------------------------------ */

/* ----------------------- AVR platform specifics --------------------------- */
#  define SERIAL_BAUD_X1(usBaud) (AVRIO_CPU_FREQ / (16UL * usBaud) - 1)
#  define SERIAL_BAUD_X2(usBaud) (AVRIO_CPU_FREQ / (8UL * usBaud) - 1)

#  if defined (__AVR_ATmega128__)
#    define UCSRC_SEL 0
#    define PARITY_ERROR _BV(UPE)
#    define FRAME_ERROR _BV(FE)

#    if MB_SERIAL_PORT != 0
#      define UCSRA           UCSR1A
#      define UCSRB           UCSR1B
#      define UCSRC           UCSR1C
#      define UBRRL           UBRR1L
#      define UBRRH           UBRR1H
#      define UDR             UDR1
#      define USART_TXC_vect  USART1_TX_vect
#      define USART_UDRE_vect USART1_UDRE_vect
#      define USART_RXC_vect  USART1_RX_vect

#    else
      /* SERIAL == 0 */
#      define UCSRA           UCSR0A
#      define UCSRB           UCSR0B
#      define UCSRC           UCSR0C
#      define UBRRL           UBRR0L
#      define UBRRH           UBRR0H
#      define UDR             UDR0
#      define USART_TXC_vect  USART0_TX_vect
#      define USART_UDRE_vect USART0_UDRE_vect
#      define USART_RXC_vect  USART0_RX_vect
#    endif
       /* SERIAL == 0 */

#  else
      /* __AVR_ATmega128__ not defined */
#    define UCSRC_SEL _BV(URSEL)
#    define PARITY_ERROR _BV(PE)
#    define FRAME_ERROR _BV(FE)

#  endif
/* ---------------------- Platform specifics --------------------------------- */
#  ifdef MB_SERIAL_LEDCOM_ENABLE
#    include <avrio/task.h>

static xTaskHandle xLedComTimer;

static inline void
vLedComSet (void) {

  MB_SERIAL_LEDCOM_PORT &= ~_BV (MB_SERIAL_LEDCOM_BIT);
  vTaskStart (xLedComTimer);
}

static inline void
vLedComClr (void) {

  MB_SERIAL_LEDCOM_PORT |= _BV (MB_SERIAL_LEDCOM_BIT);
  vTaskStop (xLedComTimer);
}

static void
vLedComTask (xTaskHandle xUnUsed) {

  vLedComClr ();
}

static inline void
vLedComInit (void) {

  MB_SERIAL_LEDCOM_DDR |= _BV (MB_SERIAL_LEDCOM_BIT);
  vLedComClr ();

  xLedComTimer = xTaskCreate (MB_SERIAL_LEDCOM_DELAY, vLedComTask);
}
#  else
#    define vLedComInit()
#    define vLedComSet()
#    define vLedComClr()
#  endif

#  ifdef MB_SERIAL_LEDERR_ENABLE
static inline void
vLedErrSet (void) {

  MB_SERIAL_LEDERR_PORT &= ~_BV (MB_SERIAL_LEDERR_BIT);
}

static inline void
vLedErrClr (void) {

  MB_SERIAL_LEDERR_PORT |= _BV (MB_SERIAL_LEDERR_BIT);
}

static inline void
vLedErrInit (void) {

  MB_SERIAL_LEDERR_DDR |= _BV (MB_SERIAL_LEDERR_BIT);
  vLedErrClr ();
}
#  else
#    define vLedErrInit()
#    define vLedErrSet()
#    define vLedErrClr()
#  endif

/* ----------------------- RS485 specifics ---------------------------------- */
#  ifdef MB_SERIAL_TXEN_ENABLE

static inline void
vTxEnSet (void) {

  MB_SERIAL_TXEN_PORT |= _BV (MB_SERIAL_TXEN_BIT);
}

static inline void
vTxEnClr (void) {

  MB_SERIAL_TXEN_PORT &= ~_BV (MB_SERIAL_TXEN_BIT);
}

static inline void
vTxEnInit (void) {

  MB_SERIAL_TXEN_DDR |= _BV (MB_SERIAL_TXEN_BIT);
  vTxEnClr ();
}
#  else
#    define vTxEnInit()
#    define vTxEnSet()
#    define vTxEnClr()
#  endif

/* private variables ======================================================== */
#  ifdef MB_SERIAL_DEBUG
static volatile uint8_t ucMBSerialStatus;

#    define READY 0
#    define READY_SET() do {\
    ucMBSerialStatus |= _BV(READY);\
  } while(0)
#    define READY_CLR() do {\
    ucMBSerialStatus &= ~_BV(READY);\
  } while(0)
#  else
#    define READY_SET()
#    define READY_CLR()
#  endif

/* private functions ======================================================== */

// ------------------------------------------------------------------------------
ISR (USART_UDRE_vect) {

#  ifndef MB_SERIAL_DEBUG
  vLedComSet ();
  pxMBFrameCBTransmitterEmpty ();
#  else
  vUARTTxReadyISR ();
#  endif
}

// ------------------------------------------------------------------------------
ISR (USART_RXC_vect) {

#  ifndef MB_SERIAL_DEBUG
  vLedComSet ();
  pxMBFrameCBByteReceived ();
#  else
  vUARTRxReadyISR ();
#  endif
}

// ------------------------------------------------------------------------------
ISR (USART_TXC_vect) {

  vTxEnClr ();
  vLedComClr ();
  UCSRB &= ~(_BV (TXEN) | _BV (TXCIE));
  READY_SET ();
}

/* internal public functions ================================================ */

// ------------------------------------------------------------------------------
void
vMBPortSerialEnable (BOOL xRxEnable, BOOL xTxEnable) {

  if (xRxEnable) {

    UCSRB |= _BV (RXEN) | _BV (RXCIE);
  } else {

    UCSRB &= ~(_BV (RXEN) | _BV (RXCIE));
  }
  if (xTxEnable) {

    UCSRA |= _BV (TXC); /* clear TXC */
    READY_CLR ();
    vTxEnSet ();
    UCSRB |= _BV (TXEN) | _BV (UDRIE) | _BV (TXCIE);  /* start transmitting */
  } else {

    UCSRB &= ~(_BV (UDRIE));
  }
  vLedComClr ();
  vLedErrClr ();
}

// ------------------------------------------------------------------------------
BOOL
xMBPortSerialInit (UCHAR ucPORT __attribute__ ((unused)),
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
  if ((lUBRR <= 0) || (lUBRR > 65535)) {

    return FALSE;
  }
  UBRRL = lUBRR & 0xFF;
  UBRRH = lUBRR >> 8;

  vTxEnInit ();
  vLedErrInit ();
  vLedComInit ();
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

#  ifdef MB_SERIAL_DEBUG
// ------------------------------------------------------------------------------
BOOL
xMBPortSerialReady (void) {
  uint8_t ucValue;

  ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {
    ucValue = ucMBSerialStatus;
  }
  return (ucValue & _BV (READY)) != 0;
}
#  endif

#endif /* AVRIO_MODBUS_ENABLE defined */
/* ========================================================================== */
