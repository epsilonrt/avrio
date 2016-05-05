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
