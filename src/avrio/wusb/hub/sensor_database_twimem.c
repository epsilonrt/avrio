/**
 * @file sensor_database_i2cmem.c
 * @brief Driver base de données capteurs WirelessUSB mémoire I2C 16 bits
 * @author Copyright © 2011-2012 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
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
