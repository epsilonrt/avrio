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
#ifndef _AVRIO_OW_DS1820_H_
#define _AVRIO_OW_DS1820_H_

#include <avrio/defs.h>
#include <avrio/ow.h>

__BEGIN_C_DECLS
/* ========================================================================== */
/**
 * @addtogroup ow_module
 * @{
 *
 *  @defgroup ow_ds1820 Circuit DS1820
 *  @{
 */
/* constants ================================================================ */
#define DS1820_FAMILY_ID  0x10
#define DS1820_ERROR      -1000   ///< Return code. Outside temperature range.

/* public variables ========================================================= */
/* internal public functions ================================================ */

/** @brief  Read the temperature from a DS1820 temperature sensor.
 *
 *  This function will start a conversion and read back the temperature
 *  from a DS1820 temperature sensor.
 *
 *  \param  id  The 64 bit identifier of the DS1820.
 *
 *  \return The 16 bit signed temperature read from the DS1820.
 */
int iDs1820Read (xOwDevice * device);

/**
 *   @}
 * @}
 */

/* ========================================================================== */
__END_C_DECLS
#endif /* _AVRIO_OW_DS1820_H_ */
