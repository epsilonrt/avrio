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
 * @file debug.h
 *
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

