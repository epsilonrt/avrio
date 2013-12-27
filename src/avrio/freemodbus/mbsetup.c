/* Copyright © 2009-2012 epsilonRT. All rights reserved.
 *  $Id$ */
/* ========================================================================== */

/* ----------------------- AVR includes ------------------------------------- */
#include <avr/io.h>

/* ----------------------- Platform includes -------------------------------- */
#include "port.h"
#include "mbsetup.h"
#include "avrio-config.h"

/* internal public functions ================================================ */

// ------------------------------------------------------------------------------
void
vMBSetupInit (void) {

  // Switch Slave Address (PORTA)
  DDRA = 0;
  PORTA = 0xFF; // résistances pull-up ON
  // Switch BaudRate (PB7-PB5)
  DDRB &= ~(_BV (7) | _BV (6) | _BV (5));
  PORTB |= (_BV (7) | _BV (6) | _BV (5)); // résistances pull-up ON
  // Switch Parity (PC7-PC6)
  DDRC &= ~(_BV (7) | _BV (6));
  PORTC |= (_BV (7) | _BV (6)); // résistances pull-up ON
}

// ------------------------------------------------------------------------------
UCHAR
ucMBSetupSlaveAddress (void) {
  UCHAR ucSwitch = ~(PINA);

  return ucSwitch;
}

// ------------------------------------------------------------------------------
ULONG
ulMBSetupBaudRate (void) {
  UCHAR ucSwitch = ((~PINB) >> 5) & 0b00000111;
  ULONG ulBaudRate = 9600;

  switch (ucSwitch) {

    default:
    case 0:
      break;
    case 1:
      ulBaudRate = 19200;
      break;
    case 2:
      ulBaudRate = 38400;
      break;
    case 3:
      ulBaudRate = 57600;
      break;
    case 4:
      ulBaudRate = 115200;
      break;
    case 5:
      ulBaudRate = 230400;
      break;
    case 6:
      ulBaudRate = 460800;
      break;
    case 7:
      ulBaudRate = 921600;
      break;
  }
  return ulBaudRate;
}

// ------------------------------------------------------------------------------
eMBParity
eMBSetupParity (void) {
  UCHAR ucSwitch = ((~PINC) >> 6) & 0b00000011;

  switch (ucSwitch) {

    case 0:
      return MB_PAR_EVEN;
    case 1:
      return MB_PAR_ODD;
    default:
      break;
  }
  return MB_PAR_NONE;
}

/* ========================================================================== */
