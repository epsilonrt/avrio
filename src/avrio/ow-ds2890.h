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
#ifndef _AVRIO_OW_DS2890_H_
#define _AVRIO_OW_DS2890_H_

#include <avrio/defs.h>
#include <avrio/ow.h>

#if defined(__cplusplus)
  extern "C" {
#endif
/* ========================================================================== */
/**
 * @addtogroup ow_module
 * @{
 *
 *  @defgroup ow_ds2890 Circuit DS2890
 *  @{
 */
/* macros =================================================================== */
/* constants ================================================================ */
#define DS2890_FAMILY_ID                0x2c

/* internal public functions ================================================ */

/** @brief  Set the wiper position of a DS2890.
 *
 *  This function initializes the DS2890 by enabling the charge pump. It then
 *  changes the wiper position.
 *
 *  \param  device      The DS2890 device
 *  \param  position    The new wiper position.
 */
int iDs2890SetPosition (xOwDevice * device, uint8_t position);

/**
 *   @}
 * @}
 */

/* ========================================================================== */
#if defined(__cplusplus)
  }
#endif
#endif /* _AVRIO_OW_DS2890_H_ */
