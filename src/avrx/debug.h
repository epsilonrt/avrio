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

