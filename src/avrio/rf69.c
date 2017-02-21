/**
 * @file
 * @brief
 *
 * Copyright © 2017 epsilonRT, All rights reserved.
 * This software is governed by the CeCILL license <http://www.cecill.info>
 */
#include <string.h>
#include "rf69_private.h"

/* private variables ======================================================== */
static const struct xRf69Config  xInitConfigPgm[] PROGMEM = {
  /* 0x01 */
  { REG_OPMODE, RF_OPMODE_SEQUENCER_ON | RF_OPMODE_LISTEN_OFF | RF_OPMODE_STANDBY },
  /* 0x02 */
  { REG_DATAMODUL, RF_DATAMODUL_DATAMODE_PACKET | RF_DATAMODUL_MODULATIONTYPE_FSK | RF_DATAMODUL_MODULATIONSHAPING_00 }, // no shaping
  /* 0x03 */
  { REG_BITRATEMSB, RF_BITRATEMSB_55555}, // default: 4.8 KBPS
  /* 0x04 */
  { REG_BITRATELSB, RF_BITRATELSB_55555},
  /* 0x05 */
  { REG_FDEVMSB, RF_FDEVMSB_50000}, // default: 5KHz, (FDEV + BitRate / 2 <= 500KHz)
  /* 0x06 */
  { REG_FDEVLSB, RF_FDEVLSB_50000},

  // looks like PA1 and PA2 are not implemented on RFM69W, hence the max output power is 13dBm
  // +17dBm and +20dBm are possible on RFM69HW
  // +13dBm formula: Pout = -18 + OutputPower (with PA0 or PA1**)
  // +17dBm formula: Pout = -14 + OutputPower (with PA1 and PA2)**
  // +20dBm formula: Pout = -11 + OutputPower (with PA1 and PA2)** and high power PA settings (section 3.3.7 in datasheet)
  ///* 0x11 */ { REG_PALEVEL, RF_PALEVEL_PA0_ON | RF_PALEVEL_PA1_OFF | RF_PALEVEL_PA2_OFF | RF_PALEVEL_OUTPUTPOWER_11111},
  ///* 0x13 */ { REG_OCP, RF_OCP_ON | RF_OCP_TRIM_95 }, // over current protection (default is 95mA)

  // RXBW defaults are { REG_RXBW, RF_RXBW_DCCFREQ_010 | RF_RXBW_MANT_24 | RF_RXBW_EXP_5} (RxBw: 10.4KHz)
  /* 0x19 */
  { REG_RXBW, RF_RXBW_DCCFREQ_010 | RF_RXBW_MANT_16 | RF_RXBW_EXP_2 }, // (BitRate < 2 * RxBw)
  //for BR-19200: /* 0x19 */ { REG_RXBW, RF_RXBW_DCCFREQ_010 | RF_RXBW_MANT_24 | RF_RXBW_EXP_3 },
  /* 0x25 */
  { REG_DIOMAPPING1, RF_DIOMAPPING1_DIO0_01 }, // DIO0 is the only IRQ we're using
  /* 0x26 */
  { REG_DIOMAPPING2, RF_DIOMAPPING2_CLKOUT_OFF }, // DIO5 ClkOut disable for power saving
  /* 0x28 */
  { REG_IRQFLAGS2, RF_IRQFLAGS2_FIFOOVERRUN }, // writing to this bit ensures that the FIFO & status flags are reset
  /* 0x29 */
  { REG_RSSITHRESH, 220 }, // must be set to dBm = (-Sensitivity / 2), default is 0xE4 = 228 so -114dBm
  ///* 0x2D */ { REG_PREAMBLELSB, RF_PREAMBLESIZE_LSB_VALUE } // default 3 preamble bytes 0xAAAAAA
  /* 0x2E */
  { REG_SYNCCONFIG, RF_SYNC_ON | RF_SYNC_FIFOFILL_AUTO | RF_SYNC_SIZE_2 | RF_SYNC_TOL_0 },
  /* 0x2F */
  { REG_SYNCVALUE1, 0x2D },      // attempt to make this compatible with sync1 byte of RFM12B lib
  /* 0x37 */
  { REG_PACKETCONFIG1, RF_PACKET1_FORMAT_VARIABLE | RF_PACKET1_DCFREE_OFF | RF_PACKET1_CRC_ON | RF_PACKET1_CRCAUTOCLEAR_ON | RF_PACKET1_ADRSFILTERING_OFF },
  /* 0x38 */
  { REG_PAYLOADLENGTH, RF69_FIFO_SIZE }, // in variable length mode: the max frame size, not used in TX
  ///* 0x39 */ { REG_NODEADRS, nodeID }, // turned off because we're not using address filtering
  /* 0x3C */
  { REG_FIFOTHRESH, RF_FIFOTHRESH_TXSTART_FIFONOTEMPTY | RF69_HEADER_SIZE }, // TX on FIFO not empty, header size = 4
  /* 0x3D */
  { REG_PACKETCONFIG2, RF_PACKET2_RXRESTARTDELAY_2BITS | RF_PACKET2_AUTORXRESTART_ON | RF_PACKET2_AES_OFF }, // RXRESTARTDELAY must match transmitter PA ramp-down time (bitrate dependent)
  //for BR-19200: /* 0x3D */ { REG_PACKETCONFIG2, RF_PACKET2_RXRESTARTDELAY_NONE | RF_PACKET2_AUTORXRESTART_ON | RF_PACKET2_AES_OFF }, // RXRESTARTDELAY must match transmitter PA ramp-down time (bitrate dependent)
  /* 0x6F */
  { REG_TESTDAGC, RF_DAGC_IMPROVED_LOWBETA0 }, // run DAGC continuously in RX mode for Fading Margin Improvement, recommended default for AfcLowBetaOn=0
  {255, 0}
};

/* protected functions ====================================================== */


/** ----------------------------------------------------------------------------
 * @brief wait for DIO0 to turn HIGH
 * @param rf
 * @param timeout
 * @return true irq occured, false timeout, -1 erreur
 */
int
iRf69WaitIrq (xRf69 * rf, int timeout) {
  TRY_INIT();
  int ret;
  timer_t t;

  TRY (t = xRf69TimerNew ());
  vRf69TimerStart (t, timeout);
  while ( (bRf69ReadIrqPin (rf) == false) && ! bRf69Timeout (t))
    ;

  ret = bRf69Timeout (t) ? false : true;
  vRf69TimerDelete (t);
  return ret;
}

/** ----------------------------------------------------------------------------
 * @brief wait for ModeReady
 * @param rf
 * @param timeout
 * @return true ready, false timeout, -1 erreur
 */
int
iRf69WaitForReady (xRf69 * rf, int timeout) {
  TRY_INIT();
  uint8_t b;
  int ret;
  timer_t t;

  TRY (t = xRf69TimerNew ());
  vRf69TimerStart (t, timeout);
  do {

    TRY (b = iRf69ReadReg (rf, REG_IRQFLAGS1));
  }
  while ( ( (b & RF_IRQFLAGS1_MODEREADY) == 0) && ! bRf69Timeout (t));

  ret = bRf69Timeout (t) ? false : (b & RF_IRQFLAGS1_MODEREADY) != 0;
  vRf69TimerDelete (t);
  return ret;
}

/** ----------------------------------------------------------------------------
 * @brief wait to send
 * @param rf
 * @param timeout
 * @return true ready to send, false timeout, -1 erreur
 */
int
iRf69WaitToSend (xRf69 * rf, int timeout) {
  TRY_INIT();
  bool bCanSend;
  int ret;
  timer_t t;

  TRY (t = xRf69TimerNew ());
  vRf69TimerStart (t, timeout);
  TRY (iRf69AvoidRxDeadLocks (rf));

  do {

    TRY (bCanSend = iRf69CanSend (rf));
    if (! bCanSend) {

      TRY (iRf69ReceiveDone (rf));
    }
  }
  while ( (bCanSend == false) && ! bRf69Timeout (t));

  ret = bRf69Timeout (t) ? false : bCanSend;
  vRf69TimerDelete (t);
  return ret;
}

/** ----------------------------------------------------------------------------
 * @brief
 * @param rf
 * @param timeout
 * @return true written, false timeout, -1 erreur
 */
int
iRf69WriteRegWithCheck (xRf69 * rf, uint8_t reg, uint8_t data, int timeout) {
  TRY_INIT();
  uint8_t b;
  int ret;
  timer_t t;

  TRY (t = xRf69TimerNew ());
  vRf69TimerStart (t, timeout);
  do {
    TRY (iRf69WriteReg (rf, reg, data));
    TRY (b = iRf69ReadReg (rf, reg));
  }
  while ( (b != (int) data) && ! bRf69Timeout (t));

  ret = bRf69Timeout (t) ? 0 : true;
  vRf69TimerDelete (t);
  return ret;
}

// -----------------------------------------------------------------------------
// Utilisé sous interruption
int
iRf69SetHighPowerRegs (xRf69 * rf, bool bOn) {

  TRY (iRf69WriteReg (rf, REG_TESTPA1, bOn ? 0x5D : 0x55));
  return iRf69WriteReg (rf, REG_TESTPA2, bOn ? 0x7C : 0x70);
}

// -----------------------------------------------------------------------------
// Utilisé sous interruption
int
iRf69SetMode (xRf69 * rf, eRf69Mode eNewMode) {

  if (eNewMode != rf->mode) {
    TRY_INIT();
    uint8_t b;

    TRY (b = iRf69ReadReg (rf, REG_OPMODE));
    b &= 0xE3;

    switch (eNewMode) {

      case eRf69ModeTx:
        TRY (iRf69WriteReg (rf, REG_OPMODE, b | RF_OPMODE_TRANSMITTER));
        if (rf->is_rfm69hw) {

          TRY (iRf69SetHighPowerRegs (rf, true));
        }
        break;

      case eRf69ModeRx:
        TRY (iRf69WriteReg (rf, REG_OPMODE, b | RF_OPMODE_RECEIVER));
        if (rf->is_rfm69hw) {

          TRY (iRf69SetHighPowerRegs (rf, false));
        }
        break;

      case eRf69ModeSynth:
        TRY (iRf69WriteReg (rf, REG_OPMODE, b | RF_OPMODE_SYNTHESIZER));
        break;

      case eRf69ModeStandby:
        TRY (iRf69WriteReg (rf, REG_OPMODE, b | RF_OPMODE_STANDBY));
        break;

      case eRf69ModeSleep:
        TRY (iRf69WriteReg (rf, REG_OPMODE, b | RF_OPMODE_SLEEP));
        break;

      default:
        return -1;
    }

    /* we are using packet mode, so this check is not really needed
     * but waiting for mode ready is necessary when going from sleep
     * because the FIFO may not be immediately available from previous mode
     */
    if (rf->mode == eRf69ModeSleep) {

      TRY (iRf69WaitForReady (rf, 50));
    }

    rf->mode = eNewMode;
  }
  return 0;
}

// -----------------------------------------------------------------------------
// Utilisé sous interruption
int
iRf69AvoidRxDeadLocks (xRf69 * rf) {
  TRY_INIT();
  uint8_t b;

  // avoid RX deadlocks
  TRY (b = iRf69ReadReg (rf, REG_PACKETCONFIG2));
  b = (b & 0xFB) | RF_PACKET2_RXRESTART;
  TRY (iRf69WriteReg (rf, REG_PACKETCONFIG2, b));
  return 0;
}

// -----------------------------------------------------------------------------
// Utilisé sous interruption
int
iRf69StartReceiving (xRf69 * rf) {
  TRY_INIT();

  rf->data_len = 0;
  rf->hdr.sender = 0;
  rf->hdr.dest = 0;
  rf->hdr.payload_len = 0;
  rf->hdr.ctl = 0;
  rf->rssi = 0;

  TRY (iRf69AvoidRxDeadLocks (rf));

  // set DIO0 to "PAYLOADREADY" in receive mode
  TRY (iRf69WriteReg (rf, REG_DIOMAPPING1, RF_DIOMAPPING1_DIO0_01));
  return iRf69SetMode (rf, eRf69ModeRx);
}

// -----------------------------------------------------------------------------
int
iRf69SendFrame (xRf69 * rf, uint8_t toAddress,
                const void * tx_buffer, uint8_t tx_len,
                bool bRequestACK, bool bSendAck) {
  TRY_INIT();

  // turn off receiver to prevent reception while filling fifo
  TRY (iRf69SetMode (rf, eRf69ModeStandby));

  // wait for ModeReady
  TRY (iRf69WaitForReady (rf, 50));

  // DIO0 is "Packet Sent"
  TRY (iRf69WriteReg (rf, REG_DIOMAPPING1, RF_DIOMAPPING1_DIO0_00));

  if (tx_len > RF69_MAX_DATA_LEN) {

    tx_len = RF69_MAX_DATA_LEN;
  }

  rf->data[0] = tx_len + 3;
  rf->data[1] = toAddress;
  rf->data[2] = rf->node_id;
  rf->data[3] = bSendAck ? RF69_ACK : RF69_ACKREQ;
  memcpy (&rf->data[4], tx_buffer, tx_len);

  TRY (iRf69WriteBlock (rf, REG_FIFO, rf->data, RF69_HEADER_SIZE + tx_len));

  // no need to wait for transmit mode to be ready since its handled by the radio
  TRY (iRf69SetMode (rf, eRf69ModeTx));

  // wait for DIO0 to turn HIGH signaling transmission finish
  TRY (iRf69WaitIrq (rf, RF69_TX_TIMEOUT_MS));

  return iRf69SetMode (rf, eRf69ModeStandby);
}

// -----------------------------------------------------------------------------
void
vRf69Isr (xRf69 * rf) {

  if (rf->mode == eRf69ModeRx)  {
    TRY_INIT();
    uint8_t irqflags;

    TRYV (irqflags = iRf69ReadReg (rf, REG_IRQFLAGS2));

    if (irqflags & RF_IRQFLAGS2_PAYLOADREADY) {

      TRYV (iRf69SetMode (rf, eRf69ModeStandby));

      if (irqflags & RF_IRQFLAGS2_FIFOLEVEL) {

        TRYV (iRf69ReadBlock (rf, REG_FIFO,
                              (uint8_t *) &rf->hdr, sizeof (rf->hdr)));
        if (rf->promiscuous || rf->hdr.dest == rf->node_id ||
            rf->hdr.dest == RF69_BROADCAST_ADDR) {

          rf->data_len = MIN (rf->hdr.payload_len, RF69_FIFO_SIZE) - 3;
          TRYV (iRf69ReadBlock (rf, REG_FIFO, rf->data, rf->data_len));
          if (rf->data_len < RF69_MAX_DATA_LEN) {
            rf->data[rf->data_len] = 0;  // add null at end of string
          }

          TRYV (iRf69SetMode (rf, eRf69ModeRx));
          TRYV (rf->rssi = iRf69Rssi (rf, false));
          return;
        }
      }

      rf->hdr.payload_len = 0;
      iRf69StartReceiving (rf); // fifo flush
    }
  }
}

/* internal public functions ================================================ */

// -----------------------------------------------------------------------------
int
iRf69Open (xRf69 * rf, eRf69Band eBand, uint8_t ucNodeId, uint8_t ucNetId) {
  TRY_INIT();
  uint8_t band;
  int i = 0, ret;

  ret = iRf69WriteRegWithCheck (rf, REG_SYNCVALUE1, 0xAA, 50);
  if (ret != true) {
    return -1;
  }
  ret = iRf69WriteRegWithCheck (rf, REG_SYNCVALUE1, 0x55, 50);
  if (ret != true) {
    return -1;
  }

  do {

    TRY (ret = iRf69WriteConstElmt (rf, &xInitConfigPgm[i++]));
  }
  while (ret);

  band = (eBand == eRf69Band315Mhz ? RF_FRFMSB_315 :
          (eBand == eRf69Band433Mhz ? RF_FRFMSB_433 :
           (eBand == eRf69Band868Mhz ? RF_FRFMSB_868 :
            RF_FRFMSB_915)));

  /* 0x07 - 0x09 */
  for (uint8_t reg = REG_FRFMSB; reg <= REG_FRFLSB; reg++) {

    TRY (iRf69WriteReg (rf, reg, band));
  }

  /* 0x30 NETWORK ID */
  TRY (iRf69WriteReg (rf, REG_SYNCVALUE2, ucNetId));

  // Encryption is persistent between resets and can trip you up during debugging.
  // Disable it during initialization so we always start from a known state.
  TRY (iRf69SetEncryptKey (rf, 0));

  // called regardless if it's a RFM69W or RFM69HW
  TRY (iRf69SetHighPower (rf, rf->is_rfm69hw));
  TRY (iRf69SetMode (rf, eRf69ModeStandby));
  TRY (iRf69WaitForReady (rf, 50));

  rf->node_id = ucNodeId;
  return 0;
}

// -----------------------------------------------------------------------------
int
iRf69CanSend (xRf69 * rf) {
  TRY_INIT();
  int rssi;

  TRY (rssi = iRf69Rssi (rf, false));

  // if signal stronger than -100dBm is detected assume channel activity
  if ( (rf->mode == eRf69ModeRx) &&
       (rf->hdr.payload_len == 0) &&
       (rssi >= RF69_CSMA_MIN_LEVEL_DBM)) {

    TRY (iRf69SetMode (rf, eRf69ModeStandby));
    return true;
  }
  return false;
}

// -----------------------------------------------------------------------------
int
iRf69Send (xRf69 * rf, uint8_t toAddress,
           const void * tx_buffer, uint8_t tx_len, bool bRequestAck) {
  TRY_INIT();

  TRY (iRf69WaitToSend (rf, RF69_CSMA_TIMEOUT_MS));
  return iRf69SendFrame (rf, toAddress, tx_buffer, tx_len, bRequestAck, false);
}

// -----------------------------------------------------------------------------
// to increase the chance of getting a packet across, call this function instead of send
// and it handles all the ACK requesting/retrying for you :)
// The only twist is that you have to manually listen to ACK requests on the other side and send back the ACKs
// The reason for the semi-automaton is that the lib is interrupt driven and
// requires user action to read the received data and decide what to do with it
// replies usually take only 5..8ms at 50kbps@915MHz
int
iRf69SendWithRetry (xRf69 * rf, uint8_t toAddress, const void * tx_buffer, uint8_t tx_len,
                    uint8_t retries, int retryWaitTime) {
  TRY_INIT();
  int iAckReceived = false;
  timer_t t;

  TRY (t = xRf69TimerNew ());

  while ( (retries) && (iAckReceived == false)) {

    iAckReceived = iRf69Send (rf, toAddress, tx_buffer, tx_len, true);
    if (iAckReceived < 0) {

      goto SendWithRetryExit;
    }
    vRf69TimerStart (t, retryWaitTime);
    while (!bRf69Timeout (t) && (iAckReceived == false)) {

      iAckReceived = iRf69AckReceived (rf, toAddress);
      if (iAckReceived < 0) {

        goto SendWithRetryExit;
      }
    }
    retries--;
  }

SendWithRetryExit:
  vRf69TimerDelete (t);
  return iAckReceived;
}

// -----------------------------------------------------------------------------
// should be called immediately after reception in case sender wants ACK
int
iRf69SendAck (xRf69 * rf, const void * tx_buffer, uint8_t tx_len) {
  TRY_INIT();
  uint8_t sender;
  int rssi;

  // TWS added to make sure we don't end up in a timing race and infinite loop sending Acks
  rf->hdr.ctl &= ~RF69_ACKREQ;
  sender = rf->hdr.sender;
  rssi = rf->rssi; // save payload received rf->rssi value

  TRY (iRf69WaitToSend (rf, RF69_CSMA_TIMEOUT_MS));

  // TWS: Restore SenderID after it gets wiped out by receiveDone()
  rf->hdr.sender = sender;
  TRY (iRf69SendFrame (rf, sender, tx_buffer, tx_len, false, true));
  rf->rssi = rssi; // restore payload rf->rssi
  return 0;
}

// -----------------------------------------------------------------------------
// should be polled immediately after sending a packet with ACK request
int iRf69AckReceived (xRf69 * rf, uint8_t fromNodeId) {
  TRY_INIT();
  bool bReceivedDone;

  TRY (bReceivedDone = iRf69ReceiveDone (rf));
  if (bReceivedDone) {

    return (rf->hdr.sender == fromNodeId || fromNodeId == RF69_BROADCAST_ADDR)
           && (rf->hdr.ctl & RF69_ACK);
  }
  return false;
}

// -----------------------------------------------------------------------------
// check whether an ACK was requested in the last received packet
// (non-broadcasted packet)
int iRf69AckRequested (xRf69 * rf) {

  return (rf->hdr.ctl & RF69_ACKREQ) && (rf->hdr.dest != RF69_BROADCAST_ADDR);
}

// -----------------------------------------------------------------------------
int
iRf69DataLength (xRf69 * rf) {

  return rf->data_len;
}

// -----------------------------------------------------------------------------
const char *
sRf69Data (xRf69 * rf) {

  return (const char *) rf->data;
}

// -----------------------------------------------------------------------------
int
iRf69SetEncryptKey (xRf69 * rf, const char* key) {
  TRY_INIT();
  uint8_t b;

  TRY (iRf69SetMode (rf, eRf69ModeStandby));
  if (key != 0) {

    if (strlen (key) != 16) {

      return -1;
    }
    TRY (iRf69WriteBlock (rf, REG_AESKEY1, (const uint8_t *) key, 16));
  }

  TRY (b = iRf69ReadReg (rf, REG_PACKETCONFIG2));
  b = (b & 0xFE) | (key ? 1 : 0);

  return iRf69WriteReg (rf, REG_PACKETCONFIG2, b);
}

// -----------------------------------------------------------------------------
int
iRf69SetNodeId (xRf69 * rf, uint8_t ucNodeId) {

  TRY (iRf69WriteReg (rf, REG_NODEADRS, ucNodeId));
  rf->node_id = ucNodeId;
  return 0;
}

// -----------------------------------------------------------------------------
int
iRf69NodeId (xRf69 * rf) {
  TRY_INIT();
  uint8_t b;

  TRY (b = iRf69ReadReg (rf, REG_NODEADRS));
  return b;
}

// -----------------------------------------------------------------------------
int
iRf69SetNetworkId (xRf69 * rf, uint8_t ucNetId) {

  return iRf69WriteReg (rf, REG_SYNCVALUE2, ucNetId);
}

// -----------------------------------------------------------------------------
int
iRf69NetworkId (xRf69 * rf) {
  TRY_INIT();
  uint8_t b;

  TRY (b = iRf69ReadReg (rf, REG_SYNCVALUE2));
  return b;
}

// -----------------------------------------------------------------------------
long
lRf69Frequency (xRf69 * rf) {
  TRY_INIT();
  uint8_t b;
  long f = 0;
  uint8_t ls = 16;

  for (uint8_t reg = REG_FRFMSB; reg <= REG_FRFLSB; reg++) {

    TRY (b = iRf69ReadReg (rf, REG_FRFMSB));
    f += ( (long) b) << ls;
    ls -= 8;
  }

  return RF69_FSTEP * f;
}

// -----------------------------------------------------------------------------
int
iRf69SetFrequency (xRf69 * rf, uint32_t ulFreq) {
  TRY_INIT();
  uint8_t oldMode = rf->mode;

  if (oldMode == eRf69ModeTx) {

    TRY (iRf69SetMode (rf, eRf69ModeRx));
  }
  ulFreq /= RF69_FSTEP; // divide down by FSTEP to get FRF
  TRY (iRf69WriteReg (rf, REG_FRFMSB, ulFreq >> 16));
  TRY (iRf69WriteReg (rf, REG_FRFMID, ulFreq >> 8));
  TRY (iRf69WriteReg (rf, REG_FRFLSB, ulFreq));

  if (oldMode == eRf69ModeRx) {

    TRY (iRf69SetMode (rf, eRf69ModeSynth));
  }
  return iRf69SetMode (rf, oldMode);
}

// -----------------------------------------------------------------------------
// has to be called after iRf69Open() for RFM69HW
// for RFM69HW only: you must call iRf69SetHighPower(true) after iRf69Open() or else transmission won't work
int
iRf69SetHighPower (xRf69 * rf, bool bOn) {
  TRY_INIT();
  uint8_t palevel;

  rf->is_rfm69hw = bOn;

  TRY (iRf69WriteReg (rf, REG_OCP, rf->is_rfm69hw ? RF_OCP_OFF : RF_OCP_ON));

  if (rf->is_rfm69hw) {
    uint8_t b;
    // turning ON

    TRY (b = iRf69ReadReg (rf, REG_PALEVEL));
    // enable P1 & P2 amplifier stages
    palevel = (b & 0x1F) | RF_PALEVEL_PA1_ON | RF_PALEVEL_PA2_ON;
  }
  else {
    // enable P0 only
    palevel = RF_PALEVEL_PA0_ON | rf->power_level;
  }

  return iRf69WriteReg (rf, REG_PALEVEL, palevel);
}

// -----------------------------------------------------------------------------
// reduce/increase transmit power level
// set *transmit/TX* output power: 0=min, 31=max
// this results in a "weaker" transmitted signal, and directly results in a lower rf->rssi at the receiver
// the power configurations are explained in the SX1231H datasheet (Table 10 on p21; RegPaLevel p66): http://www.semtech.com/images/datasheet/sx1231h.pdf
// valid ucLevel parameter values are 0-31 and result in a directly proportional effect on the output/transmission power
// this function implements 2 modes as follows:
//       - for RFM69W the range is from 0-31 [-18dBm to 13dBm] (PA0 only on RFIO pin)
//       - for RFM69HW the range is from 0-31 [5dBm to 20dBm]  (PA1 & PA2 on PA_BOOST pin & high Power PA settings - see section 3.3.7 in datasheet, p22)
int
iRf69SetPowerLevel (xRf69 * rf, uint8_t ucLevel) {
  TRY_INIT();
  uint8_t b;

  rf->power_level = (ucLevel > 31 ? 31 : ucLevel);
  if (rf->is_rfm69hw) {

    rf->power_level /= 2;
  }

  TRY (b = iRf69ReadReg (rf, REG_PALEVEL));
  return iRf69WriteReg (rf, REG_PALEVEL, (b & 0xE0) | rf->power_level);
}

// -----------------------------------------------------------------------------
int
iRf69PowerLevel (xRf69 * rf) {
  TRY_INIT();
  uint8_t b;

  TRY (b = iRf69ReadReg (rf, REG_PALEVEL));
  b &= 0x1F;
  if (rf->is_rfm69hw) {

    b *= 2;
  }
  return b;
}

// -----------------------------------------------------------------------------
// get CMOS temperature (8bit) in centigrade
// RF69_TEMP_SLOPE puts reading in the ballpark, user can add additional correction
int
iRf69Temperature (xRf69 * rf, uint8_t ucCalFactor) {
  TRY_INIT();
  uint8_t b;

  TRY (iRf69SetMode (rf, eRf69ModeStandby));
  TRY (iRf69WriteReg (rf, REG_TEMP1, RF_TEMP1_MEAS_START));

  do {
    TRY (b = iRf69ReadReg (rf, REG_TEMP1));
  }
  while ( (b & RF_TEMP1_MEAS_RUNNING));

  TRY (b = iRf69ReadReg (rf, REG_TEMP2));

  // 'complement' corrects the slope, rising temp = rising val
  return ~b + RF69_TEMP_SLOPE + ucCalFactor;
}

// -----------------------------------------------------------------------------
// Utilisé sous interruption
int
iRf69Rssi (xRf69 * rf, bool bForceTrigger) {
  TRY_INIT();
  uint8_t b;
  int16_t rssi = 0;

  if (bForceTrigger) {

    // RSSI trigger not needed if DAGC is in continuous mode
    TRY (iRf69WriteReg (rf, REG_RSSICONFIG, RF_RSSI_START));
    do {
      // wait for RSSI_Ready
      TRY (b = iRf69ReadReg (rf, REG_RSSICONFIG));
    }
    while ( (b & RF_RSSI_DONE) == 0);
  }

  TRY (b = iRf69ReadReg (rf, REG_RSSIVALUE));
  rssi = b >> 1;
  return rssi;
}

// -----------------------------------------------------------------------------
int iRf69SetPromiscuous (xRf69 * rf, bool bOn) {

  rf->promiscuous = bOn;
  //iRf69WriteReg(REG_PACKETCONFIG1, (iRf69ReadReg(REG_PACKETCONFIG1) & 0xF9) | (bOn ? RF_PACKET1_ADRSFILTERING_OFF : RF_PACKET1_ADRSFILTERING_NODEBROADCAST));
  return 0;
}

// -----------------------------------------------------------------------------
bool
bRf69isPromiscuous (const xRf69 * rf) {

  return rf->promiscuous;
}

// -----------------------------------------------------------------------------
int
iRf69Sleep (xRf69 * rf) {

  return iRf69SetMode (rf, eRf69ModeSleep);
}

// -----------------------------------------------------------------------------
int
iRf69RcCalibration (xRf69 * rf) {
  TRY_INIT();
  uint8_t b;

  TRY (iRf69WriteReg (rf, REG_OSC1, RF_OSC1_RCCAL_START));
  do {
    // wait for RCCAL_DONE
    TRY (b = iRf69ReadReg (rf, REG_OSC1));
  }
  while ( (b & RF_OSC1_RCCAL_DONE) == 0);
  return 0;
}

/* ========================================================================== */
