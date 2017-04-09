/**
 * Copyright © 2017 epsilonRT. All rights reserved.
 *
 * This file is part of AvrIO.
 *
 * Copyright (c) 2011, Atmel Corporation All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * 3. The name of ATMEL may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE EXPRESSLY AND
 * SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * @file tws.c
 * @brief Bus I2C avec coupleur TWS
 *
 * Revision History ------------------------------------------------------------
 *    20170402 - Initial version by epsilonRT from AVR290 8380A-AVR-03/11
 */
#include "tws.h"

/* private functions ======================================================== */

// -----------------------------------------------------------------------------
ISR (TWI_SLAVE_vect) {
  uint8_t status = TWSSRA & 0xC0;

  if (status & _BV (TWDIF)) {

    if (TWSSRA & _BV (TWDIR)) {

      // Master Reads data from slave
      TWSD = ucTwsToMaster(); // This function gets data from the application and loads it into the TWSD (Two Wire Send Data) register

      TWSCRB = (uint8_t) (_BV(TWHNM) | _BV (TWCMD1) | _BV (TWCMD0)); // release SCL
    }
    else {
      // Master Write 
      // Here we read the base address
      // If the base address is with in the range save it, else NACK the Master
      TWSCRB = (uint8_t) (_BV(TWHNM) | _BV (TWCMD1) | _BV (TWCMD0));
      ucTwsFromMaster (TWSD) ; // Callback function: data from master to application
    }
  }
  else if (status & _BV (TWASIF)) {

    // Address match can happen due to Collision
    if (TWSSRA & _BV (TWC)) {

      // Clear the Collision Flag
      TWSSRA = TWSSRA;
      // Wait for any Start Condition
      TWSCRB = (uint8_t) (_BV(TWHNM) | _BV (TWCMD1));
    }
    else {

      // Address Match
      if (TWSSRA & _BV (TWAS)) {
        // Execute ACK and then receive next byte or set TWDIF to send the data
        TWSCRB = (uint8_t) (_BV(TWHNM) | _BV (TWCMD1) | _BV (TWCMD0));
      }
      else {
        // Stop Condition
        // Wait for any Start Condition
        TWSCRB = (uint8_t) (_BV(TWHNM) | _BV (TWCMD1));
      }
    }
  }
}

/* internal public functions ================================================ */

void vTwsBusErrorCheck (void) {
  
  if (TWSSRA & _BV (TWBE)) {
    
    // Clear all Flags
    TWSSRA = TWSSRA;
    // Disable TWI Interface
    TWSCRA = 0x00u;

    // Re-Initialise TWI Slave
    vTwsInit(TWSA);
  }
}
/* ========================================================================== */
