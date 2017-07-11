/**
 * Copyright © 2017 epsilonRT. All rights reserved.
 *
 * This file is part of AvrIO.
 *
 * This software is governed by the CeCILL license under French law and
 * abiding by the rules of distribution of free software.  You can  use,
 * modify and/ or redistribute the software under the terms of the CeCILL
 * license as circulated by CEA, CNRS and INRIA at the following URL
 * <http://www.cecill.info>.
 *
 * The fact that you are presently reading this means that you have had
 * knowledge of the CeCILL license and that you accept its terms.
 * -----------------------------------------------------------------------------
 * This file was derived from the usitwislave project on
 * <https://github.com/eriksl/usitwislave> published by Erik Slagter :
 *
 * Copyright © 2013 Erik Slagter <erik@slagter.name>
 * usitwislave is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * usitwislave is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with usitwislave. If not, see <http://www.gnu.org/licenses/>
 *
 * @file
 * @brief
 */
#include <avr/sleep.h>
#include <util/atomic.h>
#include "usislave.h"
#include "avrio-board-usi.h"


/* constants ================================================================ */
enum {
  of_state_check_address,
  of_state_send_data,
  of_state_request_ack,
  of_state_check_ack,
  of_state_receive_data,
  of_state_store_data_and_send_ack
} overflow_state_t;

enum {
  ss_state_before_start,
  ss_state_after_start,
  ss_state_address_selected,
  ss_state_address_not_selected,
  ss_state_data_processed
} startstop_state_t;

/* private variables ======================================================== */
static ucUsiSlaveRxCallback usi_rxcb;

static uint8_t of_state;
static uint8_t ss_state;

static uint8_t usi_flags;
static uint8_t  usi_slave_addr;

static uint8_t  * usi_txbuf;
static uint8_t  usi_txbuf_len;
static uint8_t  usi_txbuf_index;
static uint8_t  usi_rxbuf[USI_TWI_BUFFER_SIZE];
static uint8_t  usi_rxbuf_len;

/* private functions ======================================================== */
//------------------------------------------------------------------------------
static void
vSetSdaToInput (void) {

  DDR_USI &= ~_BV (PORT_USI_SDA);
}

//------------------------------------------------------------------------------
static void
vSetSdaToOutput (void) {

  DDR_USI |= _BV (PORT_USI_SDA);
}

//------------------------------------------------------------------------------
static inline void
vSetSclToInput (void) {

  DDR_USI &= ~_BV (PORT_USI_SCL);
}

//------------------------------------------------------------------------------
static inline void
vSetSclToOutput (void) {

  DDR_USI |= _BV (PORT_USI_SCL);
}

//------------------------------------------------------------------------------
static inline void
vSetSdaLow (void) {

  PORT_USI &= ~_BV (PORT_USI_SDA);
}

//------------------------------------------------------------------------------
static inline void
vSetSdaHigh (void) {

  PORT_USI |= _BV (PORT_USI_SDA);
}

//------------------------------------------------------------------------------
static inline void
vSetSclLow (void) {

  PORT_USI &= ~_BV (PORT_USI_SCL);
}

//------------------------------------------------------------------------------
static inline void
vSetSclHigh (void) {

  PORT_USI |= _BV (PORT_USI_SCL);
}

//------------------------------------------------------------------------------
static inline void
vEnable (void) {
  USISR =
    (1    << USISIF)  |   // clear start condition flag
    (1    << USIOIF)  |   // clear overflow condition flag
    (0    << USIPF) |   // !clear stop condition flag
    (1    << USIDC) |   // clear arbitration error flag
    (0x00 << USICNT0);    // set counter to "8" bits

  USICR =
    (1 << USISIE) |                 // enable start condition interrupt
    (0 << USIOIE) |                 // !enable overflow interrupt
    (1 << USIWM1) | (0 << USIWM0) |         // set usi in two-wire mode, disable bit counter overflow hold
    (1 << USICS1) | (0 << USICS0) | (0 << USICLK) | // shift register clock source = external, positive edge, 4-bit counter source = external, both edges
    (0 << USITC);                 // don't toggle clock-port pin
}

//------------------------------------------------------------------------------
static void
vReset (void) {
  // make sure no sda/scl remains pulled up or down

  vSetSdaToInput();   //  deactivate internal pullup on sda/scl
  vSetSdaLow();
  vSetSclToInput();
  vSetSclLow();

  vSetSdaToOutput();  //  release (set high) on sda/scl
  vSetSdaHigh();
  vSetSdaToInput();
  vSetSdaHigh();
  vSetSclToOutput();
  vSetSclHigh();

  vEnable();
}

//------------------------------------------------------------------------------
static inline void
vInit (void) {

#if defined(USIPP)
#if  defined(USI_ON_PORT_A)
  USIPP |= _BV (USIPOS);
#else
  USIPP &= ~_BV (USIPOS);
# endif
#endif

  vReset();
}

//------------------------------------------------------------------------------
ISR (USI_START_vect) {
  vSetSdaToInput();

  // wait for SCL to go low to ensure the start condition has completed (the
  // start detector will hold SCL low) - if a stop condition arises then leave
  // the interrupt to prevent waiting forever - don't use USISR to test for stop
  // condition as in Application Note AVR312 because the stop condition Flag is
  // going to be set from the last TWI sequence

  while (! (PIN_USI & _BV (PIN_USI_SDA)) &&
         (PIN_USI & _BV (PIN_USI_SCL)))

    // possible combinations
    //  sda = low scl = low   break start condition
    //  sda = low scl = high    loop
    //  sda = high  scl = low   break stop condition
    //  sda = high  scl = high    break stop condition

    if ( (PIN_USI & _BV (PIN_USI_SDA))) { // stop condition
      vReset();

      return;
    }


  of_state = of_state_check_address;
  ss_state = ss_state_after_start;

  USIDR = 0xff;

  USICR =
    (1 << USISIE) |                 // enable start condition interrupt
    (1 << USIOIE) |                 // enable overflow interrupt
    (1 << USIWM1) | (1 << USIWM0) |         // set usi in two-wire mode, enable bit counter overflow hold
    (1 << USICS1) | (0 << USICS0) | (0 << USICLK) | // shift register clock source = external, positive edge, 4-bit counter source = external, both edges
    (0 << USITC);                 // don't toggle clock-port pin

  USISR =
    (1    << USISIF)  |   // clear start condition flag
    (1    << USIOIF)  |   // clear overflow condition flag
    (0    << USIPF) |   // !clear stop condition flag
    (1    << USIDC) |   // clear arbitration error flag
    (0x00 << USICNT0);    // set counter to "8" bits
}

//------------------------------------------------------------------------------
ISR (USI_OVERFLOW_VECTOR) {
  // bit shift register overflow condition occured
  // scl forced low until overflow condition is cleared!

  uint8_t data    = USIDR;
  uint8_t set_counter = 0x00;   // send 8 bits (16 edges)


again:
  switch (of_state) {
      // start condition occured and succeed
      // check address, if not OK, reset usi
      // note: not using general call address

    case (of_state_check_address) : {
      uint8_t address;
      uint8_t direction;

      direction = data & 0x01;
      address   = (data & 0xfe) >> 1;

      if (address == usi_slave_addr) {
        ss_state = ss_state_address_selected;

        if (direction) {      // read request from master
          of_state = of_state_send_data;
        }
        else {            // write request from master
          of_state = of_state_receive_data;
        }

        USIDR   = 0x00;
        set_counter = 0x0e;       // send 1 bit (2 edges)
        vSetSdaToOutput();      // initiate send ack
      }
      else {
        USIDR   = 0x00;
        set_counter = 0x00;
        vEnable();
        ss_state = ss_state_address_not_selected;
      }

      break;
    }

    // process read request from master

    case (of_state_send_data) : {
      ss_state = ss_state_data_processed;
      of_state = of_state_request_ack;

      if (usi_txbuf_index >= usi_txbuf_len) {
        usi_txbuf_index = 0;
      }
      USIDR = usi_txbuf[usi_txbuf_index++];

      set_counter = 0x00;
      vSetSdaToOutput();        // initiate send data

      break;
    }

    // data sent to master, request ack (or nack) from master

    case (of_state_request_ack) : {
      of_state = of_state_check_ack;

      USIDR   = 0x00;
      set_counter = 0x0e;         //  receive 1 bit (2 edges)
      vSetSdaToInput();         //  initiate receive ack

      break;
    }

    // ack/nack from master received

    case (of_state_check_ack) : {
      if (data) { // if NACK, the master does not want more data
        of_state = of_state_check_address;
        set_counter = 0x00;
        vReset();
      }
      else {
        of_state = of_state_send_data;
        goto again; // from here we just drop straight into state_send_data
      }       // don't wait for another overflow interrupt

      break;
    }

    // process write request from master

    case (of_state_receive_data) : {
      ss_state = ss_state_data_processed;

      of_state = of_state_store_data_and_send_ack;

      set_counter = 0x00;         // receive 1 bit (2 edges)
      vSetSdaToInput();         // initiate receive data

      break;
    }

    // data received from master, store it and wait for more data

    case (of_state_store_data_and_send_ack) : {
      of_state = of_state_receive_data;

      if (usi_rxbuf_len < (USI_TWI_BUFFER_SIZE - 1)) {
        usi_rxbuf[usi_rxbuf_len++] = data;
      }

      USIDR   = 0x00;
      set_counter = 0x0e;         // send 1 bit (2 edges)
      vSetSdaToOutput();        // initiate send ack

      break;
    }
  }

  USISR =
    (0        << USISIF)  |   // don't clear start condition flag
    (1        << USIOIF)  |   // clear overflow condition flag
    (0        << USIPF) |   // don't clear stop condition flag
    (1        << USIDC) |   // clear arbitration error flag
    (set_counter  << USICNT0);    // set counter to 8 or 1 bits
}

/* internal public functions ================================================ */

//------------------------------------------------------------------------------
void
vUsiSlaveInit (uint8_t slave_addr, uint8_t flags, uint8_t *tx_buffer,
               uint8_t tx_buffer_len, ucUsiSlaveRxCallback rx_cb) {

  usi_slave_addr = slave_addr;
  usi_rxcb = rx_cb;
  usi_flags = flags;
  usi_txbuf = tx_buffer;
  usi_txbuf_len = tx_buffer_len;
  usi_txbuf_index = 0;
  ss_state = ss_state_before_start;

  if (_BV (USIF_USE_SLEEP) & usi_flags) {
    set_sleep_mode (SLEEP_MODE_IDLE);
  }

  vInit();
}

//------------------------------------------------------------------------------
void
vUsiSlaveEnable (bool enable) {

  if (enable) {

    vEnable();
  }
  else {

    USICR = 0;
  }
}

//------------------------------------------------------------------------------
void
vUsiSlaveSetTxBufferIndex (uint8_t index) {

  index %= usi_txbuf_len;
  ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {
    usi_txbuf_index = index;
  }
}

//------------------------------------------------------------------------------
void
vUsiSlavePoll (void) {
  uint8_t call_datacallback = 0;

  if ( (_BV (USIF_USE_SLEEP) & usi_flags) && (ss_state == ss_state_before_start)) {
    sleep_mode();
  }

  if (USISR & _BV (USIPF)) {

    ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {

      USISR |= _BV (USIPF); // clear stop condition flag

      switch (ss_state) {

        case (ss_state_after_start) : {
          vReset();
          break;
        }

        case (ss_state_data_processed) : {
          call_datacallback = 1;
          break;
        }
      }

      ss_state = ss_state_before_start;
    }
  }

  if (call_datacallback && usi_rxbuf_len) {

    if (usi_rxcb) {
      usi_rxcb (usi_rxbuf, usi_rxbuf_len);
    }
    usi_rxbuf_len   = 0;
  }
}

/* ========================================================================== */
