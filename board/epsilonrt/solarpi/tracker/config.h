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
 * @file config.h
 * @brief Configuration
 * Copyright (c) 2013 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * Revision History ------------------------------------------------------------
 *    20131208 - Initial version by epsilonRT
 */
#ifndef _WHEREAVR_CONFIG_H_
#define _WHEREAVR_CONFIG_H_

#include <avrio/defs.h>
#include <avr/interrupt.h>

/* *INDENT-OFF* */
__BEGIN_C_DECLS
/* ========================================================================== */

/* constants ================================================================ */
#define TIMER0  0
#define TIMER1  1
#define TIMER2  2

/* configuration ============================================================ */
#define TONE_TIMER TIMER2
#define AX25_BAUDRATE 1200

//#define ALARM_DELAY_LOOP Serial_Processes
/* ========================================================================== */
__END_C_DECLS
/* *INDENT-ON* */
#endif  /* _WHEREAVR_CONFIG_H_ not defined */
