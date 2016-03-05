/**
 * @file tc.c
 * @brief Liaison série asynchrone version
 *
 * Copyright © 2011-2015 Pascal JEAN aka epsilonRT. All rights reserved.
 *
 * This file is part of AvrIO.
 *
 * AvrIO is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * AvrIO is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with AvrIO.  If not, see <http://www.gnu.org/licenses/lgpl.html>
 */
#ifdef AVRIO_TC_ENABLE
/* ========================================================================== */
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "tc_private.h"

/* public variables ======================================================== */
int iTcError;

/* private functions ======================================================== */

// -----------------------------------------------------------------------------
// Configure: parité, stop bits, data bits
static inline int
iSetConfig (const xTcPort * p) {
  uint8_t ucUCSRB = UCSRC_SEL;

  switch (p->ios.dbits) {

    case SERIAL_DATABIT_5:
      break;

    case SERIAL_DATABIT_6:
      ucUCSRB |= _BV (UCSZ0);
      break;

    case SERIAL_DATABIT_7:
      ucUCSRB |= _BV (UCSZ1);
      break;

    case SERIAL_DATABIT_8:
      ucUCSRB |= _BV (UCSZ1) | _BV (UCSZ0);
      break;

#ifdef UCSZ2
    case SERIAL_DATABIT_9:
      ucUCSRB  |= _BV (UCSZ1) | _BV (UCSZ0);
      TC_UCSRC |= _BV (UCSZ2);
      break;
#endif

    default:
      iTcError = eTcDataBitsUnknown;
      return -1;
  }

  switch (p->ios.parity) {

    case SERIAL_PARITY_NONE:
      break;

    case SERIAL_PARITY_EVEN:
      ucUCSRB |= _BV (UPM1);
      break;

    case SERIAL_PARITY_ODD:
      ucUCSRB |= _BV (UPM1) | _BV (UPM0);
      break;

    default:
      iTcError = eTcParityUnknown;
      return -1;
  }

  switch (p->ios.sbits) {

    case SERIAL_STOPBIT_ONE:
      break;

    case SERIAL_STOPBIT_TWO:
      ucUCSRB |= _BV (USBS);
      break;

    default:
      iTcError = eTcStopBitsUnknown;
      return -1;
  }

  TC_UCSRC = ucUCSRB;
  return 0;
}

// -----------------------------------------------------------------------------
static int
iSetupUart (xTcPort * p) {
  uint32_t usUBRR;

  TC_UCSRB = 0;
  TC_UCSRA = 0;
  TC_UCSRC = 0;

  // Calcul de UBRR
  usUBRR = (AVRIO_CPU_FREQ / 8UL);
  usUBRR /= p->ios.baud;

#if defined(AVRIO_TC_BAUD_USE_X2)
  // Utilisation exclusive de X2
  TC_UCSRA |= _BV (U2X);
#else
  if (usUBRR > 4096) {

    // usUBRR trop grand, on passe en X1
    usUBRR /= 2;
    TC_UCSRA &= ~_BV (U2X);
  }
  else {

    TC_UCSRA |= _BV (U2X);
  }
#endif
  usUBRR--;
  TC_UBRRL = usUBRR & 0xFF;
  TC_UBRRH = (usUBRR >> 8) & 0x0F;

  if (iSetConfig (p) != 0) {

    return -1;
  }

#ifdef TC_RXTX_PULLUP_ENABLE
  vDpSetMode (&p->io.txd, eModeInputPullUp);
  vDpSetMode (&p->io.rxd, eModeInputPullUp);
#endif

  vTxenInit (p);
  vRxenInit (p);
  vRtsInit (p);
  vCtsInit (p);
  vTcPrivInit (p);
  vTcPrivRxEn ( (p->hook->flag & O_RD) != 0, p);
  return 0;
}

#if 0
// TODO Utile ?
// -----------------------------------------------------------------------------
static void
vTcSetMode (int mode, xTcPort * p) {

  if ( (mode & O_RDWR) != (p->hook->flag & O_RDWR)) {

    if (mode & O_RD) {

      TC_UCSRB |= _BV (RXEN);
    }
    else {

      TC_UCSRB &= ~_BV (RXEN);
    }
    if (mode & O_WR) {

      TC_UCSRB |= _BV (TXEN);
    }
    else {

      TC_UCSRB &= ~_BV (TXEN);
    }
    p->hook->flag = (mode & O_RDWR) | (p->hook->flag & ~O_RDWR);
  }
}
#endif

// -----------------------------------------------------------------------------
int
iTcRxError (xTcPort * p) {
  uint8_t s = TC_UCSRA & (_BV (PE) | _BV (FE));

  if (s & _BV (PE)) {

    iTcError |= eTcRxParityError;
  }
  if (s & _BV (FE)) {

    iTcError |= eTcRxFormatError;
  }
  return iTcError & (eTcRxParityError | eTcRxFormatError);
}

// -----------------------------------------------------------------------------
static int
iTcPutChar (char c, FILE * f) {
  xTcPort * p = (xTcPort *) pvFileDevice (f);

  if (p->hook->flag & O_WR) {

    switch (p->ios.eol) {

      case SERIAL_CRLF:
        if (c == '\n') {
          if (iTcPrivPutChar ('\r', p) != 0) {

            return _FDEV_EOF;
          }
        }
        break;

      case SERIAL_LF:
        if (c == '\r') {
          c = '\n';
        }
        break;

      case SERIAL_CR:
        if (c == '\n') {
          c = '\r';
        }
        break;
      default:
        break;
    }
    return iTcPrivPutChar (c, p);
  }

  return _FDEV_EOF;
}

// -----------------------------------------------------------------------------
static int
iTcGetChar (FILE * f) {
  int c = _FDEV_EOF;
  xTcPort * p = (xTcPort *) pvFileDevice (f);

  clearerr (f);
  if (p->hook->flag & O_RD) {

    c = iTcPrivGetChar (p);
    if ( (p->hook->flag & O_ECHO) && (c != _FDEV_EOF)) {

      (void) iTcPutChar (c, f);
    }
  }
  return (unsigned int) c;
}

// -----------------------------------------------------------------------------
static int
iTcIoCtl (FILE * f, int c, va_list ap) {
  xTcPort * p = (xTcPort *) pvFileDevice (f);

  switch (c) {

    case FIOFLUSH:
      vTcPrivFlush (p);
      break;

    case FIONREAD:
      return usTcPrivDataAvailable (p);
      break;

    case FIOGETS: {

      xSerialIos * ios = va_arg (ap, xSerialIos*);
      memcpy (ios, &p->ios, sizeof (xSerialIos));
    }
    break;

    case FIOSETS: {

      xSerialIos * ios = va_arg (ap, xSerialIos*);
      memcpy (&p->ios, ios, sizeof (xSerialIos));
      return iSetupUart (p);
    }
    break;

    default:
      return -1;
  }
  return 0;
}

// -----------------------------------------------------------------------------
static int
iTcClose (FILE * f) {
#if TC_NUMOF_PORT > 1
  xTcPort * p = (xTcPort *) pvFileDevice (f);
#endif

  TC_UCSRB = 0;
  return 0;
}

/* private variables ======================================================== */
static xTcPort xTcPorts[] = {
#ifdef TC0_IO
  { .io = TC0_IO, .uart = 0 },
#endif
#ifdef TC1_IO
  { .io = TC1_IO, .uart = 1 },
#endif
#ifdef TC2_IO
  { .io = TC2_IO, .uart = 2 },
#endif
#ifdef TC3_IO
  { .io = TC3_IO, .uart = 3 },
#endif
};

static xFileHook xTcHooks[TC_NUMOF_PORT] = {
  { .dev = &xTcPorts[0], .ioctl = iTcIoCtl, .close = iTcClose },
#if TC_NUMOF_PORT > 1
  { .dev = &xTcPorts[1], .ioctl = iTcIoCtl, .close = iTcClose },
#endif
#if TC_NUMOF_PORT > 2
  { .dev = &xTcPorts[2], .ioctl = iTcIoCtl, .close = iTcClose },
#endif
#if TC_NUMOF_PORT > 3
  { .dev = &xTcPorts[3], .ioctl = iTcIoCtl, .close = iTcClose },
#endif
};

static FILE xTcFile[TC_NUMOF_PORT] = {
  { .udata = &xTcHooks[0], .put = iTcPutChar, .get = iTcGetChar },
#if TC_NUMOF_PORT > 1
  { .udata = &xTcHooks[1], .put = iTcPutChar, .get = iTcGetChar },
#endif
#if TC_NUMOF_PORT > 2
  { .udata = &xTcHooks[2], .put = iTcPutChar, .get = iTcGetChar },
#endif
#if TC_NUMOF_PORT > 3
  { .udata = &xTcHooks[3], .put = iTcPutChar, .get = iTcGetChar },
#endif
};

/* internal public functions ================================================ */

// -----------------------------------------------------------------------------
// name au format tty*
FILE *
xTcOpen (const char * name, int flag, xSerialIos * ios) {
  int8_t i;

#if TC_NUMOF_PORT > 1
  if ( (strlen (name) > 3) && (isdigit (name[3]))) {

    i = name[3] - '0'; // ASCII -> binaire
    if (i >= TC_NUMOF_PORT) {

      return NULL;
    }
  }
  else {

    return NULL;
  }
#else
  i = 0;
#endif /* TC_NUMOF_PORT > 1 */

  memcpy (&xTcPorts[i].ios, ios, sizeof (xSerialIos));
  xTcPorts[i].inode = i;
  xTcHooks[i].flag = flag;
  xTcPorts[i].hook = &xTcHooks[i];
  // Modifie les flags du fichier stdio pour valider écriture/lecture
  xTcFile[i].flags = 0;

  if (flag & O_RD) {

    xTcFile[i].flags |= __SRD;
  }
  if (flag & O_WR) {

    xTcFile[i].flags |= __SWR;
  }

  if (iSetupUart (&xTcPorts[i]) == 0) {

    return &xTcFile[i];
  }
  return NULL;
}

/* ========================================================================== */
#endif /* AVRIO_TC_ENABLE defined */
