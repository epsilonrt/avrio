/**
 * Copyright © 2011-2015 epsilonRT. All rights reserved.
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
 * @file serial.h
 * @brief CDC Virtual Serial
 *
 *
 * Revision History ------------------------------------------------------------
 *    20121226 - Initial version by epsilonRT
 */
#ifndef _MSL_SERIAL_H_
#define _MSL_SERIAL_H_
#include <avrio/defs.h>
#include <stdio.h>

/* public variables ========================================================= */
extern FILE xUsbSerial;

/* internal public functions ================================================ */
void vSerialInit (void);
uint16_t usSerialHit (void);
void vSerialUsbTask (void);
#endif  /* _MSL_SERIAL_H_ not defined */
/* ========================================================================== */
