/**
 * @file eeprom.c
 * @brief EEPROM Module (Implémentation C portable sur tous les MCU)
 * @author Copyright © 2011-2012 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>

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

