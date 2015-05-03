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
 * @file can.c
 * @brief Bus CAN
 *
 * @author Portions Copyright © 2005-2009 proconX Pty Ltd. All rights reserved.
 * @author Portions Copyright © 2004 by Ole Reinhardt <ole.reinhardt@kernelconcepts.de>, Kernelconcepts http://www.kernelconcepts.de

 */
#include "avrio-config.h"

#ifdef AVRIO_CAN_ENABLE
/* ========================================================================== */
#  include "avrio-board-can.h"
#  include "can_private.h"
#  include <avrio/can.h>
#  include <avrio/delay.h>
#  include <avr/interrupt.h>
#  include <util/atomic.h>
#  include <string.h>

/* macros =================================================================== */
/* constants ================================================================ */
#  define AVR_MAX_MOB   15// Number of messages objects
#  define DLC_MAX 8

/* structures =============================================================== */
typedef struct xCanFrameQueue {

  xCanFrame *pxFrame; // Pointeur sur les trames
  uint8_t ucSize; // Nombre de trames max.
  uint8_t ucLength; // Nombre de trames dans le buffer
  uint8_t ucIndex;  // Index de la prochaine trame
} xCanFrameQueue;

typedef struct xCanStatusInfo {

  uint16_t xRxFrames; // Number of packets received
  uint16_t xTxFrames; // Number of packets sent
  uint16_t xInterrupts; // Number of interrupts
  uint16_t xOverruns; // Number of packet overruns
  uint16_t xErrors; // Number of frame errors
  eCanError xLastError;
} xCanStatusInfo;

/* types ==================================================================== */

/* private variables ======================================================== */
static eCanSpecification eCanType;
static eCanSpeed eCanBaudrate;
static uint8_t ucCanAccMask[4];
static uint8_t ucCanAccCode[4];
static uint16_t usCanRxTimeout;
static xCanFrame xCanRxFrames[CAN_RXBUFSIZE];
static xCanFrameQueue xCanRxQueue = {
  .pxFrame = xCanRxFrames,
  .ucSize = CAN_RXBUFSIZE
};
static xCanStatusInfo xCanInfo;

/* private functions ======================================================== */
/* 
 * Returns the state of this CAN node.
 * Refer to state diagram in AT90CAN128 dataheet chapter 20.7 "Error
 * Management"
 *
 * @retval CAN_SUCCESS if CAN node is in ACTIVE state
 * @retval CAN_PASSIVE if CAN node is in PASSIVE state
 * @retval CAN_BUS_OFF if CAN node is in BUS OFF state
 */
static int8_t
iGetBusState (void) {

  if (CANGSTA & _BV (BOFF))
    return CAN_BUS_OFF;

  if (CANGSTA & _BV (ERRP))
    return CAN_PASSIVE;

  return CAN_SUCCESS;
}

/* 
 * Search for a free message object
 * @return Index of free message object or -1 if none is available
 */
static int
iGetFreeMob (void) {
  int8_t iMob;
  uint8_t ucCtrlReg;

  for (iMob = 0; iMob < AVR_MAX_MOB; iMob++) {

    CANPAGE = iMob << 4;
    ucCtrlReg = CANCDMOB & (_BV (CONMOB1) | _BV (CONMOB0));

    if (ucCtrlReg == 0)
      return iMob;

    if (ucCtrlReg == _BV (CONMOB0) && (bit_is_set (CANSTMOB, TXOK))) {

      // Free MOB
      CANCDMOB &= ~(_BV (CONMOB1) | _BV (CONMOB0));
      return iMob;
    }
  }
  return -1;
}

/* 
 * Set ID Tag registers
 */
static void
vSetIdTag (const xCanArbField * pxTag) {
  uint32_t ulId = pxTag->xId;
  uint8_t *ucId = (uint8_t *) & (ulId);

  if (pxTag->xIde) {

    CANCDMOB |= _BV (IDE);

    CANIDT1 = (ucId[3] << 3) + (ucId[2] >> 5);
    CANIDT2 = (ucId[2] << 3) + (ucId[1] >> 5);
    CANIDT3 = (ucId[1] << 3) + (ucId[0] >> 5);
    CANIDT4 = (ucId[0] << 3);
  } else {

    CANIDT1 = (ucId[1] << 5) + (ucId[0] >> 3);
    CANIDT2 = (ucId[0] << 5);
    CANIDT3 = 0;
    CANIDT4 = 0;
  }
  if (pxTag->xRtr) {

    CANIDT4 |= _BV (RTRTAG);
  }
}

/* 
 * Set ID Mask registers
 */
static void
vSetIdMask (const xCanArbField * pxMask) {
  uint32_t ulId = pxMask->xId;
  uint8_t *ucId = (uint8_t *) & (ulId);

  if (pxMask->xIde) {

    CANIDM1 = (ucId[3] << 3) + (ucId[2] >> 5);
    CANIDM2 = (ucId[2] << 3) + (ucId[1] >> 5);
    CANIDM3 = (ucId[1] << 3) + (ucId[0] >> 5);
    CANIDM4 = (ucId[0] << 3) | _BV (IDEMSK);
  } else {

    CANIDM1 = (ucId[1] << 5) + (ucId[0] >> 3);
    CANIDM2 = (ucId[0] << 5);
    CANIDM3 = 0;
    CANIDM4 = 0;
  }
  if (pxMask->xRtr) {

    CANIDM4 |= _BV (RTRMSK);
  }
}

/* 
 * Configure a single message object for reception.
 *
 * @param iMob  Message object index (0-14)
 * @param xTag  Acceptance code
 * @param xMask Acceptance mask
 * @return Result code. See @ref eCanError
 */
static int
iEnableMsgObj (uint8_t ucMob,
               const xCanArbField * pxTag, const xCanArbField * pxMask) {

  if (ucMob >= AVR_MAX_MOB) {

    return CAN_ILLEGAL_MOB;
  }
  // Select MOb
  CANPAGE = ucMob << 4;
  // Abort MOb
  CANCDMOB = 0;

  // Set identifier and mask
  vSetIdTag (pxTag);
  vSetIdMask (pxMask);

  // Enable reception
  CANCDMOB |= _BV (CONMOB1);

  return CAN_SUCCESS;
}

/* 
 * Configure message objects for receiption.
 *
 * @param noOfMsgObjs Number of message objects used for receiption (1-14).
 *                    Message objects not configured for receiption are used for
 *                    transmission.
 *                    The same acceptance filter and mask is applied to the
 *                    message objects.
 *                    If it is required to set-up more complex
 *                    filtering mechanisms use canEnableMsgObj() instead, which
 *                    allows setting filters on a per message object basis.
 * @param xTag  Acceptance code
 * @param xMask Acceptance mask
 * @return Result code. See @ref eCanError
 */
static int
iEnableRx (uint8_t noOfMsgObjs,
           const xCanArbField * pxTag, const xCanArbField * pxMask) {
  uint8_t ucIndex;
  int iResult;

  for (ucIndex = 0; ucIndex < noOfMsgObjs; ucIndex++) {

    iResult = iEnableMsgObj (ucIndex, pxTag, pxMask);
    if (iResult != CAN_SUCCESS)
      return iResult;
  }
  return CAN_SUCCESS;
}

/* 
 * Send a CAN message
 *
 * @param pxFrame Container for CAN message to be sent
 * @return Result code. See @ref eCanError
 */
static int
iSendMsg (const xCanFrame * pxFrame) {
  int8_t iMob;
  uint8_t ucIndex;

  iMob = iGetFreeMob ();
  if (iMob < 0) {

    return CAN_TXBUF_FULL;
  }
  // Select MOB
  CANPAGE = iMob << 4;

  // Abort MOB and set length fields
  CANCDMOB = pxFrame->xCtrl.xDlc;

  vSetIdTag (&(pxFrame->xArb));

  for (ucIndex = 0; ucIndex < DLC_MAX; ucIndex++) {

    CANMSG = pxFrame->ucData[ucIndex];
  }

  // Enable transmission
  CANCDMOB |= _BV (CONMOB0);
  return CAN_SUCCESS;
}

/* 
 * Sets the acceptance code
 *
 * @param ac 4 byte char array with the acceptance code
 */
static void
vSetAccCode (uint8_t * ac) {

  memcpy (ucCanAccCode, ac, sizeof (ucCanAccCode));;
  iEnableRx (CAN_RXMOB, 0, 0);  // ttt TODO: Implement it!
}

/* 
 * Sets the acceptance mask
 *
 * @param am 4 byte char array with the acceptance mask
 */
static void
vSetAccMask (uint8_t * am) {

  memcpy (ucCanAccMask, am, sizeof (ucCanAccMask));
  iEnableRx (CAN_RXMOB, 0, 0);  // ttt TODO: Implement it!
}

/* 
 * Empile la trame reçue
 */
static inline void
vPushRxFrame (void) {

  if (xCanRxQueue.ucLength < xCanRxQueue.ucSize) {
    uint8_t ucIndex;
    uint8_t *ucId;
    xCanFrame *pxRxFrame;

    pxRxFrame =
       &xCanRxQueue.pxFrame[(xCanRxQueue.ucIndex + xCanRxQueue.ucLength) %
                            xCanRxQueue.ucSize];
    ucId = (uint8_t *) & (pxRxFrame->xArb.xValue);

    if (bit_is_set (CANCDMOB, IDE)) {

      ucId[3] = CANIDT1 >> 3;
      ucId[2] = (CANIDT2 >> 3) | (CANIDT1 << 5);
      ucId[1] = (CANIDT3 >> 3) | (CANIDT2 << 5);
      ucId[0] = (CANIDT4 >> 3) | (CANIDT3 << 5);
    } else {

      ucId[3] = 0;
      ucId[2] = 0;
      ucId[1] = CANIDT1 >> 5;
      ucId[0] = (CANIDT1 << 3) | (CANIDT2 >> 5);
    }

    pxRxFrame->xArb.xIde = bit_is_set (CANCDMOB, IDE);
    pxRxFrame->xArb.xRtr = bit_is_set (CANIDT4, RTRTAG);
    pxRxFrame->xCtrl.xDlc =
       CANCDMOB & (_BV (DLC0) | _BV (DLC1) | _BV (DLC2) | _BV (DLC3));

    for (ucIndex = 0; ucIndex < 8; ucIndex++)
      pxRxFrame->ucData[ucIndex] = CANMSG;

    xCanRxQueue.ucLength++;
    xCanInfo.xRxFrames++;
  } else {

    xCanInfo.xOverruns++;
  }
}

// ------------------------------------------------------------------------------
ISR (CANIT_vect) {
  uint8_t ucCurrentCanPage;

  ucCurrentCanPage = CANPAGE; // Save the current CAN page
  xCanInfo.xInterrupts++;

  if ((CANHPMOB & 0xF0) != 0xF0) {

    CANPAGE = CANHPMOB; // Switch page

    if (bit_is_set (CANSTMOB, RXOK)) {
      // RX interrupt ?

      vPushRxFrame ();
    } else if (bit_is_set (CANSTMOB, TXOK)) {
      // TX interrupt ?

      xCanInfo.xTxFrames++;
    } else {
      // Error interrupts ?

      xCanInfo.xErrors++;
    }

    // Acknowledge all MOB interrupts and manage MObs
    CANSTMOB = CANSTMOB & ~(_BV (TXOK) | _BV (RXOK) | _BV (BERR) |
                            _BV (SERR) | _BV (CERR) | _BV (FERR) |
                            _BV (AERR));

    if (bit_is_set (CANCDMOB, CONMOB1)) {

      // Re-enable MOb for reception by re-writing CONMOB1 bit
      CANCDMOB |= _BV (CONMOB1);
    } else {

      // Re-claim MOb as send buffer
      CANCDMOB &= ~(_BV (CONMOB1) | _BV (CONMOB0));
    }
  } else {

    // Acknowledge general CAN interrupt
    CANGIT =
       _BV (BOFFIT) | _BV (BXOK) | _BV (SERG) | _BV (CERG) | _BV (FERG) |
       _BV (AERG);
    xCanInfo.xErrors++;
  }
  CANPAGE = ucCurrentCanPage; // Restore CAN page register
}

/* internal public functions ================================================ */

// ------------------------------------------------------------------------------
eCanError
eCanInit (eCanSpecification eType) {
  int8_t iMob, iIndex;

  eCanType = eType;
  eCanBaudrate = CAN_SPEED_125K;
  memset (&ucCanAccMask, 0xFF, sizeof (ucCanAccMask));
  memset (&ucCanAccCode, 0x00, sizeof (ucCanAccCode));
  usCanRxTimeout = WAIT_INFINITE;

  memset (&xCanInfo, 0, sizeof (xCanInfo));

  // Init receive buffer
  xCanRxQueue.ucIndex = 0;
  xCanRxQueue.ucLength = 0;

  // Disable AT90CAN128 CAN system
  CANGCON &= ~_BV (ENASTB);
  loop_until_bit_is_clear (CANGSTA, ENFG);

  // Clear all MObs
  for (iMob = 0; iMob < AVR_MAX_MOB; iMob++) {

    CANPAGE = iMob << 4;
    CANSTMOB = 0; // Clear status register
    CANCDMOB = 0; // Clear control register
    CANHPMOB = 0; // Clear HP flags
    // Clear identifier tag
    CANIDT1 = 0;
    CANIDT2 = 0;
    CANIDT3 = 0;
    CANIDT4 = 0;
    // Clear identifier mask
    CANIDM1 = 0;
    CANIDM2 = 0;
    CANIDM3 = 0;
    CANIDM4 = 0;
    for (iIndex = 0; iIndex < DLC_MAX; iIndex++) {

      CANMSG = 0;
    }
  }

  // Enable all interrupts
  CANIE1 = 0x7F;
  CANIE2 = 0xFF;
  CANGIE = 0xFE;  // All interrupts except OVRTIM

  // Enable receiving MOBs
  iEnableRx (CAN_RXMOB, 0, 0);

  // Enable AT90CAN128 CAN system
  CANGCON |= _BV (ENASTB);
  loop_until_bit_is_set (CANGSTA, ENFG);

  return CAN_SUCCESS;
}

// ------------------------------------------------------------------------------
eCanError
eCanSetSpeed (eCanSpeed eSpeed) {

  switch (eSpeed) {

    case CAN_SPEED_10K:
      CANBT1 = CAN_BT1_10K;
      CANBT2 = CAN_BT2_10K;
      CANBT3 = CAN_BT3_10K;
      break;
    case CAN_SPEED_20K:
      CANBT1 = CAN_BT1_20K;
      CANBT2 = CAN_BT2_20K;
      CANBT3 = CAN_BT3_20K;
      break;
    case CAN_SPEED_50K:
      CANBT1 = CAN_BT1_50K;
      CANBT2 = CAN_BT2_50K;
      CANBT3 = CAN_BT3_50K;
      break;
    case CAN_SPEED_100K:
      CANBT1 = CAN_BT1_100K;
      CANBT2 = CAN_BT2_100K;
      CANBT3 = CAN_BT3_100K;
      break;
    case CAN_SPEED_125K:
      CANBT1 = CAN_BT1_125K;
      CANBT2 = CAN_BT2_125K;
      CANBT3 = CAN_BT3_125K;
      break;
    case CAN_SPEED_250K:
      CANBT1 = CAN_BT1_250K;
      CANBT2 = CAN_BT2_250K;
      CANBT3 = CAN_BT3_250K;
      break;
    case CAN_SPEED_500K:
      CANBT1 = CAN_BT1_500K;
      CANBT2 = CAN_BT2_500K;
      CANBT3 = CAN_BT3_500K;
      break;
    case CAN_SPEED_800K:
      CANBT1 = CAN_BT1_800K;
      CANBT2 = CAN_BT2_800K;
      CANBT3 = CAN_BT3_800K;
      break;
    case CAN_SPEED_1M:
      CANBT1 = CAN_BT1_1M;
      CANBT2 = CAN_BT2_1M;
      CANBT3 = CAN_BT3_1M;
      break;
    case CAN_SPEED_CUSTOM:
      // Do nothing, user sets baud rate directly but don't report an error
      break;
    default:
      return CAN_INVALID_SPEED;
  }
  eCanBaudrate = eSpeed;
  return CAN_SUCCESS;
}

// ------------------------------------------------------------------------------
void
vCanSetFilter (uint8_t * pucAcceptanceCode, uint8_t * pucAcceptanceMask) {

  vSetAccCode (pucAcceptanceCode);
  vSetAccMask (pucAcceptanceMask);
}

// ------------------------------------------------------------------------------
eCanError
eCanSendFrame (const xCanFrame * pxFrame) {

  while (iSendMsg (pxFrame) == CAN_TXBUF_FULL) {

  }
  return CAN_SUCCESS;
}

// ------------------------------------------------------------------------------
bool
xCanTxFree (void) {

  return (iGetFreeMob () >= 0);
}

// ------------------------------------------------------------------------------
eCanError
eCanTrySendFrame (const xCanFrame * pxFrame) {

  if (!xCanTxFree ()) {

    return CAN_TXBUF_FULL;
  }
  (void) iSendMsg (pxFrame);
  return CAN_SUCCESS;
}

// ------------------------------------------------------------------------------
uint8_t
ucCanRxAvail (void) {

  return xCanRxQueue.ucLength;
}

// ------------------------------------------------------------------------------
eCanError
eCanReceiveFrame (xCanFrame * pxFrame) {
  uint16_t usRxTimeout = usCanRxTimeout;

  while (!ucCanRxAvail ()) {

    if (usRxTimeout--) {

      delay_ms (100);
      if (usRxTimeout == 0) {

        return CAN_TIMEOUT;
      }
    }
  }

  ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {
    // Get the first frame from buffer
    *pxFrame = xCanRxQueue.pxFrame[xCanRxQueue.ucIndex];
    // Move index down and decrement length
    xCanRxQueue.ucIndex++;
    if (xCanRxQueue.ucIndex >= xCanRxQueue.ucSize)
      xCanRxQueue.ucIndex %= xCanRxQueue.ucSize;
    xCanRxQueue.ucLength--;
  }

  return CAN_SUCCESS;
}

// ------------------------------------------------------------------------------
eCanError
eCanTryReceiveFrame (xCanFrame * pxFrame) {

  if (!ucCanRxAvail ()) {

    return CAN_RXBUF_EMPTY;
  }
  return eCanReceiveFrame (pxFrame);
}

// ------------------------------------------------------------------------------
void
vCanSetRxTimeout (uint16_t usTimeout) {

  usCanRxTimeout = usTimeout;
}

#endif /* AVRIO_CAN_ENABLE defined */
/* ========================================================================== */
