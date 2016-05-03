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
 * @file rtc_ds1339.h
 * @brief RTC DS1339
 *
 * Revision History ------------------------------------------------------------
 *    20120402 - Initial version by epsilonRT
 */
#ifndef _AVRIO_RTC_DS1339_H_
#define _AVRIO_RTC_DS1339_H_

#include <avrio/twi.h>

/* constants ================================================================ */
#define DS1339_BASE  0xD0 /**< Adresse I2c de base des circuits DS1339 */

// -------------------------------------
#define DS1339_CK_SEC  0x00
#define DS1339_CK_MIN  0x01
#define DS1339_CK_HOUR 0x02
#define DS1339_CK_WDAY 0x03
#define DS1339_CK_DATE 0x04
#define DS1339_CK_MON  0x05
#define DS1339_CK_YEAR 0x06

// -------------------------------------
#define DS1339_A1_SEC  0x07
#define DS1339_A1_MIN  0x08
#define DS1339_A1_HOUR 0x09
#define DS1339_A1_DAY  0x0A

// -------------------------------------
#define DS1339_A2_MIN  0x0B
#define DS1339_A2_HOUR 0x0C
#define DS1339_A2_DAY  0x0D

// -------------------------------------
// Time/Alarm registers bits
#define b12n24  6
#define bPMnAM  5
#define bDYnDT  6

// -------------------------------------
#define DS1339_CONTROL   0x0E
// Control register bits
#define bnEOSC  7
#define bBBSQI  5
#define bRS2    4
#define bRS1    3
#define bINTCN  2
#define bA2IE   1
#define bA1IE   0

// -------------------------------------
#define DS1339_STATUS    0x0F
// Status register bits
#define bOSF    7
#define bA2F    1
#define bA1F    0

// -------------------------------------
#define DS1339_TCHARG    0x10
// Trickle charger register bits
#define bTCS3   7
#define bTCS2   6
#define bTCS1   5
#define bTCS0   4
#define bDS1    3
#define bDS0    2
#define bROUT1  1
#define bROUT0  0


/* ========================================================================== */
#endif  /* _AVRIO_RTC_DS1339_H_ not defined */
