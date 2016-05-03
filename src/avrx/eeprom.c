/**
 * Copyright © 2011-2015 epsilonRT. All rights reserved.
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
 *
 * @file eeprom.c
 * @brief EEPROM Module (Implémentation C portable sur tous les MCU)
 *
 * Revision History
 *    20110830 - Initial version epsilonRT
 */
#include <avrx/private.h>
#include <avr/eeprom.h>

/* internal public functions ================================================ */

// -----------------------------------------------------------------------------
uint8_t 
ucAvrXReadEEProm (const uint8_t * pucAddress) {
  uint8_t ucByte;
  
  vAvrXWaitSemaphore (&xEEPromSem);
    ucByte = eeprom_read_byte (pucAddress);
  vAvrXSetSemaphore (&xEEPromSem);
  
  return ucByte;
}

// -----------------------------------------------------------------------------
uint16_t 
usAvrXReadEEPromWord (const uint16_t * pusAddress) {
  uint16_t usWord;
  
  vAvrXWaitSemaphore (&xEEPromSem);
    usWord = eeprom_read_word (pusAddress);
  vAvrXSetSemaphore (&xEEPromSem);
  
  return usWord;
}

// -----------------------------------------------------------------------------
void vAvrXWriteEEProm 
(uint8_t * pucAddress, uint8_t ucByte) {

  vAvrXWaitSemaphore (&xEEPromSem);
    eeprom_write_byte (pucAddress, ucByte);
  vAvrXSetSemaphore (&xEEPromSem);
}
/* ========================================================================== */

