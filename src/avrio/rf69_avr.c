/**
 * @file
 * @brief Module de transmission UHF RFM69 (Implémentation ATMEL AVR)
 * 
 * http://www.hoperf.com/rf_transceiver/modules/RFM69W.html
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

/* internal protected functions ============================================= */

// -----------------------------------------------------------------------------
// Utilisé sous interruption
int
iRf69WriteReg (const xRf69 * rf, uint8_t reg, uint8_t data) {

  ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {

    vSpiMasterWriteReg8 (reg, data);
  }
  return 1;
}

// -----------------------------------------------------------------------------
// Utilisé sous interruption
int
iRf69ReadReg (const xRf69 * rf, uint8_t reg) {
  int ret;

  ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {

    ret = (unsigned) ucSpiMasterReadReg8 (reg);
  }
  return ret;
}

// -----------------------------------------------------------------------------
int
iRf69WriteBlock (const xRf69 * rf, uint8_t reg, const uint8_t * buf, uint8_t len) {

  ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {

    vSpiMasterWriteRegBlock (reg, buf, len);
  }
  return len;
}

// -----------------------------------------------------------------------------
// Utilisé sous interruption
int
iRf69ReadBlock (const xRf69 * rf, uint8_t reg, uint8_t * buf, uint8_t len) {

  ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {

    vSpiMasterReadRegBlock (reg, buf, len);
  }
  return len;
}

// -----------------------------------------------------------------------------
int
iRf69WriteConstElmt (const xRf69 * rf, const struct xRf69Config * elmt) {
  struct xRf69Config config;
  memcpy_P (&config, elmt, sizeof (config));

  if (config.reg != 255) {

    return iRf69WriteReg (rf, config.reg, config.data);
  }

  return 0;
}

// -----------------------------------------------------------------------------
xRf69Timer
xRf69TimerNew (void) {

  return  xTaskCreate (0, NULL);
}

// -----------------------------------------------------------------------------
void
vRf69TimerStart (xRf69Timer t, int timeout) {

  vTaskSetInterval (t, xTaskConvertTicks (timeout));
  vTaskStart (t);
}

// -----------------------------------------------------------------------------
void
vRf69TimerStop (xRf69Timer t) {

  vTaskStop (t);
}

// -----------------------------------------------------------------------------
void
vRf69TimerDelete (xRf69Timer t) {

  vTaskDelete (t);
}

// -----------------------------------------------------------------------------
bool
bRf69TimerTimeout (xRf69Timer t) {

  return ! xTaskIsStarted (t);
}

// -----------------------------------------------------------------------------
int
iRf69ReadIrqPin (const xRf69 * rf) {

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
  int ret = false;

  ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {

    if (rf->mode == eRf69ModeRx) {

      if (rf->hdr.payload_len > 0) {

        ret = iRf69SetMode (rf, eRf69ModeStandby);
        if (ret == 0) {

          ret = true;;
        }
      }
    }
    else {

      ret = iRf69StartReceiving (rf);
      if (ret == 0) {

        ret = false;
      }
    }
  }
  return ret;
}

/* ========================================================================== */
