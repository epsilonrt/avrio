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
 * @file cypress.h
 * @brief Private definitions
 *
 * Revision History ------------------------------------------------------------
 *    20120227 - Initial version by epsilonRT
 * --------------------------------------------------------------------------
 *               
 *  Copyright 2004, Cypress Semiconductor Corporation.
 * 
 *  This software is owned by Cypress Semiconductor Corporation (Cypress)
 *  and is protected by and subject to worldwide patent protection (United
 *  States and foreign), United States copyright laws and international
 *  treaty provisions. Cypress hereby grants to licensee a personal,
 *  non-exclusive, non-transferable license to copy, use, modify, create
 *  derivative works of, and compile the Cypress Source Code and derivative
 *  works for the sole purpose of creating custom software in support of
 *  licensee product to be used only in conjunction with a Cypress integrated
 *  circuit as specified in the applicable agreement. Any reproduction,
 *  modification, translation, compilation, or representation of this
 *  software except as specified above is prohibited without the express
 *  written permission of Cypress.
 * 
 *  Disclaimer: CYPRESS MAKES NO WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 *  WITH REGARD TO THIS MATERIAL, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *  Cypress reserves the right to make changes without further notice to the
 *  materials described herein. Cypress does not assume any liability arising
 *  out of the application or use of any product or circuit described herein.
 *  Cypress does not authorize its products for use as critical components in
 *  life-support systems where a malfunction or failure may reasonably be
 *  expected to result in significant injury to the user. The inclusion of
 *  Cypress' product in a life-support systems application implies that the
 *  manufacturer assumes all risk of such use and in doing so indemnifies
 *  Cypress against all charges.
 * 
 *  Use may be limited by and subject to the applicable Cypress software
 *  license agreement.
 * 
 * --------------------------------------------------------------------------
 */
#ifndef _IDWARF_CYPRESS_H_
#define _IDWARF_CYPRESS_H_

#include "hub.h"

/* macros =================================================================== */
 #define offsetof(s,m)   (U16)&(((s *)0)->m)

/* constants ================================================================ */
// These parameters are burned into FLASH during prgramming causing the sensor
// to be factory seeded bound.  Note that the CRC is based on the preceeding
// two values and must be recomputed if either of these two values are changed.
#define SEEDED_BIND_CHI     1
#define SEEDED_BIND_PNI     1
#define SEEDED_BIND_CRC     0x02
#ifndef FALSE
#define FALSE 0
#endif

//--------------------------------------
typedef enum
{
#if defined(HUB)
  POR_SW1_DETECT      = 0x01,
  SW1_DOWN            = 0x02,
  HEARTBEAT_CHANGED   = 0x04,
  SYSTEM_POR          = 0x10
#else // Sensor
  POR_SW1_DETECT    = 0x01,
  SYSTEM_TEST_1    = 0x02,
  SYSTEM_TEST_2    = 0x04,
  SWITCH_SLEEP_EVENT  = 0x08,
  SYSTEM_POR          = 0x10,
  FLASH_VALID         = 0x20,
  SYSTEM_TEST_3    = 0x80
#endif
} HW_STATUS;

/* structures =============================================================== */
//--------------------------------------
typedef struct _NETWORK_CONFIG_PARAMETERS
{
  U8  u8ChIndex;  // Keep the first element a byte
  U8  u8PnIndex;
  MID sHubMid;
  U16 u16DeviceId;
  U16 u16DeviceInterval;
} NETWORK_CONFIG_PARAMETERS;

//--------------------------------------
typedef struct _RADIO_PARAMETERS
{
    U8  u8Xtal;

} RADIO_PARAMETERS;

//--------------------------------------
typedef struct _FLASH_STRUCTURE
{
    NETWORK_CONFIG_PARAMETERS   sNetConfig;
    MID                         sSensorMid;
    U8                          u8FlashCrc;
    RADIO_PARAMETERS            sRadio;

} FLASH_STRUCTURE;


/* types ==================================================================== */
/* private variables ======================================================== */
/* private functions ======================================================== */
/* public variables ========================================================= */
extern const FLASH_STRUCTURE  gsFlash;
extern HW_STATUS gu8HardwareStatus;

/* internal public functions ================================================ */
void SpiRadioOn (void);
void RadioInit (void);
U16  ProtocolComputeCrc(U8 * Buffer, U8 Lentgh, U8 Seed);

/* ========================================================================== */
#endif  /* _IDWARF_CYPRESS_H_ not defined */
