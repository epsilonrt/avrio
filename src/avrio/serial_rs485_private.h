/**
 * @file serial_rs485_private.h
 * @brief Entête privé liaison série rs485
 *
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
 */
#ifndef _AVRIO_SERIAL_RS485_PRIVATE_H_
#define _AVRIO_SERIAL_RS485_PRIVATE_H_
/* ========================================================================== */
#include "avrio-config.h"

#ifdef AVRIO_SERIAL_ENABLE
/* ========================================================================== */
#include <avrio/serial_irq_private.h>

#if AVRIO_SERIAL_FLAVOUR == SERIAL_FLAVOUR_RS485
// -----------------------------------------------------------------------------
#ifndef SERIAL_TXEN_ENABLE
#error SERIAL_TXEN_ENABLE must be defined for RS485 serial link !
#endif
// -----------------------------------------------------------------------------
#endif /* AVRIO_SERIAL_FLAVOUR == SERIAL_FLAVOUR_RS485 */

/* ========================================================================== */
#endif /* AVRIO_SERIAL_ENABLE defined */
#endif /* _AVRIO_SERIAL_RS485_PRIVATE_H_ */
