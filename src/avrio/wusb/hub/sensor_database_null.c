
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
 * @file sensor_database_null.c
 * @brief Driver base de données capteurs WirelessUSB null
 *
 * Revision History ------------------------------------------------------------
 *    20111205 - Initial version by epsilonRT
 */
#include <avrio/wusb/sensor_database.h>

/* internal public functions ================================================ */
// -----------------------------------------------------------------------------
int 
iWSdBaseOpen (int iArg1, void * pvArg2) {
  
  return iWSdBaseInit (0 /* File Size (bytes) */);
}

// -----------------------------------------------------------------------------
int 
iWSdBaseRead (void * pvAddress, uint8_t * pucData, size_t xDataLength) {

  return xDataLength;
}

// -----------------------------------------------------------------------------
int 
iWSdBaseWrite (void * pvAddress, const uint8_t * pucData, size_t xDataLength) {

  return xDataLength;
} 
/* ========================================================================== */
