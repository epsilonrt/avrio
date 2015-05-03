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
 * @file spi.h
 * @brief SPI Driver
 *
 * Revision History ------------------------------------------------------------
 *    20120227 - Initial version by epsilonRT
 */
#ifndef _IDWARF_SPI_H_
#define _IDWARF_SPI_H_

#include "itypes.h"

/* internal public functions ================================================ */
U8 SPI_MasterReceive(void);
void SPI_MasterTransmit(U8);

/* ========================================================================== */
#endif  /* _IDWARF_SPI_H_ not defined */
