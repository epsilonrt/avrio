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
 * @file thmi.h
 * @brief tiny HMI
 *
 * Revision History ------------------------------------------------------------
 *    20120519 - Initial version by epsilonRT
 */
#ifndef _AVRIO_THMI_H_
#define _AVRIO_THMI_H_

#include <avrio/defs.h>
#include <avrio/keyb.h>
#include <avrio/lcd.h>

#if defined(__cplusplus)
  extern "C" {
#endif
/* ========================================================================== */
/**
 * @addtogroup dev_group
 * @{
 *
 *  @defgroup thmi_module tiny HMI
 *  Ce module permet de gérer une interface homme-machine avec une carte tiny
 *  HMI.
 *  @{
 */
  
/* internal public functions ================================================ */

/**
 * @brief Lecture de la version du firmware
 * @return Version du firmware 0xMMNN, MM version majeure, NN mineure
 */
uint16_t usThmiVersion (void);

/**
 *   @}
 * @}
 */

/* ========================================================================== */
#if defined(__cplusplus)
  }
#endif
#endif  /* _AVRIO_THMI_H_ not defined */
