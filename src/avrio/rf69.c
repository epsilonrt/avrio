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
  {
    /*
     * 0x01
     */
    REG_OPMODE,
    RF_OPMODE_SEQUENCER_ON | RF_OPMODE_LISTEN_OFF | RF_OPMODE_STANDBY
  },
  {
    /*
     * 0x02
     * no shaping
     */
    REG_DATAMODUL,
    RF_DATAMODUL_DATAMODE_PACKET |
    RF_DATAMODUL_MODULATIONTYPE_FSK | RF_DATAMODUL_MODULATIONSHAPING_00
  },
  {
    /*
     * 0x03
     * default: 4.8 KBPS
     */
    REG_BITRATEMSB,
    RF_BITRATEMSB_55555
  },
  {
    /*
     * 0x04
     */
    REG_BITRATELSB,
    RF_BITRATELSB_55555
  },
  {
    /*
     * 0x05
     * default: 5KHz, (FDEV + BitRate / 2 <= 500KHz)
     */
    REG_FDEVMSB,
    RF_FDEVMSB_50000
  },
  {
    /*
     * 0x06
     */
    REG_FDEVLSB,
    RF_FDEVLSB_50000
  },
  /*
   * 0x11
   * looks like PA1 and PA2 are not implemented on RFM69W, hence the max
   * output power is 13dBm:
   * +17dBm and +20dBm are possible on RFM69HW
   * +13dBm formula: Pout = -18 + OutputPower (with PA0 or PA1)**
   * +17dBm formula: Pout = -14 + OutputPower (with PA1 and PA2)**
   * +20dBm formula: Pout = -11 + OutputPower (with PA1 and PA2)** and high
   * power PA settings (** section 3.3.7 in datasheet)
   */
//  {
//    REG_PALEVEL,
//    RF_PALEVEL_PA0_ON | RF_PALEVEL_PA1_OFF | RF_PALEVEL_PA2_OFF |
//    RF_PALEVEL_OUTPUTPOWER_11111
//  },
  /*
   * 0x13
   * over current protection (default is 95mA)
   */
//  {
//    REG_OCP,
//    RF_OCP_ON | RF_OCP_TRIM_95
//  },
  {
    /*
     * 0x19
     * (BitRate < 2 * RxBw)
     * RXBW defaults are (RxBw: 10.4KHz) :
     * { REG_RXBW, RF_RXBW_DCCFREQ_010 | RF_RXBW_MANT_24 | RF_RXBW_EXP_5}
     */
    REG_RXBW,
    RF_RXBW_DCCFREQ_010 | RF_RXBW_MANT_16 | RF_RXBW_EXP_2
    // for BR-19200: RF_RXBW_DCCFREQ_010 | RF_RXBW_MANT_24 | RF_RXBW_EXP_3
  },
  {
    /*
     * 0x25
     * DIO0 is the only IRQ we're using
     */
    REG_DIOMAPPING1,
    RF_DIOMAPPING1_DIO0_01
  },
  {
    /*
     * 0x26
     * DIO5 ClkOut disable for power saving
     */
    REG_DIOMAPPING2,
    RF_DIOMAPPING2_CLKOUT_OFF
  },
  {
    /*
     * 0x28
     * writing to this bit ensures that the FIFO & status flags are reset
     */
    REG_IRQFLAGS2,
    RF_IRQFLAGS2_FIFOOVERRUN
  },
  {
    /*
     * 0x29
     * must be set to dBm = (-Sensitivity / 2), default is 0xE4 = 228 so -114dBm
     */
    REG_RSSITHRESH,
    220
  },
  /*
   * 0x2D
   * default 3 preamble bytes 0xAAAAAA
   */
//  {
//    REG_PREAMBLELSB,
//    RF_PREAMBLESIZE_LSB_VALUE
//  },
  {
    /*
     * 0x2E
     */
    REG_SYNCCONFIG,
    RF_SYNC_ON | RF_SYNC_FIFOFILL_AUTO | RF_SYNC_SIZE_2 | RF_SYNC_TOL_0
  },
  {
    /*
     * 0x2F
     * attempt to make this compatible with sync1 byte of RFM12B lib
     */
    REG_SYNCVALUE1,
    0x2D
  },
  {
    /*
     * 0x37
     */
    REG_PACKETCONFIG1,
    RF_PACKET1_FORMAT_VARIABLE | RF_PACKET1_DCFREE_OFF | RF_PACKET1_CRC_ON |
    RF_PACKET1_CRCAUTOCLEAR_ON | RF_PACKET1_ADRSFILTERING_OFF
  },
  {
    /*
     * 0x38
     * in variable length mode: the max frame size, not used in TX
     */
    REG_PAYLOADLENGTH,
    RF69_FIFO_SIZE
  },
  {
    /*
     * 0x3A
     */
    REG_BROADCASTADRS,
    RF69_BROADCAST_ADDR
  },
  {
    /*
     * 0x3C
     * TX on FIFO not empty, header size = 4
     */
    REG_FIFOTHRESH,
    RF_FIFOTHRESH_TXSTART_FIFONOTEMPTY | RF69_HEADER_SIZE
  },
  {
    /*
     * 0x3D
     * RXRESTARTDELAY must match transmitter PA ramp-down time (bitrate dependent)
     */
    REG_PACKETCONFIG2,
    RF_PACKET2_RXRESTARTDELAY_2BITS | RF_PACKET2_AUTORXRESTART_ON | RF_PACKET2_AES_OFF
    // for BR-19200: RF_PACKET2_RXRESTARTDELAY_NONE | RF_PACKET2_AUTORXRESTART_ON | RF_PACKET2_AES_OFF
  },
  {
    /*
     * 0x6F
     * run DAGC continuously in RX mode for Fading Margin Improvement,
     * recommended default for AfcLowBetaOn=0
     */
    REG_TESTDAGC,
    RF_DAGC_IMPROVED_LOWBETA0
  },
  {255, 0} // the end
};


/* internal public functions ================================================ */

// -----------------------------------------------------------------------------
int
iRf69Open (xRf69 * rf, eRf69Band eBand, uint8_t ucNodeId, uint8_t ucNetId) {

  if (!rf->is_open) {

    TRY_INT();
    uint8_t band;
    int i = 0, ret;

    TRY_ERR(iRf69WriteRegWithCheck (rf, REG_SYNCVALUE1, 0xAA, 50));
    TRY_ERR(iRf69WriteRegWithCheck (rf, REG_SYNCVALUE1, 0x55, 50));

    do {

      TRY_ERR (ret = iRf69WriteConstElmt (rf, &xInitConfigPgm[i++]));
    }
    while (ret);

    band = (eBand == eRf69Band315Mhz ? RF_FRFMSB_315 :
            (eBand == eRf69Band433Mhz ? RF_FRFMSB_433 :
             (eBand == eRf69Band868Mhz ? RF_FRFMSB_868 :
              RF_FRFMSB_915)));

    /* 0x07 - 0x09 */
    for (uint8_t reg = REG_FRFMSB; reg <= REG_FRFLSB; reg++) {

      TRY_ERR (iRf69WriteReg (rf, reg, band));
    }

    /* 0x30 NETWORK ID */
    TRY_ERR (iRf69WriteReg (rf, REG_SYNCVALUE2, ucNetId));
    /* 0x39 NODE ID */
    TRY_ERR (iRf69WriteReg (rf, REG_NODEADRS, ucNodeId));

    // Encryption is persistent between resets and can trip you up during debugging.
    // Disable it during initialization so we always start from a known state.
    TRY_ERR (iRf69SetEncryptKey (rf, 0));

    TRY_ERR (iRf69SetHighPower (rf, false));
    TRY_ERR (iRf69SetPowerLevel (rf, RF69_DEFAULT_POWER));
    TRY_ERR (iRf69SetMode (rf, eRf69ModeStandby));
    TRY_ERR (iRf69SetPromiscuous (rf, true));
    TRY_ERR (iRf69WaitForReady (rf, 50));

    rf->node_id = ucNodeId;
    rf->is_open = true;
  }
  return 0;
}

// -----------------------------------------------------------------------------
int
iRf69Close (xRf69 * rf) {
  // TODO
  return 0;
}

// -----------------------------------------------------------------------------
int
iRf69CanSend (xRf69 * rf) {
  TRY_INT();
  int rssi;

  TRY_ERR (rssi = iRf69Rssi (rf, false));

  if ( (rf->mode == eRf69ModeRx) &&
       (rf->hdr.payload_len == 0) &&
       (rssi < RF69_CSMA_MIN_LEVEL_DBM)) {

    /* Nous sommes en mode réception, aucun paquet reçu, pas de réception en cours
     * (if signal stronger than -100dBm is detected assume channel activity
     * on peut passer en mode attente... */
    TRY_ERR (iRf69SetMode (rf, eRf69ModeStandby));
    return true;
  }
  return false;
}

// -----------------------------------------------------------------------------
int
iRf69Send (xRf69 * rf, uint8_t toAddress,
           const void * tx_buffer, uint8_t tx_len, bool bRequestAck) {
  TRY_INT();

  TRY_ERR (iRf69WaitToSend (rf, RF69_CSMA_TIMEOUT_MS));
  return iRf69SendFrame (rf, toAddress, tx_buffer, tx_len, bRequestAck, false);
}

// -----------------------------------------------------------------------------
int
iRf69SendWithRetry (xRf69 * rf, uint8_t toAddress,
                    const void * tx_buffer, uint8_t tx_len,
                    uint8_t retries, int retryWaitTime) {
  TRY_INT();
  int iAckReceived = false;
  timer_t t;

  TRY_ERR (t = xRf69TimerNew ());

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
int
iRf69SendAck (xRf69 * rf, const void * tx_buffer, uint8_t tx_len) {
  TRY_INT();
  uint8_t sender;
  int rssi;

  // TWS added to make sure we don't end up in a timing race and infinite loop sending Acks
  rf->hdr.ctl &= ~RF69_ACKREQ;
  sender = rf->hdr.sender;
  rssi = rf->rssi; // save payload received rf->rssi value

  TRY_ERR (iRf69WaitToSend (rf, RF69_CSMA_TIMEOUT_MS));

  // TWS: Restore SenderID after it gets wiped out by receiveDone()
  rf->hdr.sender = sender;
  TRY_ERR (iRf69SendFrame (rf, sender, tx_buffer, tx_len, false, true));
  rf->rssi = rssi; // restore payload rf->rssi
  return 0;
}

// -----------------------------------------------------------------------------
int iRf69AckReceived (xRf69 * rf, uint8_t fromNodeId) {
  TRY_INT();
  bool bReceivedDone;

  TRY_ERR (bReceivedDone = iRf69ReceiveDone (rf));
  if (bReceivedDone) {

    return (rf->hdr.sender == fromNodeId || fromNodeId == RF69_BROADCAST_ADDR)
           && (rf->hdr.ctl & RF69_ACK);
  }
  return false;
}

// -----------------------------------------------------------------------------
int iRf69AckRequested (const xRf69 * rf) {

  return (rf->hdr.ctl & RF69_ACKREQ) && (rf->hdr.dest != RF69_BROADCAST_ADDR);
}

// -----------------------------------------------------------------------------
int
iRf69DataLength (const xRf69 * rf) {

  return rf->data_len;
}

// -----------------------------------------------------------------------------
const char *
sRf69Data (const xRf69 * rf) {

  return (const char *) rf->data;
}

// -----------------------------------------------------------------------------
int
iRf69isEncrypted (const xRf69 * rf) {
  TRY_INT();
  uint8_t b;

  TRY_ERR (b = iRf69ReadReg (rf, REG_PACKETCONFIG2));
  return (b & RF_PACKET2_AES_ON) != 0;
}

// -----------------------------------------------------------------------------
int
iRf69SetEncryptKey (xRf69 * rf, const char* key) {
  TRY_INT();
  uint8_t b;

  TRY_ERR (iRf69SetMode (rf, eRf69ModeStandby));
  if (key != 0) {

    if (strlen (key) != 16) {

      return -1;
    }
    TRY_ERR (iRf69WriteBlock (rf, REG_AESKEY1, (const uint8_t *) key, 16));
  }

  TRY_ERR (b = iRf69ReadReg (rf, REG_PACKETCONFIG2));
  b = (b & 0xFE) | (key ? RF_PACKET2_AES_ON : 0);

  return iRf69WriteReg (rf, REG_PACKETCONFIG2, b);
}

// -----------------------------------------------------------------------------
int
iRf69SetNodeId (xRf69 * rf, uint8_t ucNodeId) {

  TRY_ERR (iRf69WriteReg (rf, REG_NODEADRS, ucNodeId));
  rf->node_id = ucNodeId;
  return 0;
}

// -----------------------------------------------------------------------------
int
iRf69NodeId (const xRf69 * rf) {
  TRY_INT();
  uint8_t b;

  TRY_ERR (b = iRf69ReadReg (rf, REG_NODEADRS));
  return b;
}

// -----------------------------------------------------------------------------
int
iRf69SetNetworkId (const xRf69 * rf, uint8_t ucNetId) {

  return iRf69WriteReg (rf, REG_SYNCVALUE2, ucNetId);
}

// -----------------------------------------------------------------------------
int
iRf69NetworkId (const xRf69 * rf) {
  TRY_INT();
  uint8_t b;

  TRY_ERR (b = iRf69ReadReg (rf, REG_SYNCVALUE2));
  return b;
}

// -----------------------------------------------------------------------------
long
lRf69Frequency (const xRf69 * rf) {
  TRY_INT();
  uint8_t b;
  long f = 0;
  uint8_t ls = 16;

  for (uint8_t reg = REG_FRFMSB; reg <= REG_FRFLSB; reg++) {

    TRY_ERR (b = iRf69ReadReg (rf, reg));
    f += ( (long) b) << ls;
    ls -= 8;
  }

  return RF69_FSTEP * f;
}

// -----------------------------------------------------------------------------
int
iRf69SetFrequency (xRf69 * rf, uint32_t ulFreq) {
  TRY_INT();
  uint8_t oldMode = rf->mode;

  if (oldMode == eRf69ModeTx) {

    TRY_ERR (iRf69SetMode (rf, eRf69ModeRx));
  }
  ulFreq /= RF69_FSTEP; // divide down by FSTEP to get FRF
  TRY_ERR (iRf69WriteReg (rf, REG_FRFMSB, ulFreq >> 16));
  TRY_ERR (iRf69WriteReg (rf, REG_FRFMID, ulFreq >> 8));
  TRY_ERR (iRf69WriteReg (rf, REG_FRFLSB, ulFreq));

  if (oldMode == eRf69ModeRx) {

    TRY_ERR (iRf69SetMode (rf, eRf69ModeSynth));
  }
  return iRf69SetMode (rf, oldMode);
}

// -----------------------------------------------------------------------------
int
iRf69SetHighPower (xRf69 * rf, bool bOn) {
  TRY_INT();
  uint8_t palevel;


  TRY_ERR (iRf69WriteReg (rf, REG_OCP, bOn ? RF_OCP_OFF : RF_OCP_ON));

  if (bOn) {
    uint8_t b;

    // turning ON
    TRY_ERR (b = iRf69ReadReg (rf, REG_PALEVEL));
    // enable P1 & P2 amplifier stages
    palevel = (b & 0x1F) | RF_PALEVEL_PA1_ON | RF_PALEVEL_PA2_ON;
  }
  else {

    // enable P0 only
    palevel = RF_PALEVEL_PA0_ON | rf->power_level;
  }
  TRY_ERR (iRf69WriteReg (rf, REG_PALEVEL, palevel));

  rf->is_rfm69hw = bOn;
  return 0;
}

// -----------------------------------------------------------------------------
int
iRf69SetPowerLevel (xRf69 * rf, int level) {
  TRY_INT();
  uint8_t b;

  level = MIN (level, 31);
  if (rf->is_rfm69hw) {

    level /= 2;
  }

  TRY_ERR (b = iRf69ReadReg (rf, REG_PALEVEL));
  TRY_ERR (iRf69WriteReg (rf, REG_PALEVEL, (b & 0xE0) | level));
  rf->power_level = level;
  return 0;
}

// -----------------------------------------------------------------------------
int
iRf69PowerLevel (const xRf69 * rf) {
  TRY_INT();
  uint8_t b;

  TRY_ERR (b = iRf69ReadReg (rf, REG_PALEVEL));
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
iRf69Temperature (xRf69 * rf, int iCalFactor) {
  TRY_INT();
  int x;

  TRY_VAL (iRf69SetMode (rf, eRf69ModeStandby), INT_MAX);
  TRY_VAL (iRf69WriteReg (rf, REG_TEMP1, RF_TEMP1_MEAS_START), INT_MAX);

  do {
    TRY_VAL (x = iRf69ReadReg (rf, REG_TEMP1), INT_MAX);
  }
  while ( (x & RF_TEMP1_MEAS_RUNNING));

  TRY_VAL (x = iRf69ReadReg (rf, REG_TEMP2), INT_MAX);

  return -x + 20 + RF69_TEMP_CAL + iCalFactor;
}

// -----------------------------------------------------------------------------
// Utilisé sous interruption
int
iRf69Rssi (const xRf69 * rf, bool bForceTrigger) {
  TRY_INT();
  uint8_t b;

  if (bForceTrigger) {

    // RSSI trigger not needed if DAGC is in continuous mode
    TRY_ERR (iRf69WriteReg (rf, REG_RSSICONFIG, RF_RSSI_START));
    do {
      // wait for RSSI_Ready
      TRY_ERR (b = iRf69ReadReg (rf, REG_RSSICONFIG));
    }
    while ( (b & RF_RSSI_DONE) == 0);
  }

  TRY_ERR (b = iRf69ReadReg (rf, REG_RSSIVALUE));
  return - (b / 2);
}

// -----------------------------------------------------------------------------
int iRf69SetPromiscuous (xRf69 * rf, bool bOn) {

  if (bOn != rf->promiscuous) {
    TRY_INT();
    uint8_t b;

    TRY_ERR (b = iRf69ReadReg (rf, REG_PACKETCONFIG1));
    b = (b & 0xF9) | (bOn ? RF_PACKET1_ADRSFILTERING_OFF : RF_PACKET1_ADRSFILTERING_NODEBROADCAST);

    TRY_ERR (iRf69WriteReg (rf, REG_PACKETCONFIG1, b));
    rf->promiscuous = bOn;
  }
  return 0;
}

// -----------------------------------------------------------------------------
bool
bRf69isPromiscuous (const xRf69 * rf) {

  return rf->promiscuous;
}

// -----------------------------------------------------------------------------
bool
bRf69isHighPower (const xRf69 * rf) {

  return rf->is_rfm69hw;
}

// -----------------------------------------------------------------------------
int
iRf69Sleep (xRf69 * rf) {

  return iRf69SetMode (rf, eRf69ModeSleep);
}

// -----------------------------------------------------------------------------
int
iRf69RcCalibration (const xRf69 * rf) {
  TRY_INT();
  uint8_t b;

  TRY_ERR (iRf69WriteReg (rf, REG_OSC1, RF_OSC1_RCCAL_START));
  do {
    // wait for RCCAL_DONE
    TRY_ERR (b = iRf69ReadReg (rf, REG_OSC1));
  }
  while ( (b & RF_OSC1_RCCAL_DONE) == 0);
  return 0;
}

/* protected functions ====================================================== */

// -----------------------------------------------------------------------------
int
iRf69WaitIrq (const xRf69 * rf, int timeout) {
  TRY_INT();
  int ret;
  timer_t t;

  TRY_ERR (t = xRf69TimerNew ());
  vRf69TimerStart (t, timeout);
  while ( (bRf69ReadIrqPin (rf) == false) && ! bRf69Timeout (t))
    ;

  ret = bRf69Timeout (t) ? -1 : 0;
  vRf69TimerDelete (t);
  return ret;
}

// -----------------------------------------------------------------------------
int
iRf69WaitForReady (const xRf69 * rf, int timeout) {
  TRY_INT();
  uint8_t b;
  int ret;
  timer_t t;

  TRY_ERR (t = xRf69TimerNew ());
  vRf69TimerStart (t, timeout);
  do {

    TRY_ERR (b = iRf69ReadReg (rf, REG_IRQFLAGS1));
  }
  while ( ( (b & RF_IRQFLAGS1_MODEREADY) == 0) && ! bRf69Timeout (t));

  ret = bRf69Timeout (t) ? -1 : 0;
  vRf69TimerDelete (t);
  return ret;
}

// -----------------------------------------------------------------------------
int
iRf69WaitToSend (xRf69 * rf, int timeout) {
  TRY_INT();
  bool bCanSend;
  int ret;
  timer_t t;

  TRY_ERR (t = xRf69TimerNew ());
  vRf69TimerStart (t, timeout);
  TRY_ERR (iRf69AvoidRxDeadLocks (rf));

  do {

    TRY_ERR (bCanSend = iRf69CanSend (rf));
    if (! bCanSend) {

      TRY_ERR (iRf69ReceiveDone (rf));
    }
  }
  while ( (bCanSend == false) && ! bRf69Timeout (t));

  ret = bRf69Timeout (t) ? -1 : 0;
  vRf69TimerDelete (t);
  return ret;
}

// -----------------------------------------------------------------------------
int
iRf69WriteRegWithCheck (const xRf69 * rf, uint8_t reg, uint8_t data, int timeout) {
  TRY_INT();
  uint8_t b;
  int ret;
  timer_t t;

  TRY_ERR (t = xRf69TimerNew ());
  vRf69TimerStart (t, timeout);
  do {
    TRY_ERR (iRf69WriteReg (rf, reg, data));
    TRY_ERR (b = iRf69ReadReg (rf, reg));
  }
  while ( (b != (int) data) && ! bRf69Timeout (t));

  ret = bRf69Timeout (t) ? -1 : 0;
  vRf69TimerDelete (t);
  return ret;
}

// -----------------------------------------------------------------------------
// Utilisé sous interruption
int
iRf69SetHighPowerRegs (const xRf69 * rf, bool bOn) {

  TRY_ERR (iRf69WriteReg (rf, REG_TESTPA1, bOn ? 0x5D : 0x55));
  return iRf69WriteReg (rf, REG_TESTPA2, bOn ? 0x7C : 0x70);
}

// -----------------------------------------------------------------------------
// Utilisé sous interruption
int
iRf69SetMode (xRf69 * rf, eRf69Mode eNewMode) {

  if (eNewMode != rf->mode) {
    TRY_INT();
    uint8_t b;

    TRY_ERR (b = iRf69ReadReg (rf, REG_OPMODE));
    b &= 0xE3;

    switch (eNewMode) {

      case eRf69ModeTx:
        TRY_ERR (iRf69WriteReg (rf, REG_OPMODE, b | RF_OPMODE_TRANSMITTER));
        if (rf->is_rfm69hw) {

          TRY_ERR (iRf69SetHighPowerRegs (rf, true));
        }
        break;

      case eRf69ModeRx:
        TRY_ERR (iRf69WriteReg (rf, REG_OPMODE, b | RF_OPMODE_RECEIVER));
        if (rf->is_rfm69hw) {

          TRY_ERR (iRf69SetHighPowerRegs (rf, false));
        }
        break;

      case eRf69ModeSynth:
        TRY_ERR (iRf69WriteReg (rf, REG_OPMODE, b | RF_OPMODE_SYNTHESIZER));
        break;

      case eRf69ModeStandby:
        TRY_ERR (iRf69WriteReg (rf, REG_OPMODE, b | RF_OPMODE_STANDBY));
        break;

      case eRf69ModeSleep:
        TRY_ERR (iRf69WriteReg (rf, REG_OPMODE, b | RF_OPMODE_SLEEP));
        break;

      default:
        return -1;
    }

    /* we are using packet mode, so this check is not really needed
     * but waiting for mode ready is necessary when going from sleep
     * because the FIFO may not be immediately available from previous mode
     */
    if (rf->mode == eRf69ModeSleep) {

      TRY_ERR (iRf69WaitForReady (rf, 50));
    }

    rf->mode = eNewMode;
  }
  return 0;
}

// -----------------------------------------------------------------------------
// Utilisé sous interruption
int
iRf69AvoidRxDeadLocks (xRf69 * rf) {
  TRY_INT();
  uint8_t b;

  // avoid RX deadlocks
  TRY_ERR (b = iRf69ReadReg (rf, REG_PACKETCONFIG2));
  b = (b & 0xFB) | RF_PACKET2_RXRESTART;
  TRY_ERR (iRf69WriteReg (rf, REG_PACKETCONFIG2, b));
  return 0;
}

// -----------------------------------------------------------------------------
// Utilisé sous interruption
int
iRf69StartReceiving (xRf69 * rf) {
  TRY_INT();

  rf->data_len = 0;
  rf->hdr.sender = 0;
  rf->hdr.dest = 0;
  rf->hdr.payload_len = 0;
  rf->hdr.ctl = 0;
  rf->rssi = 0;

  TRY_ERR (iRf69AvoidRxDeadLocks (rf));

  // set DIO0 to "PAYLOADREADY" in receive mode
  TRY_ERR (iRf69WriteReg (rf, REG_DIOMAPPING1, RF_DIOMAPPING1_DIO0_01));
  return iRf69SetMode (rf, eRf69ModeRx);
}

// -----------------------------------------------------------------------------
int
iRf69SendFrame (xRf69 * rf, uint8_t toAddress,
                const void * tx_buffer, uint8_t tx_len,
                bool bRequestACK, bool bSendAck) {
  TRY_INT();

  // turn off receiver to prevent reception while filling fifo
  TRY_ERR (iRf69SetMode (rf, eRf69ModeStandby));

  // wait for ModeReady
  TRY_ERR (iRf69WaitForReady (rf, 50));

  // DIO0 is "Packet Sent"
  TRY_ERR (iRf69WriteReg (rf, REG_DIOMAPPING1, RF_DIOMAPPING1_DIO0_00));

  tx_len = MIN(tx_len, RF69_MAX_DATA_LEN);
  rf->data[0] = tx_len + RF69_HEADER_SIZE - 1;
  rf->data[1] = toAddress;
  rf->data[2] = rf->node_id;
  rf->data[3] = bSendAck ? RF69_ACK : RF69_ACKREQ;
  memcpy (&rf->data[4], tx_buffer, tx_len);

  TRY_ERR (iRf69WriteBlock (rf, REG_FIFO, rf->data, RF69_HEADER_SIZE + tx_len));

  // no need to wait for transmit mode to be ready since its handled by the radio
  TRY_ERR (iRf69SetMode (rf, eRf69ModeTx));

  // wait for DIO0 to turn HIGH signaling transmission finish
  TRY_ERR (iRf69WaitIrq (rf, RF69_TX_TIMEOUT_MS));

  return iRf69SetMode (rf, eRf69ModeStandby);
}

// -----------------------------------------------------------------------------
void
vRf69Isr (xRf69 * rf) {

  if (rf->mode == eRf69ModeRx)  {
    TRY_INT();
    uint8_t irqflags;

    TRY_VOID (irqflags = iRf69ReadReg (rf, REG_IRQFLAGS2));

    if (irqflags & RF_IRQFLAGS2_PAYLOADREADY) {

      TRY_VOID (iRf69SetMode (rf, eRf69ModeStandby));

      if (irqflags & RF_IRQFLAGS2_FIFOLEVEL) {

        TRY_VOID (iRf69ReadBlock (rf, REG_FIFO,
                                  (uint8_t *) &rf->hdr, sizeof (rf->hdr)));
        if (rf->promiscuous || rf->hdr.dest == rf->node_id ||
            rf->hdr.dest == RF69_BROADCAST_ADDR) {

          rf->data_len = MIN (rf->hdr.payload_len, RF69_FIFO_SIZE) - 3;
          TRY_VOID (iRf69ReadBlock (rf, REG_FIFO, rf->data, rf->data_len));
          if (rf->data_len < RF69_MAX_DATA_LEN) {
            rf->data[rf->data_len] = 0;  // add null at end of string
          }

          TRY_VOID (iRf69SetMode (rf, eRf69ModeRx));
          TRY_VOID (rf->rssi = iRf69Rssi (rf, false));
          return;
        }
      }

      rf->hdr.payload_len = 0;
      iRf69StartReceiving (rf); // fifo flush
    }
  }
}

/* ========================================================================== */
