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
 * @file twi_addr.h
 * @brief Adresses des circuits TWI
 *
 *
 * Revision History ------------------------------------------------------------
 *    20120501 - Initial version by epsilonRT
 */
#ifndef _AVRIO_TWI_ADDR_H_
#define _AVRIO_TWI_ADDR_H_

#include <avrio/defs.h>

/* *INDENT-OFF* */
__BEGIN_C_DECLS
/* ========================================================================== */

/* PCF2119 - I2C */
#define PCF2119_BASE 0x74   /**< Adresse de base du PCF2119 */
#define PCF2119_SA0  0x02   /**< La broche SA0 permet de décaler l'adresse I2C */

/* ST7032 - I2C */
#define ST7032_BASE 0x7C   /**< Adresse de base du ST7032 */

/* THMI - I2C */
#define THMI_BASE 0x7E   /**< Adresse de base du THMI */

/* ========================================================================== */
__END_C_DECLS
/* *INDENT-ON* */
#endif  /* _AVRIO_TWI_ADDR_H_ not defined */
