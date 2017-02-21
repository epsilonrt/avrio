/**
 * @file
 * @brief
 *
 * Copyright © 2017 epsilonRT, All rights reserved.
 * This software is governed by the CeCILL license <http://www.cecill.info>
 */
#include <avrio/irq.h>
#include <avrio/spi.h>
#include "avrio-board-rf69.h"
#include "rf69_private.h"

/* private variables ======================================================== */
static xRf69 xAvrRf69;

/* private functions ======================================================== */
// -----------------------------------------------------------------------------
static void
vAvrIsr (xIrqHandle i) {

  vRf69Isr (&xAvrRf69);
}

/* protected functions ====================================================== */

// -----------------------------------------------------------------------------
// Utilisé sous interruption
int
iRf69WriteReg (xRf69 * rf, uint8_t reg, uint8_t data) {

  ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {

    vSpiMasterWriteReg8 (reg, data);
  }
  return 0;
}

// -----------------------------------------------------------------------------
// Utilisé sous interruption
int
iRf69ReadReg (xRf69 * rf, uint8_t reg) {
  int ret;

  ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {

    ret = (unsigned) ucSpiMasterReadReg8 (reg);
  }
  return ret;
}

// -----------------------------------------------------------------------------
int
iRf69WriteBlock (xRf69 * rf, uint8_t reg, const uint8_t * buf, uint8_t len) {

  ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {

    vSpiMasterWriteRegBlock (reg, buf, len);
  }
  return 0;
}

// -----------------------------------------------------------------------------
// Utilisé sous interruption
int 
iRf69ReadBlock (xRf69 * rf, uint8_t reg, uint8_t * buf, uint8_t len) {
  
  ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {

    vSpiMasterReadRegBlock (reg, buf, len);
  }
  return 0;
}

// -----------------------------------------------------------------------------
int
iRf69WriteConstElmt (xRf69 * rf, const struct xRf69Config * elmt) {
  struct xRf69Config config;
  memcpy_P (&config, elmt, sizeof (config));

  if (config.reg != 255) {

    (void) iRf69WriteReg (rf, config.reg, config.data);
    return true;
  }

  return false;
}

// -----------------------------------------------------------------------------
timer_t
xRf69TimerNew (void) {

  return  xTaskCreate (0, NULL);
}

// -----------------------------------------------------------------------------
void
vRf69TimerStart (timer_t t, int timeout) {

  vTaskSetInterval (t, xTaskConvertTicks (timeout));
  vTaskStart (t);
}

// -----------------------------------------------------------------------------
void
vRf69TimerStop (timer_t t) {
  vTaskStop (t);
}

// -----------------------------------------------------------------------------
void
vRf69TimerDelete (timer_t t) {

  vTaskDelete (t);
}

// -----------------------------------------------------------------------------
bool
bRf69Timeout (timer_t t) {

  return ! xTaskIsStarted (t);
}

// -----------------------------------------------------------------------------
bool
bRf69ReadIrqPin (xRf69 * rf) {

  return bIrqReadPin (rf->irqpin);
}

/* internal public functions ================================================ */
// -----------------------------------------------------------------------------
xRf69 *
xRf69New (int iBus, int iDevice, int iIrqPin) {

  vSpiSetSsAsOutput();
  // decided to slow down from DIV2 after SPI stalling in some instances,
  // especially visible on mega1284p when RFM69 and FLASH chip both present
  vSpiMasterInit (RF69_SPI_DIV);
  vSpiSetMode (eSpiMode0);
  vSpiSetBitOrder (eSpiBitOrderMsb);

  xAvrRf69.irqpin = iIrqPin;
  vIrqAttach (iIrqPin, vAvrIsr, eIrqRising);

  return &xAvrRf69;
}

// -----------------------------------------------------------------------------
void
vRf69Delete (xRf69 * rf) {

  // Rien à faire, pour compatibilité
}

// -----------------------------------------------------------------------------
// checks if a packet was received and/or puts transceiver in receive (ie RX or listen) mode
int
iRf69ReceiveDone (xRf69 * rf) {
  TRY_INIT();
  bool bReceiveDone = false;

  ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {

    if (rf->mode == eRf69ModeRx) {

      if (rf->hdr.payload_len > 0) {

        TRY (iRf69SetMode (rf, eRf69ModeStandby));
        bReceiveDone = true;
      }
    }
    else {

      TRY (iRf69StartReceiving (rf));
    }
  }
  return bReceiveDone;
}

/* ========================================================================== */
