/*
 * FreeModbus Libary: AVR Demo Application
 * Copyright (C) 2006 Christian Walter <wolti@sil.at>
 * Copyright (C) 2015 Pascal Jean, epsilonRT. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

/* ----------------------- AVR includes ------------------------------------- */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avrio/delay.h>
/* ----------------------- Modbus includes ---------------------------------- */
#include <mb.h>
#include <mbutils.h>
#include <mbport.h>

/* ----------------------- Defines ------------------------------------------ */
#define REG_INPUT_START 1
#define REG_INPUT_NREGS 5

#define REG_HOLD_START  1
#define REG_HOLD_NREGS  20

#define BIT_COIL_START  1
#define BIT_COIL_NBITS  16

#define BIT_DISC_START  1
#define BIT_DISC_NBITS  16

/* ----------------------- Static variables --------------------------------- */
static USHORT   usRegInputBuf[REG_INPUT_NREGS] = { 0, 0xAABB, 0xCCDD, 0xEEFF, 0x1234 };
static USHORT   usRegHoldBuf[REG_HOLD_NREGS];
static UCHAR    ucBitCoilBuf[BIT_COIL_NBITS / 8];
static UCHAR    ucBitDiscBuf[BIT_DISC_NBITS / 8] = { 0x55, 0xAA };

/* ----------------------- Start implementation ----------------------------- */
int
main (void) {
  const UCHAR ucSlaveID[] = { 0xAA, 0xBB, 0xCC };
  eMBErrorCode eStatus;

  eStatus = eMBInit (MB_RTU, 32, 0, 19200, MB_PAR_EVEN);
  eStatus = eMBSetSlaveID (0x34, TRUE, ucSlaveID, 3);
  sei ();

  /* Enable the Modbus Protocol Stack. */
  eStatus = eMBEnable ();

  for (;;) {
    (void) eMBPoll ();
  }
}

//------------------------------------------------------------------------------
// Empile un mot dans le buffer
UCHAR * pucPushWord (UCHAR * pucRegBuffer, USHORT usWord) {

  *pucRegBuffer++ = (UCHAR) (usWord >> 8);
  *pucRegBuffer++ = (UCHAR) (usWord & 0xFF);
  return pucRegBuffer;
}

//------------------------------------------------------------------------------
// Dépile un mot du buffer
UCHAR * pucPullWord (UCHAR * pucRegBuffer, USHORT * usWord) {

  *usWord  = (*pucRegBuffer++) << 8;
  *usWord |=  *pucRegBuffer++;
  return pucRegBuffer;
}

//------------------------------------------------------------------------------
eMBErrorCode
eMBRegInputCB (UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs) {
  eMBErrorCode eStatus = MB_ENOERR;
  int iRegIndex;

  if ( (usAddress >= REG_INPUT_START)
       && (usAddress + usNRegs <= REG_INPUT_START + REG_INPUT_NREGS)) {
    iRegIndex = (int) (usAddress - REG_INPUT_START);
    while (usNRegs > 0) {
      pucRegBuffer = pucPushWord (pucRegBuffer, usRegInputBuf[iRegIndex]);
      iRegIndex++;
      usNRegs--;
    }
    if (usAddress == REG_INPUT_START) {
      usRegInputBuf[0]++;
    }
  }
  else {
    eStatus = MB_ENOREG;
  }
  return eStatus;
}

//------------------------------------------------------------------------------
eMBErrorCode
eMBRegHoldingCB (UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs,
                 eMBRegisterMode eMode) {

  eMBErrorCode    eStatus = MB_ENOERR;
  int             iRegIndex;

  if ( (usAddress >= REG_HOLD_START)
       && (usAddress + usNRegs <= REG_HOLD_START + REG_HOLD_NREGS)) {

    usRegInputBuf[0]++;
    iRegIndex = (int) (usAddress - REG_HOLD_START);
    while (usNRegs > 0) {

      if (eMode == MB_REG_READ) {
        // lecture mot
        pucRegBuffer = pucPushWord (pucRegBuffer, usRegHoldBuf[iRegIndex]);
      }
      else {
        // écriture mot
        pucRegBuffer = pucPullWord (pucRegBuffer, &usRegHoldBuf[iRegIndex]);
      }
      iRegIndex++;
      usNRegs--;
    }
  }
  else {

    eStatus = MB_ENOREG;
  }

  return eStatus;
}

//------------------------------------------------------------------------------
eMBErrorCode
eMBRegDiscreteCB (UCHAR * pucByteBuffer, USHORT usBitOffset, USHORT usNBits) {
  eMBErrorCode eStatus = MB_ENOERR;
  UCHAR ucValues, ucBitsCount;

  if ( (usBitOffset >= BIT_DISC_START)
       && (usBitOffset + usNBits <= BIT_DISC_START + BIT_DISC_NBITS)) {

    usBitOffset--; // ModBus -> PDU
    while (usNBits) {
      // Recopie des bits par paquet de 8 bits max.
      ucBitsCount = MIN (8, usNBits);
      ucValues = xMBUtilGetBits (ucBitDiscBuf, usBitOffset, ucBitsCount);
      xMBUtilSetBits (pucByteBuffer, usBitOffset, ucBitsCount, ucValues);
      ucValues ^= 0xFF; // Bascule l'état
      xMBUtilSetBits (ucBitDiscBuf, usBitOffset, ucBitsCount, ucValues);
      usNBits -= ucBitsCount;
      usBitOffset += ucBitsCount;
    }
  }
  else {
    eStatus = MB_ENOREG;
  }
  return eStatus;
}

//------------------------------------------------------------------------------
eMBErrorCode
eMBRegCoilsCB (UCHAR * pucByteBuffer, USHORT usBitOffset, USHORT usNBits,
               eMBRegisterMode eMode) {
  eMBErrorCode eStatus = MB_ENOERR;
  static UCHAR ucValues, ucBitsCount;

  if ( (usBitOffset >= BIT_COIL_START)
       && (usBitOffset + usNBits <= BIT_COIL_START + BIT_COIL_NBITS)) {

    usBitOffset--; // ModBus -> PDU
    while (usNBits) {
      // Recopie des bits par paquet de 8 bits max.
      ucBitsCount = MIN (8, usNBits);
      if (eMode == MB_REG_READ) {
        // lecture bits
        ucValues = xMBUtilGetBits (ucBitCoilBuf, usBitOffset, ucBitsCount);
        xMBUtilSetBits (pucByteBuffer, usBitOffset, ucBitsCount, ucValues);
      }
      else {
        // écriture bits
        ucValues = xMBUtilGetBits (pucByteBuffer, usBitOffset, ucBitsCount);
        xMBUtilSetBits (ucBitCoilBuf, usBitOffset, ucBitsCount, ucValues);
      }
      usNBits -= ucBitsCount;
      usBitOffset += ucBitsCount;
    }
  }
  else {
    eStatus = MB_ENOREG;
  }
  return eStatus;
}
