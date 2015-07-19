/*
 * FreeModbus Libary: AVR Demo Application
 * Copyright (C) 2006 Christian Walter <wolti@sil.at>
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
 *
 * File: $Id$
 */

/* ----------------------- AVR includes ------------------------------------- */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avrio/delay.h>
/* ----------------------- Modbus includes ---------------------------------- */
#include <mb.h>
#include <mbport.h>

/* ----------------------- Defines ------------------------------------------ */
#define REG_INPUT_START 1
#define REG_INPUT_NREGS 4

/* ----------------------- Static variables --------------------------------- */
static USHORT usRegInputBuf[REG_INPUT_NREGS] = { 0, 0x1234, 0x5678, 0x9ABC };

/* ----------------------- Start implementation ----------------------------- */
int
main (void) {
  const UCHAR ucSlaveID[] = { 0xAA, 0xBB, 0xCC };
  eMBErrorCode eStatus;

  eStatus = eMBInit (MB_RTU, 32, 0, 19200, MB_PAR_EVEN);
  //eStatus = eMBInit (MB_ASCII, 32, 0, 19200, MB_PAR_EVEN);

  eStatus = eMBSetSlaveID (0x34, TRUE, ucSlaveID, 3);
  sei ();

  /* Enable the Modbus Protocol Stack. */
  eStatus = eMBEnable ();

  for (;;) {
    (void) eMBPoll ();

  }
}

eMBErrorCode
eMBRegInputCB (UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs) {
  eMBErrorCode eStatus = MB_ENOERR;
  int iRegIndex;

  if ( (usAddress >= REG_INPUT_START)
       && (usAddress + usNRegs <= REG_INPUT_START + REG_INPUT_NREGS)) {
    iRegIndex = (int) (usAddress - REG_INPUT_START);
    while (usNRegs > 0) {
      *pucRegBuffer++ = (unsigned char) (usRegInputBuf[iRegIndex] >> 8);
      *pucRegBuffer++ = (unsigned char) (usRegInputBuf[iRegIndex] & 0xFF);
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
  delay_ms (1);
  return eStatus;
}

eMBErrorCode
eMBRegHoldingCB (UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs,
                 eMBRegisterMode eMode) {
  return MB_ENOREG;
}

eMBErrorCode
eMBRegCoilsCB (UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNCoils,
               eMBRegisterMode eMode) {
  return MB_ENOREG;
}

eMBErrorCode
eMBRegDiscreteCB (UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNDiscrete) {
  return MB_ENOREG;
}
