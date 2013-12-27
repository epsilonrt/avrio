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
//static uint8_t ucSerialFlags;
uint8_t ucSerialFlags;

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

  if (ucSerialFlags & SERIAL_RD) {
  
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

  if (ucSerialFlags & SERIAL_RD) {
  
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

    (void) UDR; /* clear ucFlags en cas d'erreur */
  }
}

/* internal public functions ================================================ */

// -----------------------------------------------------------------------------
// Version avec interruption 
char
cSerialGetChar (void) {
  char cChar = 0;

  if (ucSerialFlags & SERIAL_RD) {
  
    prvvRxEnable();
    while (xQueueIsEmpty (&xSerialRxQueue) == true)
      ;
    cChar = ucQueuePull (&xSerialRxQueue);
  }
  return cChar;
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

  if (ucSerialFlags & SERIAL_RD) {
  
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

  if (ucSerialFlags & SERIAL_RD) {
    
    UCSRB &= ~_BV(RXEN);
  }
}

/* internal public functions ================================================ */

// -----------------------------------------------------------------------------
// Version sans interruption 
char
cSerialGetChar (void) {
  char cChar = 0;

  if (ucSerialFlags & SERIAL_RD) {
    bool xError = false;

    prvvRxEnable();
    do {

      while ((UCSRA & _BV (RXC)) == 0)
        ;
      
      cChar = UDR;
      if ((UCSRA & (_BV (PE) | _BV (FE))) == 0) {

        xError = false;

        if (ucSerialFlags & SERIAL_ECHO)
          vSerialPutChar (cChar);
      } 
      else {

        xError = true;
      }
    } while (xError == true);
  }
  
  return cChar;
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

  if (ucSerialFlags & SERIAL_RW) {

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

  if (ucSerialFlags & SERIAL_RW) {
  
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

  if (ucSerialFlags & SERIAL_RW) {

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

  if (ucSerialFlags & SERIAL_RW) {

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

  if (ucSerialFlags & SERIAL_RW) {

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

  if (ucSerialFlags & SERIAL_RW) {
  
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

  if (ucSerialFlags & SERIAL_RW) {
  
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

  if (ucSerialFlags & SERIAL_RW) {
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
vSerialInit (uint16_t usBaud, uint8_t ucFlags) {
  uint16_t usUBRR;

  UCSRB = 0;
  ucSerialFlags = 0;
  
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
  vSerialSetFlags (ucFlags);
}

// -----------------------------------------------------------------------------
void
vSerialSetFlags (uint8_t ucFlags) {

  UCSRC = UCSRC_SEL | (ucFlags & ~(SERIAL_ECHO | SERIAL_RW));
  vSerialEnable (ucFlags);
  ucSerialFlags = ucFlags;
}

// -----------------------------------------------------------------------------
void
vSerialEnable (uint8_t ucFlags) {

  if ((ucFlags & SERIAL_RW) != (ucSerialFlags & SERIAL_RW)) {

    while (xSerialReady () == false) 
      ; 
    if (ucFlags & SERIAL_RD) {
      
      UCSRB |= _BV(RXEN);
    }
    else {
    
      UCSRB &= ~_BV(RXEN);
    }
    if (ucFlags & SERIAL_WR) {
      
      UCSRB |= _BV(TXEN);
    }
    else {
    
      UCSRB &= ~_BV(TXEN);
    }
    ucSerialFlags = (ucFlags & SERIAL_RW) | (ucSerialFlags & ~SERIAL_RW);
  }
}

// -----------------------------------------------------------------------------
uint8_t
ucSerialGetFlags (void) {

  return ucSerialFlags;
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
// -----------------------------------------------------------------------------
static int
prvPutChar (char c, FILE * pxStream) {

  vSerialPutChar (c);
  return 0;
}

// -----------------------------------------------------------------------------
static int
prvGetChar (FILE * pxStream) {

  return (unsigned int) cSerialGetChar ();
}

FILE xSerialPort = FDEV_SETUP_STREAM (prvPutChar, prvGetChar, _FDEV_SETUP_RW);

#endif /* AVRIO_SERIAL_ENABLE defined */
/* ========================================================================== */
