/**
 * @file debug.h
 * @author Copyright © 2011-2012 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * Revision History ------------------------------------------------------------
 *    20110905 - Initial version by epsilonRT
 * ---
 * @brief Module pour le débugage
 */

#ifndef _AVRX_DEBUG_H_
#define _AVRX_DEBUG_H_

void vDebugInit (uint16_t UBRValue);
void vDebugPutChar (char);
char cDebugGetChar (void);
char cDebugGetCharEcho (void); // Echo's charactor
void vDebugPutCR (void);
void vDebugPutSpace (void);
const char * vDebugPutString (const char *FlashString);
uint16_t usDebugGetHexWord (void);
uint16_t ucDebugGetHexByte (void);
uint16_t ucDebugGetHexDigit (void);
uint8_t ucDebugGetError (void);
void vDebugPutHexWord (uint16_t val);
void vDebugPutHexByte (uint8_t val);
void vDebugPutHexDigit (uint8_t val);
void vDebugMonitor (void);

/* ========================================================================== */
#endif  /* _AVRX_DEBUG_H_ not defined */

