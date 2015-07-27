/**
 * Copyright © 2015 Pascal JEAN aka epsilonRT <pascal.jean--AT--btssn.net>
 * All rights reserved.
 * This software is governed by the CeCILL license <http://www.cecill.info>
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <mb.h>
#include <mbutils.h>
#include <mbport.h>
#include <avrio/net.h>

/* constants ================================================================ */
#define MB_SLAVEADDR  32
#define MB_BAUDRATE   38400
#define MB_PARITY     MB_PAR_EVEN

#define REG_INPUT_START 1
#define REG_INPUT_NREGS 5

#define REG_HOLD_START  1
#define REG_HOLD_NREGS  20

#define BIT_COIL_START  1
#define BIT_COIL_NBITS  16

#define BIT_DISC_START  1
#define BIT_DISC_NBITS  16

/* private variables ======================================================== */
static USHORT   usRegInputBuf[REG_INPUT_NREGS] = {
  0, 0xAABB, 0xCCDD, 0xEEFF, 0x1234
};
static USHORT   usRegHoldBuf[REG_HOLD_NREGS];
static UCHAR    ucBitCoilBuf[BIT_COIL_NBITS / 8];
static UCHAR    ucBitDiscBuf[BIT_DISC_NBITS / 8] = { 0xF5, 0x41 };

/* main ===================================================================== */
int
main (void) {
  const UCHAR ucSlaveID[] = { 0xAA, 0xBB, 0xCC };
  eMBErrorCode eStatus;

  eStatus = eMBInit (MB_RTU, MB_SLAVEADDR, 0, MB_BAUDRATE, MB_PARITY);
  eStatus = eMBSetSlaveID (0x34, TRUE, ucSlaveID, 3);
  sei ();

  /* Enable the Modbus Protocol Stack. */
  eStatus = eMBEnable ();

  for (;;) {
    (void) eMBPoll ();
  }
}

/* private functions ======================================================== */


/* internal public functions ================================================ */

//------------------------------------------------------------------------------
eMBErrorCode
eMBRegInputCB (UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs) {
  eMBErrorCode eStatus = MB_ENOERR;
  int iRegIndex;

  if ( (usAddress >= REG_INPUT_START)
       && (usAddress + usNRegs <= REG_INPUT_START + REG_INPUT_NREGS)) {

    iRegIndex = (int) (usAddress - REG_INPUT_START);
    while (usNRegs > 0) {
      pucRegBuffer = pushs (pucRegBuffer, usRegInputBuf[iRegIndex]);
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
        // lecture d'un mot par le maître, on empile ce mot dans le buffer
        pucRegBuffer = pushs (pucRegBuffer, usRegHoldBuf[iRegIndex]);
      }
      else {
        // écriture d'un mot par le maître, on dépile ce mot du buffer
        pucRegBuffer = pops (pucRegBuffer, PUINT16(&usRegHoldBuf[iRegIndex]));
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
      xMBUtilSetBits (pucByteBuffer++, 0, ucBitsCount, ucValues);
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
        xMBUtilSetBits (pucByteBuffer++, 0, ucBitsCount, ucValues);
      }
      else {
        // écriture bits
        ucValues = xMBUtilGetBits (pucByteBuffer++, 0, ucBitsCount);
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
/* ========================================================================== */
