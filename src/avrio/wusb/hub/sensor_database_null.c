
/**
 * @file sensor_database_null.c
 * @brief Driver base de données capteurs WirelessUSB null
 * @author Copyright © 2011-2012 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
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
