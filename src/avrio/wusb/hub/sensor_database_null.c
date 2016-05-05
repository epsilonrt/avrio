
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
