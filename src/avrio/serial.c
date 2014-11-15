/**
 * @file serial.c
 * @ingroup serial_module
 * @brief Liaison série asynchrone version sans interruption
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
#include "serial_private.h"

/* public variables ======================================================== */
uint16_t usSerialFlags;
#ifdef SERIAL_HALF_DUPLEX
xMutex xSerialMutex = MUTEX_INITIALIZER;
#endif

#ifndef AVRIO_SERIAL_RXIE
/* -----------------------------------------------------------------------------
 *
 *                         Réception sans interruption
 *
 * ---------------------------------------------------------------------------*/

/* private functions ======================================================== */
// -----------------------------------------------------------------------------
static inline void
vRxInit (void) {

}

// -----------------------------------------------------------------------------
static inline void
vRxEnable (void) {

  if (usSerialFlags & SERIAL_RD) {

#ifdef SERIAL_HALF_DUPLEX
    // Attente fin de transmission
    vMutexLock (&xSerialMutex);
#endif
    UCSRB |= _BV (RXEN);
    if ( (UCSRA & (_BV (PE) | _BV (FE))) != 0) {

      (void) UDR; /* clear des flags d'erreur */
    }
  }
}

// -----------------------------------------------------------------------------
static inline void
vRxDisable (void) {

  if (usSerialFlags & SERIAL_RD) {

    UCSRB &= ~_BV (RXEN);
#ifdef SERIAL_HALF_DUPLEX
    vMutexUnlock (&xSerialMutex);
#endif
  }
}

/* internal public functions ================================================ */
static bool
bCheckError (void) {

  if ( (UCSRA & (_BV (PE) | _BV (FE))) == 0) {

    return false;
  }
  else {

    return true;
  }

}
// -----------------------------------------------------------------------------
int
iSerialGetChar (void) {
  int iChar = _FDEV_EOF;

  if (usSerialFlags & SERIAL_RD) {
    bool bError = false;

    vRxEnable();
    vRtsEnable();
    do {

      if (usSerialFlags & SERIAL_NOBLOCK) {

        if (UCSRA & _BV (RXC)) {

          iChar = UDR;
          bError = bCheckError();
        }
      }
      else {

        while ( (UCSRA & _BV (RXC)) == 0)
          ;
        iChar = UDR;
        bError = bCheckError();
      }

    } while (bError == true);
    vRtsDisable();

    if ((usSerialFlags & SERIAL_ECHO) && (iChar != _FDEV_EOF)) {

      (void)iSerialPutChar ( (char) iChar);
    }
  }
  return (unsigned int) iChar;
}

// -----------------------------------------------------------------------------
uint16_t
usSerialHit (void) {

  return ( (UCSRA & _BV (RXC)) != 0);
}

// -----------------------------------------------------------------------------
#endif /* AVRIO_SERIAL_RXIE not defined */





#ifndef AVRIO_SERIAL_TXIE
/* -----------------------------------------------------------------------------
 *
 *                         Transmission sans interruption
 *
 * ---------------------------------------------------------------------------*/

/* private functions ======================================================== */

// -----------------------------------------------------------------------------
static inline void
vTxInit (void) {

}

// -----------------------------------------------------------------------------
static inline void
vTxEnable (void) {

  if (usSerialFlags & SERIAL_WR) {

#ifdef SERIAL_HALF_DUPLEX
    vMutexLock (&xSerialMutex);
#endif

    vTxEnSet ();
  }
}

// -----------------------------------------------------------------------------
static inline void
vTxDisable (void) {

  if (usSerialFlags & SERIAL_WR) {

    vTxEnClear ();

#ifdef SERIAL_HALF_DUPLEX
    vMutexUnlock (&xSerialMutex);
#endif
  }
}

/* internal public functions ================================================ */

// -----------------------------------------------------------------------------
int
iSerialPutChar (char c) {

  if (usSerialFlags & SERIAL_WR) {

#if defined(CONFIG_EOL_CRLF)
    if (c == '\n') {
      (void)iSerialPutChar ('\r');
    }
#elif defined(CONFIG_EOL_CR)
    if (c == '\n') {
      c = '\r';
    }
#elif defined(CONFIG_EOL_LF)
    if (c == '\r') {
      c = '\n';
    }
#endif

    while (bCtsIsEnabled() == false)
      ; // Attente CTS = 0
    vTxEnable();
    while ( (UCSRA & _BV (UDRE)) == 0)
      ;
    UCSRA |= _BV (TXC);
    UDR = c;
    while ( (UCSRA & _BV (TXC)) == 0)
      ;
    vTxDisable();
  }
  else {

    return -1;
  }
  return 0;
}

// -----------------------------------------------------------------------------
void
vSerialPutString (const char *pcString) {

  if (usSerialFlags & SERIAL_WR) {
    while (*pcString) {

      (void)iSerialPutChar (*pcString++);
    }
  }
}
// -----------------------------------------------------------------------------
#endif /* AVRIO_SERIAL_TXIE not defined */

/* internal public functions ================================================ */
#if defined(AVRIO_SERIAL_RXIE) || defined(AVRIO_SERIAL_TXIE)
// TODO: séparation complète des 2 versions à faire
#include "serial_irq.c"
#endif

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

  vRxInit();
  vTxInit();
  vTxEnInit ();
  vSerialSetFlags (usFlags);
  vRtsInit();
  vCtsInit();
}

// -----------------------------------------------------------------------------
void
vSerialSetFlags (uint16_t usFlags) {

  UCSRC = UCSRC_SEL | (usFlags & ~ (SERIAL_ECHO | SERIAL_RW));
  vSerialEnable (usFlags);
  usSerialFlags = usFlags;
}

// -----------------------------------------------------------------------------
void
vSerialEnable (uint16_t usFlags) {

  if ( (usFlags & SERIAL_RW) != (usSerialFlags & SERIAL_RW)) {

    while (xSerialReady () == false)
      ;
    if (usFlags & SERIAL_RD) {

      UCSRB |= _BV (RXEN);
    }
    else {

      UCSRB &= ~_BV (RXEN);
    }
    if (usFlags & SERIAL_WR) {

      UCSRB |= _BV (TXEN);
    }
    else {

      UCSRB &= ~_BV (TXEN);
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
static int PutChar (char c, FILE * pxStream);
static int GetChar (FILE * pxStream);

FILE xSerialPort = FDEV_SETUP_STREAM (PutChar, GetChar, _FDEV_SETUP_RW);

// -----------------------------------------------------------------------------
static int
PutChar (char c, FILE * pxStream) {

  return iSerialPutChar (c);
}

// -----------------------------------------------------------------------------
static int
GetChar (FILE * pxStream) {

  clearerr (&xSerialPort);
  return iSerialGetChar ();
}


#endif /* AVRIO_SERIAL_ENABLE defined */
/* ========================================================================== */
