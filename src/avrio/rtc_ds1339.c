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
 * @file rtc_ds1339.c
 * @brief RTC DS1339
 *
 * Revision History ------------------------------------------------------------
 *    20120402 - Initial version by epsilonRT
 */
#include "avrio-config.h"

#ifdef AVRIO_RTC_ENABLE
/* ========================================================================== */
#include "rtc.h"
#include "rtc_ds1339.h"
#include "twi.h"
#include "avrio-board-rtc.h"

/* constants ================================================================ */
// Valeur du registre de charge batterie à l'initialisation
#define REG_TCHARG_VALUE 0
// Valeur du registre de contrôle à l'initialisation
#define REG_CONTROL_VALUE (_BV(bRS2)|_BV(bRS1))
// Valeur ajoutée ou retranchée à une année sur 2 chiffres
#define YEAR_OFFSET 2000

/* private variables ======================================================== */
static xTwiFrame xRtcFrame;
static xMutex xMutexAlm[2]; // 0 -> LOCKED

/* private functions ======================================================== */
// -----------------------------------------------------------------------------
static uint8_t 
ucBcd2Bin (uint8_t ucBcd) {

  return (ucBcd & 0x0F) + (ucBcd >> 4) * 10;
}

// -----------------------------------------------------------------------------
static uint8_t
ucBin2Bcd (uint8_t ucBin) {

  return ucBin / 10 * 16 + ucBin % 10;
}

/* internal public functions ================================================ */
#include <avr/interrupt.h>

// -----------------------------------------------------------------------------
#define REG_CONTROL (DS1339_CONTROL - DS1339_CONTROL)
#define REG_STATUS  (DS1339_STATUS  - DS1339_CONTROL)
#define REG_TCHARG  (DS1339_TCHARG  - DS1339_CONTROL)
int
iRtcInit (int xDeviceAddr) {
  int iError;
  uint8_t ucReg[3];

  vRtcHardwareInit();
  
  // Lecture des registres de contrôle, d'état et de charge batterie
  xRtcFrame.xAddr = (xTwiDeviceAddr) xDeviceAddr,
  xRtcFrame.pxData = ucReg,
  xRtcFrame.xLen = 3;
  iError = (int) eTwiMem8Read (DS1339_CONTROL, &xRtcFrame);
  if (iError != 0) 
    return iError;

  // Registre de charge batterie
  ucReg[REG_TCHARG] = REG_TCHARG_VALUE;
  
  // Registre d'état
  if (bit_is_set(ucReg[REG_STATUS], bOSF)) {
  
    xRtcTime xInitTime = {
      // Dim 1 Avril 2012 - 00:00:00
      .ucSecond = 0,
      .ucMinute = 0,
      .ucHour = 0,
      .ucWeekDay = SUNDAY,
      .ucDate = 1,
      .ucMonth = 4,
      .usYear = 2012
    };
    iError = iRtcSetTime (&xInitTime);
    if (iError != 0) 
      return iError;
  }
  ucReg[REG_STATUS] = 0; // Clear flags

  // Registre de contrôle
  ucReg[REG_CONTROL] = REG_CONTROL_VALUE;
  
  iError = (int) eTwiMem8Write (DS1339_CONTROL, &xRtcFrame);
  if (iError != 0) 
    return iError;
  
  return 0;
}


// -----------------------------------------------------------------------------
int
iRtcSetTime (const xRtcTime * pxTime) {
  uint8_t ucBuffer[7];

  ucBuffer[DS1339_CK_SEC]  = ucBin2Bcd (pxTime->ucSecond);
  ucBuffer[DS1339_CK_MIN]  = ucBin2Bcd (pxTime->ucMinute);
  ucBuffer[DS1339_CK_HOUR] = ucBin2Bcd (pxTime->ucHour);
  ucBuffer[DS1339_CK_WDAY] = pxTime->ucWeekDay & 7;
  ucBuffer[DS1339_CK_DATE]  = ucBin2Bcd (pxTime->ucDate);
  ucBuffer[DS1339_CK_MON]  = ucBin2Bcd (pxTime->ucMonth);
  ucBuffer[DS1339_CK_YEAR] = ucBin2Bcd (pxTime->usYear - YEAR_OFFSET);
  
  xRtcFrame.pxData = ucBuffer,
  xRtcFrame.xLen = 7;
  //ATOMIC
  return (int) eTwiMem8Write (DS1339_CK_SEC, &xRtcFrame);
}

// -----------------------------------------------------------------------------
int
iRtcGetTime (xRtcTime * pxTime) {
  uint8_t ucBuffer[7];
  int iError;
  
  xRtcFrame.pxData = ucBuffer,
  xRtcFrame.xLen = 7;
  //ATOMIC
  iError = (int) eTwiMem8Read (DS1339_CK_SEC, &xRtcFrame);
  if (iError != 0) 
    return iError;

  pxTime->ucSecond = ucBcd2Bin (ucBuffer[DS1339_CK_SEC] & ~0x80);
  pxTime->ucMinute = ucBcd2Bin (ucBuffer[DS1339_CK_MIN] & ~0x80);
  pxTime->ucHour = ucBcd2Bin (ucBuffer[DS1339_CK_HOUR] & ~0xC0);
  pxTime->ucWeekDay = (ucBuffer[DS1339_CK_WDAY] & 0x07);
  pxTime->ucDate = ucBcd2Bin (ucBuffer[DS1339_CK_DATE] & ~0xC0);
  pxTime->ucMonth = ucBcd2Bin (ucBuffer[DS1339_CK_MON] & ~0xE0);
  pxTime->usYear = YEAR_OFFSET + ucBcd2Bin (ucBuffer[DS1339_CK_YEAR]);

  return 0;
}

// -----------------------------------------------------------------------------
int
iRtcSetAlm (int8_t iIndex, const xRtcTime * pxTime) {

  return -1;
}

// -----------------------------------------------------------------------------
int
iRtcGetAlm (int8_t iIndex, xRtcTime * pxTime) {

  return -1;
}

// -----------------------------------------------------------------------------
xMutex *
pxRtcAlmMutex (int8_t iIndex) {

  return 0;
}

#ifdef RTC_IRQ_vect
// -----------------------------------------------------------------------------
ISR (RTC_IRQ_vect) {
  uint8_t ucStatus;
  xTwiFrame xIrqFrame = { .pxData = &ucStatus, .xLen = 1 };
  
  xIrqFrame.xAddr = xRtcFrame.xAddr;
  (void) eTwiMem8Read (DS1339_STATUS, &xIrqFrame);
  
}
#endif

#endif /* AVRIO_RTC_ENABLE defined */
/* ========================================================================== */
