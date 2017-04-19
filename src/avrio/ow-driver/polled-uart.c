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
 *
 * @file 
 * @brief
 */
#ifdef OW_DRIVER_POLLED_UART

#include "polled.h"
#include "uart.h"
#include "avrio-board-ow.h"

/* private functions ======================================================== */

//------------------------------------------------------------------------------
/** @brief  Write and read one bit to/from the 1-Wire bus. (Polled UART driver)
 *
 *  Writes one bit to the bus and returns the value read from the bus.
 *
 *  \param  outValue    The value to transmit on the bus.
 *
 *  \return The value received by the UART from the bus.
 */
static uint8_t
ucTouchBit (uint8_t outValue) {
  
  // Place the output value in the UART transmit buffer, and wait
  // until it is received by the UART receiver.
  OWU_UDR = outValue;
  while (! (OWU_UCSRA & (1 << OWU_RXC))) {

  }
  // Set the UART Baud Rate back to 115200kbps when finished.
  OWU_UBRRL = OWU_UBRR_115200;
  return OWU_UDR;
}

/* internal public functions ================================================ */

//------------------------------------------------------------------------------
void 
vOwInit(void) {
#ifdef OWU_UMAP
  REMAP |= (OWU_UART_REMAP << OWU_UMAP);
#endif

  // Choose single or double UART speed.
  OWU_UCSRA = (OWU_UART_2X << OWU_U2X);

  // Enable UART transmitter and receiver.
  OWU_UCSRB = (1 << OWU_TXEN) | (1 << OWU_RXEN);

  // Set up asynchronous mode, 8 data bits, no parity, 1 stop bit.
  // (Initial value, can be removed)
#ifdef URSEL
  OWU_UCSRC = (1 << OWU_URSEL) | (1 << OWU_UCSZ1) | (1 << OWU_UCSZ0);
#else
  OWU_UCSRC = (1 << OWU_UCSZ1) | (1 << OWU_UCSZ0);
#endif

  OWU_UBRRL = OWU_UBRR_115200;
}

//------------------------------------------------------------------------------
void 
vOwBitWrite1 (void) {
  
  ucTouchBit (OW_UART_WRITE1);
}

//------------------------------------------------------------------------------
void 
vOwBitWrite0 (void) {
  
  ucTouchBit (OW_UART_WRITE0);
}

//------------------------------------------------------------------------------
bool
vOwBitRead (void) {
  
  // Return 1 if the value received matches the value sent.
  // Return 0 else. (A slave held the bus low).
  return (ucTouchBit (OW_UART_READ_BIT) == OW_UART_READ_BIT);
}

//------------------------------------------------------------------------------
bool
bOwReset (void) {
  
  // Reset UART receiver to clear RXC register.
  OWU_UCSRB &= ~ (1 << OWU_RXEN);
  OWU_UCSRB |= (1 << OWU_RXEN);

  // Set UART Baud Rate to 9600 for Reset/Presence signaling.
  OWU_UBRRL = OWU_UBRR_9600;

  // Return 0 if the value received matches the value sent.
  // return 1 else. (Presence detected)
  return (ucTouchBit (OW_UART_RESET) != OW_UART_RESET);
}

/* ========================================================================== */
#endif /* OW_DRIVER_POLLED_UART defined */
