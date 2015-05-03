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

/* *INDENT-OFF* */
__BEGIN_C_DECLS
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
__END_C_DECLS
/* *INDENT-ON* */
#endif  /* _AVRIO_THMI_H_ not defined */
