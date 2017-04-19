/**
 * Copyright © 2017 epsilonRT. All rights reserved.
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
 * @file 
 * @brief
 */
#ifndef _AVRIO_OW_DRIVER_COMMON_H_
#define _AVRIO_OW_DRIVER_COMMON_H_

#include <avrio/defs.h>

__BEGIN_C_DECLS
/* ========================================================================== */
/**
 * @addtogroup ow_module
 * @{
 *
 *  @defgroup ow_driver Driver 1wire
 *  @{
 */
/* constants ================================================================ */
#define     OW_ROM_READ    0x33
#define     OW_ROM_SKIP    0xcc
#define     OW_ROM_MATCH   0x55
#define     OW_ROM_SEARCH  0xf0

/* internal public functions ================================================ */
/**
 * @brief Initialisation du module 1wire
 */
void vOwInit(void);

/**
 * @brief Génère un reset sur le bus et vérifie la présence de circuits
 * @return true si des circuits sont présents
 */
bool bOwReset(void);

/* @brief  Sends the SEARCH ROM command and returns 1 id found on the
 *          1-Wire(R) bus.
 *
 *  \param  bitPattern      A pointer to an 8 byte char array where the
 *                          discovered identifier will be placed. When
 *                          searching for several slaves, a copy of the
 *                          last found identifier should be supplied in
 *                          the array, or the search will fail.
 *
 *  \param  lastDeviation   The bit position where the algorithm made a
 *                          choice the last time it was run. This argument
 *                          should be 0 when a search is initiated. Supplying
 *                          the return argument of this function when calling
 *                          repeatedly will go through the complete slave
 *                          search.
 *
 *  \return The last bit position where there was a discrepancy between slave
 * addresses the last time this function was run. Returns -1 (0xFF)
 * if an error was detected (e.g. a device was connected to the bus during the
 * search), or 0 when there are no more devices to be
 * discovered.
 */
uint8_t ucOwSearchRom (uint8_t * bitPattern, uint8_t lastDeviation);

/**
 *   @}
 * @}
 */

/* ========================================================================== */
__END_C_DECLS
#endif /* _AVRIO_OW_DRIVER_COMMON_H_ */
