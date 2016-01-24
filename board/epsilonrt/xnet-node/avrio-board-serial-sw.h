/*
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
#ifndef _AVRIO_BOARD_SERIAL_SW_H_
#define _AVRIO_BOARD_SERIAL_SW_H_
/* ========================================================================== */

#include <avr/io.h>
#include <avr/interrupt.h>

#define SERIAL_SW_RXBUFSIZE 32
#define SERIAL_SW_TXBUFSIZE 64

//#define AVRIO_SERIAL_SW_BAUD 9600
//#define AVRIO_SERIAL_SW_NOBLOCK

#define SERIAL_SW_CS1     1
#define SERIAL_SW_CS8     2
#define SERIAL_SW_CS32    3
#define SERIAL_SW_CS64    4
#define SERIAL_SW_CS128   5
#define SERIAL_SW_CS256   6
#define SERIAL_SW_CS1024  7
#define SERIAL_SW_TIMER_MAX 255

#define SERIAL_SW_TIMER_vect  TIMER2_COMPA_vect
#define SERIAL_SW_INT_vect    INT1_vect

/* Port and pin settings. */
#define SERIAL_SW_RX      3       //< Set pin number for reception (must be an interrupt pin).
#define SERIAL_SW_RX_PORT PORTD   //< Set port for reception.
#define SERIAL_SW_RX_PIN  PIND    //< Set pin for reception.
#define SERIAL_SW_RX_DDR  DDRD    //< Data direction register.

#define SERIAL_SW_TX      4       //< Set pin number for transmission.
#define SERIAL_SW_TX_PORT PORTD   //< Set port for transmission.
#define SERIAL_SW_TX_PIN  PIND    //< Set pin for transmission.
#define SERIAL_SW_TX_DDR  DDRD    //< Data direction register.

// -----------------------------------------------------------------------------
static inline void
vSerialSwTimerEnable (void) {

  TIFR2  |= 1 << OCF2A;
  TIMSK2 |= 1 << OCIE2A;
}

// -----------------------------------------------------------------------------
static inline void
vSerialSwTimerDisable (void) {

  TIMSK2 &= ~(1 << OCIE2A);
}

// -----------------------------------------------------------------------------
static inline void
vSerialSwTimerInit (void) {

  TCCR2A = (1<<WGM21);
}

// -----------------------------------------------------------------------------
static inline void
vSerialSwTimerClear (void) {

  TCNT2 = 0;
}

// -----------------------------------------------------------------------------
static inline void
vSerialSwTimerSet (uint8_t ucOcr) {

  OCR2A = ucOcr;
}

// -----------------------------------------------------------------------------
static inline void
vSerialSwTimerStart (uint8_t ucCS) {

  TCCR2B = ucCS;
}

// -----------------------------------------------------------------------------
static inline void
vSerialSwTimerStop (void) {

  TCCR2B = 0;
}

// -----------------------------------------------------------------------------
static inline void
vSerialSwIntEnable (void) {

  EIFR  |= 1 << INTF1;
  EIMSK |= 1 << INT1;
}

// -----------------------------------------------------------------------------
static inline void
vSerialSwIntDisable (void) {

  EIMSK &= ~(1 << INT1);
}

// -----------------------------------------------------------------------------
static inline void
vSerialSwIntInit (void) {

  // Sets falling edge of INT1 generates interrupt
  EICRA |= 1 << ISC11;
  vSerialSwIntEnable();
}

/* ========================================================================== */
#endif  /* _AVRIO_BOARD_SERIAL_SW_H_ not defined */
