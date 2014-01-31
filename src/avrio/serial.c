/**
 * @file serial_irq.c
 * @ingroup serial_module
 * @brief Liaison série asynchrone
 *
 * Dépend des modules:
 * - \ref queue_module (version avec interruption seulement)
 * .
 * @author Copyright © 2011-2012 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * @version $Id$
   ========================================================================== */
#include "avrio-config.h"

#ifdef AVRIO_SERIAL_ENABLE
/* ========================================================================== */
#  include "avrio-board-serial.h"
#  include <avrio/mutex.h>
#  include <avrio/serial.h>
#  include <avrio/queue.h>
#  include <avrio/delay.h>
#  include "serial_private.h"

/* constants ================================================================ */
# define EOL_CR
//# define EOL_LF
//# define EOL_CRLF

// Mutex Bit Mask
# define BUSY _BV(0)
# define ALL_FLAGS (BUSY)

/* private variables ======================================================== */
//static uint16_t usSerialFlags;
uint16_t usSerialFlags;

#ifdef SERIAL_HALF_DUPLEX
  xMutex xSerialMutex = MUTEX_INITIALIZER;
#endif

#if defined(AVRIO_SERIAL_RXIE)
// ------------------------------------------------------------------------------

/* private variables ======================================================== */
//QUEUE_STATIC_DECLARE (xSerialRxQueue, SERIAL_RXBUFSIZE);
QUEUE_DECLARE (xSerialRxQueue, SERIAL_RXBUFSIZE);

/* private functions ======================================================== */

// -----------------------------------------------------------------------------
// Version avec interruption
static inline void
prvvRxInit (void) {

  vQueueFlush (&xSerialRxQueue);
}

// -----------------------------------------------------------------------------
// Version avec interruption
static inline void
prvvRxEnable (void) {

  if (usSerialFlags & SERIAL_RD) {

#ifdef SERIAL_HALF_DUPLEX
    // Attente fin de transmission
    vMutexUntilBitUnlocked (&xSerialMutex, BUSY);
#endif
    UCSRB |= _BV (RXCIE);
  }
}

// -----------------------------------------------------------------------------
// Version avec interruption
static inline void
prvvRxDisable (void) {

  if (usSerialFlags & SERIAL_RD) {

    UCSRB &= ~_BV (RXCIE);
  }
}

// ------------------------------------------------------------------------------
ISR (USART_RXC_vect) {

  if ((UCSRA & (_BV (PE) | _BV (FE))) == 0) {

    vQueuePush (&xSerialRxQueue, UDR);
    /* TODO Que faire si débordement de pile ? */
  }
  else {

    (void) UDR; /* clear usFlags en cas d'erreur */
  }
}

/* internal public functions ================================================ */

// -----------------------------------------------------------------------------
// Version avec interruption
int
iSerialGetChar (void) {
  int iChar = _FDEV_EOF;

  if (usSerialFlags & SERIAL_RD) {

    prvvRxEnable();
    if (usSerialFlags & SERIAL_NOBLOCK) {

      if (xQueueIsEmpty (&xSerialRxQueue) == false)
        iChar = ucQueuePull (&xSerialRxQueue);
    }
    else {

      while (xQueueIsEmpty (&xSerialRxQueue) == true)
        ;
      iChar = ucQueuePull (&xSerialRxQueue);
    }
  }
  return (unsigned int) iChar;
}

// -----------------------------------------------------------------------------
// Version avec interruption
uint16_t
usSerialHit (void) {

  return xQueueLength (&xSerialRxQueue);
}

#else  /* AVRIO_SERIAL_RXIE not defined */
// ------------------------------------------------------------------------------

/* private functions ======================================================== */

// -----------------------------------------------------------------------------
// Version sans interruption
static inline void
prvvRxInit (void) {

}

// -----------------------------------------------------------------------------
// Version sans interruption
static inline void
prvvRxEnable (void) {

  if (usSerialFlags & SERIAL_RD) {

#ifdef SERIAL_HALF_DUPLEX
    // Attente fin de transmission
    vMutexUntilBitUnlocked (&xSerialMutex, BUSY);
#endif
    UCSRB |= _BV(RXEN);
    if ((UCSRA & (_BV (PE) | _BV (FE))) != 0) {

      (void) UDR; /* clear des flags d'erreur */
    }
  }
}

// -----------------------------------------------------------------------------
// Version sans interruption
static inline void
prvvRxDisable (void) {

  if (usSerialFlags & SERIAL_RD) {

    UCSRB &= ~_BV(RXEN);
  }
}

/* internal public functions ================================================ */
static bool
prvbCheckError (void) {

  if ((UCSRA & (_BV (PE) | _BV (FE))) == 0) {

    return false;

  }
  else {

    return true;
  }

}
// -----------------------------------------------------------------------------
// Version sans interruption
int
iSerialGetChar (void) {
  int iChar = _FDEV_EOF;

  if (usSerialFlags & SERIAL_RD) {
    bool bError = false;

    prvvRxEnable();
    do {

      if (usSerialFlags & SERIAL_NOBLOCK) {

        if (UCSRA & _BV (RXC)) {

          iChar = UDR;
          bError = prvbCheckError();
        }
      }
      else {

        while ((UCSRA & _BV (RXC)) == 0)
          ;
        iChar = UDR;
        bError = prvbCheckError();
      }

    } while (bError == true);

    if ((usSerialFlags & SERIAL_ECHO) && (iChar != _FDEV_EOF))
      vSerialPutChar ((char)iChar);
  }

  return (unsigned int) iChar;
}

// -----------------------------------------------------------------------------
// Version sans interruption
uint16_t
usSerialHit (void) {

  return ((UCSRA & _BV (RXC)) != 0);
}

// -----------------------------------------------------------------------------
#endif /* AVRIO_SERIAL_RXIE defined */


#if defined(AVRIO_SERIAL_TXIE)
// -----------------------------------------------------------------------------

/* private variables ======================================================== */
//QUEUE_STATIC_DECLARE (xSerialTxQueue, SERIAL_TXBUFSIZE);
QUEUE_DECLARE (xSerialTxQueue, SERIAL_TXBUFSIZE);

/* private functions ======================================================== */

// -----------------------------------------------------------------------------
// Version avec interruption
static inline void
prvvTxInit (void) {

  vQueueFlush (&xSerialTxQueue);
}

// -----------------------------------------------------------------------------
// Version avec interruption
static inline void
prvvTxEnable (void) {

  if (usSerialFlags & SERIAL_RW) {

#ifdef SERIAL_HALF_DUPLEX
    if (!xMutexBitLocked(&xSerialMutex, BUSY)) {

      // Transmission en cours
      vMutexLockBit (&xSerialMutex, BUSY);
    }
    prvvRxDisable();
#endif

    TXEN_SET ();
    UCSRB &= ~_BV (TXCIE);
    UCSRB |= _BV (UDRIE);
  }
}

// -----------------------------------------------------------------------------
// Version avec interruption
static inline void
prvvTxDisable (void) {

  if (usSerialFlags & SERIAL_RW) {

    UCSRB &= ~(_BV (TXCIE) | _BV (UDRIE));
    TXEN_CLR ();

#ifdef SERIAL_HALF_DUPLEX
    vMutexUnlockBit (&xSerialMutex, BUSY);
//    prvvRxEnable();
#endif
  }
}

// ------------------------------------------------------------------------------
ISR (USART_UDRE_vect) {

  if (xQueueIsEmpty (&xSerialTxQueue) == false) {
    uint8_t ucNextByte;

    ucNextByte = ucQueuePull (&xSerialTxQueue);

#if defined(EOL_CRLF)
    if (ucNextByte == '\n') {

      vQueuePush (&xSerialTxQueue, '\r');
    }
#elif defined(EOL_CR)
    if (ucNextByte == '\n') {

      ucNextByte = '\r';
    }
#elif defined(EOL_LF)
    if (ucNextByte == '\r') {

      ucNextByte = '\n';
    }
#endif

    UDR = ucNextByte;
  }
  else {

    UCSRB &= ~_BV (UDRIE);
    UCSRB |= _BV (TXCIE);
  }
}

// ------------------------------------------------------------------------------
ISR (USART_TXC_vect) {

  prvvTxDisable();
}

/* internal public functions ================================================ */

// -----------------------------------------------------------------------------
// Version avec interruption
void
vSerialPutChar (char c) {

  if (usSerialFlags & SERIAL_RW) {

    while (xQueueIsFull (&xSerialTxQueue))
      ;
    vQueuePush (&xSerialTxQueue, c);
    prvvTxEnable();
  }
}

// -----------------------------------------------------------------------------
// Version avec interruption
void
vSerialPutString (const char *pcString) {

  if (usSerialFlags & SERIAL_RW) {

    while (*pcString) {

      pcString = pcQueuePushString (&xSerialTxQueue, pcString);
      prvvTxEnable();
    }
  }
}

#else  /* AVRIO_SERIAL_TXIE not defined */
// ------------------------------------------------------------------------------
/* private functions ======================================================== */

// -----------------------------------------------------------------------------
// Version sans interruption
static inline void
prvvTxInit (void) {

}

// -----------------------------------------------------------------------------
// Version sans interruption
static inline void
prvvTxEnable (void) {

  if (usSerialFlags & SERIAL_RW) {

#ifdef SERIAL_HALF_DUPLEX
    if (!xMutexBitLocked(&xSerialMutex, BUSY)) {
      // Transmission en cours
      vMutexLockBit (&xSerialMutex, BUSY);
    }
    prvvRxDisable();
#endif

    TXEN_SET ();
  }
}

// -----------------------------------------------------------------------------
// Version sans interruption
static inline void
prvvTxDisable (void) {

  if (usSerialFlags & SERIAL_RW) {

    TXEN_CLR ();

#ifdef SERIAL_HALF_DUPLEX
    vMutexUnlockBit (&xSerialMutex, BUSY);
//    prvvRxEnable();
#endif
  }
}

/* internal public functions ================================================ */

// -----------------------------------------------------------------------------
// Version sans interruption
void
vSerialPutChar (char cChar) {

  if (usSerialFlags & SERIAL_RW) {

    prvvTxEnable();

#if defined(EOL_CRLF)
    if (cChar == '\n')
      vSerialPutChar ('\r');
#elif defined(EOL_CR)
    if (cChar == '\n')
      cChar = '\r';
#elif defined(EOL_LF)
    if (cChar == '\r')
      cChar = '\n';
#endif

    while ((UCSRA & _BV (UDRE)) == 0)
      ;
    UCSRA |= _BV(TXC);
    UDR = cChar;
    while ((UCSRA & _BV (TXC)) == 0)
      ;

    prvvTxDisable();
  }
}

// -----------------------------------------------------------------------------
// Version sans interruption
void
vSerialPutString (const char *pcString) {

  if (usSerialFlags & SERIAL_RW) {
    while (*pcString) {

      vSerialPutChar (*pcString++);
    }
  }
}

// -----------------------------------------------------------------------------
#endif /* AVRIO_SERIAL_TXIE defined */

/* internal public functions ================================================ */

// -----------------------------------------------------------------------------
void
vSerialInit (uint16_t usBaud, uint16_t usFlags) {
  uint16_t usUBRR;

  UCSRB = 0;
  usSerialFlags = 0;

  usUBRR = SERIAL_BAUD_X1 (usBaud);
  if (usUBRR == 0) {

    usUBRR = SERIAL_BAUD_X2 (usBaud);
    UCSRA |= _BV (U2X);
  }
  UBRRL = usUBRR & 0xFF;
  UBRRH = usUBRR >> 8;

  TXEN_INIT ();
  prvvRxInit();
  prvvTxInit();
  vSerialSetFlags (usFlags);
}

// -----------------------------------------------------------------------------
void
vSerialSetFlags (uint16_t usFlags) {

  UCSRC = UCSRC_SEL | (usFlags & ~(SERIAL_ECHO | SERIAL_RW));
  vSerialEnable (usFlags);
  usSerialFlags = usFlags;
}

// -----------------------------------------------------------------------------
void
vSerialEnable (uint16_t usFlags) {

  if ((usFlags & SERIAL_RW) != (usSerialFlags & SERIAL_RW)) {

    while (xSerialReady () == false)
      ;
    if (usFlags & SERIAL_RD) {

      UCSRB |= _BV(RXEN);
    }
    else {

      UCSRB &= ~_BV(RXEN);
    }
    if (usFlags & SERIAL_WR) {

      UCSRB |= _BV(TXEN);
    }
    else {

      UCSRB &= ~_BV(TXEN);
    }
    usSerialFlags = (usFlags & SERIAL_RW) | (usSerialFlags & ~SERIAL_RW);
  }
}

// -----------------------------------------------------------------------------
uint16_t
usSerialGetFlags (void) {

  return usSerialFlags;
}

// -----------------------------------------------------------------------------
bool
xSerialReady (void) {

#ifdef SERIAL_HALF_DUPLEX
  return !xMutexBitLocked (&xSerialMutex, BUSY);
#else
  return true;
#endif
}

/* avr-libc stdio interface ================================================= */
static int prvPutChar (char c, FILE * pxStream);
static int prvGetChar (FILE * pxStream);

FILE xSerialPort = FDEV_SETUP_STREAM (prvPutChar, prvGetChar, _FDEV_SETUP_RW);

// -----------------------------------------------------------------------------
static int
prvPutChar (char c, FILE * pxStream) {

  vSerialPutChar (c);
  return 0;
}

// -----------------------------------------------------------------------------
static int
prvGetChar (FILE * pxStream) {

  clearerr (&xSerialPort);
  return iSerialGetChar ();
}


#endif /* AVRIO_SERIAL_ENABLE defined */
/* ========================================================================== */
