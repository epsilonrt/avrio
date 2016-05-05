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
 * @file ds1621.c
 * @brief Thermostat I2c DS1621 de DALLAS/MAXIM - Implémentation
 *
 * Revision History ------------------------------------------------------------
 *    20120303 - Initial version by epsilonRT
 */
#include <avrio/ds1621.h>
#include <avrio/delay.h>
//#include <stdio.h>
//#include <avr/pgmspace.h>

/* private variables ======================================================== */
eTwiStatus eDs1621LastErrorValue;

/* private functions ======================================================== */

// -----------------------------------------------------------------------------
static xTwiLength
prvxRead (xTwiDeviceAddr xDeviceAddr,
          uint8_t ucCmd, uint8_t * pucData, xTwiLength xDataLen) {
  xTwiFrame xFrame;

  xFrame.xAddr = xDeviceAddr;
  xFrame.pxData = pucData;
  xFrame.xLen = xDataLen;
  eDs1621LastErrorValue = eTwiMem8Read (ucCmd, &xFrame);
  return xFrame.xLen;
}

// -----------------------------------------------------------------------------
static xTwiLength
prvxWrite (xTwiDeviceAddr xDeviceAddr,
           uint8_t ucCmd, uint8_t * pucData, xTwiLength xDataLen) {
  xTwiFrame xFrame;

  xFrame.xAddr = xDeviceAddr;
  xFrame.pxData = pucData;
  xFrame.xLen = xDataLen;
  eDs1621LastErrorValue = eTwiMem8Write (ucCmd, &xFrame);
  return xFrame.xLen;
}

/* internal public functions ================================================ */

/* ----------------------  Fonctions de bas niveau ------------------------- */

// -----------------------------------------------------------------------------
int16_t
iDs1621ReadRawTemp (xTwiDeviceAddr xDeviceAddr, uint8_t ucCmd) {
  int16_t iTemp = DS1621_TEMP_ERROR;
  uint8_t buf[2];

  if (prvxRead (xDeviceAddr, ucCmd, (uint8_t *) buf, 2) == 2) {
    
    iTemp = (((uint16_t) buf[0]) << 1) + (buf[1] >> 7);
    
    if (iTemp & 0x0100) {
      
      iTemp -= 512;
    }
  }
  return iTemp;
}

// -----------------------------------------------------------------------------
int16_t
iDs1621ReadTemp (xTwiDeviceAddr xDeviceAddr, uint8_t ucCmd) {
  int16_t iRaw = iDs1621ReadRawTemp (xDeviceAddr, ucCmd);

  if (iRaw != DS1621_TEMP_ERROR) {
    
    int16_t iTemp = iRaw * 5;
    return iTemp;
  }
  return iRaw;
}

// -----------------------------------------------------------------------------
void
vDs1621WriteTemp (xTwiDeviceAddr xDeviceAddr, uint8_t ucCmd, int16_t iTemp) {
  uint8_t pucTemp[2];

  iTemp = (iTemp / 5) << 7;
  pucTemp[0] = iTemp >> 8;
  pucTemp[1] = iTemp & 0x80;

  (void) prvxWrite (xDeviceAddr, ucCmd, pucTemp, 2);
}

// -----------------------------------------------------------------------------
uint8_t
ucDs1621ReadByte (xTwiDeviceAddr xDeviceAddr, uint8_t ucCmd) {
  uint8_t ucByte;

  (void) prvxRead (xDeviceAddr, ucCmd, &ucByte, 1);
  return ucByte;
}

// -----------------------------------------------------------------------------
void
vDs1621SendCmd (xTwiDeviceAddr xDeviceAddr, uint8_t ucCmd) {

  (void) prvxWrite (xDeviceAddr, ucCmd, 0, 0);
}

/* ----------------------  Fonctions de haut niveau ------------------------- */

// -----------------------------------------------------------------------------
void
vDs1621Init (xTwiDeviceAddr xDeviceAddr, uint8_t ucConfig) {

  ucConfig &= (POL | ONESHOT);
  (void) prvxWrite (xDeviceAddr, DS1621_RWCONF, &ucConfig, 1);
  delay_ms (10);
}

// -----------------------------------------------------------------------------
void
vDs1621ClrFlags (xTwiDeviceAddr xDeviceAddr, uint8_t ucFlags) {
  uint8_t ucStatus;

  ucStatus = ucDs1621Status (xDeviceAddr);
  ucStatus &= ~ (ucFlags & TAF) ;
  (void) prvxWrite (xDeviceAddr, DS1621_RWCONF, &ucStatus, 1);
}

// -----------------------------------------------------------------------------
void
vDs1621SetTl (xTwiDeviceAddr xDeviceAddr, int16_t iTl) {

  while (xDs1621MemIsBusy (xDeviceAddr))
    ;
  vDs1621WriteTemp (xDeviceAddr, DS1621_RWTL, iTl);
}

// -----------------------------------------------------------------------------
void
vDs1621SetTh (xTwiDeviceAddr xDeviceAddr, int16_t iTh) {

  while (xDs1621MemIsBusy (xDeviceAddr))
    ;
  vDs1621WriteTemp (xDeviceAddr, DS1621_RWTH, iTh);
}

// -----------------------------------------------------------------------------
double 
dDs1621HiResTemp (xTwiDeviceAddr xDeviceAddr) {
  double dTemp = NAN;
  int8_t iTemp[2], iCnt, iSlope;

  if (prvxRead (xDeviceAddr, DS1621_RDTEMP, (uint8_t *) iTemp, 2) == 2) {
    if (prvxRead (xDeviceAddr, DS1621_RDCNT, (uint8_t *) &iCnt, 1) == 1) {
      if (prvxRead (xDeviceAddr, DS1621_RDSLOP, (uint8_t *) &iSlope, 1) == 1) {
        double dFrac = 0;
        if (iSlope != 0) {
          
          dFrac = (iSlope - iCnt) / (double) iSlope - 0.25;
        }
        dTemp = (double) iTemp[0] + dFrac ;
      }
    }
  }
  return dTemp;
}

/* ========================================================================== */
