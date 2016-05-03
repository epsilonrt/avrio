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
 * @file sensor_database_i2cmem.c
 * @brief Driver base de données capteurs WirelessUSB mémoire I2C 16 bits
 *
 * Revision History ------------------------------------------------------------
 *    20111205 - Initial version by epsilonRT
 */
#include <avrio/twi.h>
#include <avrio/wusb/hub/sensor_database.h>

/*
  Performances:
  -------------
   - Temps d'ajout d'un capteur: 692 ṁs 
   - Conditions: FM24CL64 sur bus I2C à 400 Khz / ATmega128 / FCPU = 14.7456 MHZ
 */

/* private variables ======================================================== */
static xTwiFrame * pxFrame;

/* internal public functions ================================================ */
// -----------------------------------------------------------------------------
int 
iWSdBaseOpen (int iSize, void * pvFrame) {
  
  pxFrame = pvFrame;
  return iWSdBaseInit (iSize);
}

// -----------------------------------------------------------------------------
int 
iWSdBaseRead (void * pvAddress, uint8_t * pucData, size_t xDataLength) {
  eTwiStatus eStatus;
  
  pxFrame->pxData = pucData;
  pxFrame->xLen = xDataLength;
  eStatus = eTwiMem16Read  ((uint16_t) pvAddress, pxFrame);
  if (eStatus < TWI_SUCCESS) {
  
    return (int) eStatus;
  }
  return (uint16_t) pxFrame->xLen;
}

// -----------------------------------------------------------------------------
int 
iWSdBaseWrite (void * pvAddress, const uint8_t * pucData, size_t xDataLength) {
  eTwiStatus eStatus;
  
  pxFrame->pxData = (uint8_t *) pucData;
  pxFrame->xLen = xDataLength;
  eStatus = eTwiMem16Write  ((uint16_t) pvAddress, pxFrame);
  if (eStatus < TWI_SUCCESS) {
  
    return (int) eStatus;
  }
  return (uint16_t) pxFrame->xLen;
} 
/* ========================================================================== */
