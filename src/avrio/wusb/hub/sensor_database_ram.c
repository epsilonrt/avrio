/**
 * Copyright © 2011-2015 Pascal JEAN aka epsilonRT. All rights reserved.
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
 * @file sensor_database_ram.c
 * @brief Driver base de données capteurs WirelessUSB en RAM
 *
 * Revision History ------------------------------------------------------------
 *    20111204 - Initial version by epsilonRT
 */
#include <avrio/wusb/hub/sensor_database.h>
#include <stdlib.h>

/*
  Performances:
  -------------
   - Temps d'ajout d'un capteur: 35 ṁs 
   - Conditions: SRAM interne / ATmega128 / FCPU = 14.7456 MHZ
 */

/* private variables ======================================================== */
static struct {
  size_t xSize;
  uint8_t * pucData;
} xMem;

/* internal public functions ================================================ */

// -----------------------------------------------------------------------------
int 
iWSdBaseOpen (int Arg1, void * Arg2) {

  xMem.xSize = Arg1;
  xMem.pucData = Arg2;
  return iWSdBaseInit (xMem.xSize);
}

// -----------------------------------------------------------------------------
int 
iWSdBaseRead (void * pvAddress, uint8_t * pucData, size_t xDataLength) {
  size_t xOffset = MIN((size_t)pvAddress, xMem.xSize - 1);
  size_t xLength = MIN(xDataLength, xMem.xSize - xOffset );
  
  memcpy (pucData, xMem.pucData + xOffset, xLength);
  return xLength;
}

// -----------------------------------------------------------------------------
int 
iWSdBaseWrite (void * pvAddress, const uint8_t * pucData, size_t xDataLength) {
  size_t xOffset = MIN((size_t)pvAddress, xMem.xSize - 1);
  size_t xLength = MIN(xDataLength, xMem.xSize - xOffset );
  
  memcpy (xMem.pucData + xOffset, pucData, xLength);
  return xLength;
} 
/* ========================================================================== */
