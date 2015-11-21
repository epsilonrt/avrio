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
#include "tc_private.h"

/* public variables ======================================================== */
int iTcError;
int8_t iTcNodes = 0;

/* private functions ======================================================== */

// -----------------------------------------------------------------------------
// Configure: parité, stop bits, data bits
static int
iSetConfig (const xTcPort * p) {
  uint8_t ucUCSRB = UCSRC_SEL;

  switch (p->ios.dbits) {

    case TC_DATABIT_5:
      break;

    case TC_DATABIT_6:
      ucUCSRB |= _BV (UCSZ0);
      break;

    case TC_DATABIT_7:
      ucUCSRB |= _BV (UCSZ1);
      break;

    case TC_DATABIT_8:
      ucUCSRB |= _BV (UCSZ1) | _BV (UCSZ0);
      break;

#ifdef UCSZ2
    case TC_DATABIT_9:
      ucUCSRB  |= _BV (UCSZ1) | _BV (UCSZ0);
      TC_UCSRC |= _BV (UCSZ2);
      break;
#endif

    default:
      iTcError = eTcDataBitsUnknown;
      return -1;
  }

  switch (p->ios.parity) {

    case TC_PARITY_NONE:
      break;

    case TC_PARITY_EVEN:
      ucUCSRB |= _BV (UPM1);
      break;

    case TC_PARITY_ODD:
      ucUCSRB |= _BV (UPM1) | _BV (UPM0);
      break;

    default:
      iTcError = eTcParityUnknown;
      return -1;
  }

  switch (p->ios.sbits) {

    case TC_STOPBIT_ONE:
      break;

    case TC_STOPBIT_TWO:
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
  uint16_t usUBRR;

  TC_UCSRB = 0;
  TC_UCSRA = 0;
  TC_UCSRC = 0;

#if defined(AVRIO_TC_BAUD_USE_X2)
  // Utilisation exclusive de X2
  usUBRR = TC_BAUD_X2 (p->ios.baud);
  TC_UCSRA |= _BV (U2X);
#else
  usUBRR = TC_BAUD_X1 (p->ios.baud);
  if (usUBRR < 32768) {
    // Vitesse trop grande, on passe en X2
    usUBRR = TC_BAUD_X2 (p->ios.baud);
    TC_UCSRA |= _BV (U2X);
  }
#endif

  TC_UBRRL = usUBRR & 0xFF;
  TC_UBRRH = usUBRR >> 8;

  if (iSetConfig (p) != 0) {

    return -1;
  }

#if 0
  // TODO
#if defined(USART_TXPIN)
  // Permet de mettre en sortie à 0 la broche TXD si TXEN=0
  // USART_PORT &= ~_BV (USART_TXPIN);
  // Permet de mettre en sortie à 1 la broche TXD si TXEN=0
  USART_PORT |= _BV (USART_TXPIN);
  USART_DDR  |= _BV (USART_TXPIN);
#endif
#if defined(USART_RXPIN)
  // Permet de mettre en entrée avec pull-up la broche RXD si RXEN=0
  USART_PORT |= _BV (USART_RXPIN);
  USART_DDR  &= ~_BV (USART_RXPIN);
#endif
#endif

  vTxEnInit (p);
  vRxEnInit (p);
  vRtsInit (p);
  vCtsInit (p);
  vTcPrivateInit (p);
  vTcPrivateRxEn ( (*p->flag & O_RD) != 0, p);
  return 0;
}

// -----------------------------------------------------------------------------
static void
vTcSetMode (int mode, xTcPort * p) {

  if ( (mode & O_RDWR) != (*p->flag & O_RDWR)) {

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
    *p->flag = (mode & O_RDWR) | (*p->flag & ~O_RDWR);
  }
}

// -----------------------------------------------------------------------------
bool
bTcIsRxError (xTcPort * p) {
  uint8_t s = TC_UCSRA & (_BV (PE) | _BV (FE));

  if (s & _BV (PE)) {

    iTcError |= eTcRxParityError;
  }
  if (s & _BV (FE)) {

    iTcError |= eTcRxFormatError;
  }
  return s != 0;
}

// -----------------------------------------------------------------------------
static int
iTcPutChar (char c, FILE * f) {
  xTcPort * p = (xTcPort *) pvFileDevice(f);

  if (*p->flag & O_WR) {

#if TC_EOL == TC_CRLF
    if (c == '\n') {
      if (iTcPrivatePutChar ('\r', p) != 0) {

        return _FDEV_EOF;
      }
    }
#elif TC_EOL == TC_LF
    if (c == '\r') {
      c = '\n';
    }
#elif TC_EOL == TC_CR
    if (c == '\n') {
      c = '\r';
    }
#endif

    return iTcPrivatePutChar (c, p);
  }

  return _FDEV_EOF;
}

// -----------------------------------------------------------------------------
static int
iTcGetChar (FILE * f) {
  int c = _FDEV_EOF;
  xTcPort * p = (xTcPort *) pvFileDevice(f);

  clearerr (f);
  if (*p->flag & O_RD) {

    c = iTcPrivateGetChar (p);
    if ( (*p->flag & O_ECHO) && (c != _FDEV_EOF)) {

      (void) iTcPutChar (c, f);
    }
  }
  return (unsigned int) c;
}


// -----------------------------------------------------------------------------
static int
iTcIoCtl (FILE * f, int c, va_list ap) {
  xTcPort * p = (xTcPort *) pvFileDevice(f);

  switch (c) {
    
    case FIOFLUSH:
      vTcFlush (p);
      break;

    case FIONREAD:
      return usTcHit (p);
      break;

    case FIOGETS: {

      xTcIos * ios = va_arg (ap, xTcIos*);
      memcpy (ios, &p->ios, sizeof (xTcIos));
    }
    break;

    case FIOSETS: {

      xTcIos * ios = va_arg (ap, xTcIos*);
      memcpy (&p->ios, ios, sizeof (xTcIos));
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

  return -1;
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

#ifdef TC_HAS_IOBLOCK
static xFileHook xTcHooks[TC_NUMOF_PORT] = {
  { .dev = &xTcPorts[0], .ioctl = iTcIoCtl },
#if TC_NUMOF_PORT > 1
  { .dev = &xTcPorts[1], .ioctl = iTcIoCtl },
#endif
#if TC_NUMOF_PORT > 2
  { .dev = &xTcPorts[2], .ioctl = iTcIoCtl },
#endif
#if TC_NUMOF_PORT > 3
  { .dev = &xTcPorts[3], .ioctl = iTcIoCtl },
#endif
};
#else
static xFileHook xTcHooks[TC_NUMOF_PORT] = {
{ .ioctl = iTcIoCtl }};
#endif

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
FILE *
xTcOpen (const char * name, int flag, xTcIos * ios) {
  int8_t i = 0;

#if TC_NUMOF_PORT > 1
  char * endptr;
  i = strtoul (&name[3], &endptr, 10);
  if (*endptr != '\0') {

    return NULL;
  }
#endif /* TC_NUMOF_PORT > 1 */

  memcpy (&xTcPorts[i].ios, ios, sizeof (xTcIos));
  xTcHooks[i].flag = flag;
  xTcPorts[i].flag = &xTcHooks[i].flag;
  // Modifie les flags du fichier stdio pour valider écriture/lecture
  xTcFile[i].flags = 0;
  
  if (flag & O_RD) {
    
    xTcFile[i].flags |= __SRD;
  }
  if (flag & O_WR) {
    
    xTcFile[i].flags |= __SWR;
  }

  if (iSetupUart (&xTcPorts[i]) == 0) {
    
    xTcHooks[i].inode = iTcNodes++;
    return &xTcFile[i];
  }
  return NULL;
}

/* ========================================================================== */
#endif /* AVRIO_TC_ENABLE defined */
