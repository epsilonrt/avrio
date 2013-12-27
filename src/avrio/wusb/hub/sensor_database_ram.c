/**
 * @file sensor_database_ram.c
 * @brief Driver base de données capteurs WirelessUSB en RAM
 * @author Copyright © 2011-2012 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * @version $Id$
 * Revision History ---
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
