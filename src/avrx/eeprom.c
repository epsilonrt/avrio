/**
 * Copyright © 2011-2015 epsilonRT. All rights reserved.
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

