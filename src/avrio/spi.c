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
 * @file spi.c
 * @brief Implémentation bus SPI
 *
 * Revision History ------------------------------------------------------------
 *    20111002 - Initial version by epsilonRT
 */
#include "avrio-config.h"

#ifdef AVRIO_SPI_ENABLE
/* ========================================================================== */
# include "avrio-board-spi.h"
# include <avrio/spi.h>

/* internal public functions ================================================ */

// -----------------------------------------------------------------------------
void
vSpiMasterInit (eSpiFclkDiv eFclkDiv) {

  vSpiDisable ();
  vSpiBoardInitMaster();
  SPSR = (uint8_t) (eFclkDiv >> 2);
  SPCR = _BV (MSTR) | ( (_BV (SPR1) | _BV (SPR0)) & eFclkDiv);
  vSpiEnable ();
}

// -----------------------------------------------------------------------------
void
vSpiSetMode (eSpiMode eMode) {
  if ( (eMode >= eSpiMode0) && (eMode <= eSpiMode3)) {
    uint8_t cr = SPCR;

    SPCR = 0;
    cr = (cr & ~ (_BV (CPOL) | _BV (CPHA))) | (eMode  << CPHA);
    SPCR = cr;
    vSpiClearIF ();
  }
}

// -----------------------------------------------------------------------------
void
vSpiSetBitOrder (eSpiBitOrder eOrder) {

  if ( (eOrder >= eSpiBitOrderMsb) && (eOrder <= eSpiBitOrderLsb)) {
    uint8_t cr = SPCR;

    SPCR = 0;
    cr = (cr & ~_BV (DORD)) | (eOrder << DORD);
    SPCR = cr;
    vSpiClearIF ();
  }
}

// -----------------------------------------------------------------------------
uint8_t
ucSpiMasterWriteRead (uint8_t tx_byte) {

  SPDR = tx_byte;
  while ( (SPSR & _BV (SPIF)) == 0)
    ;
  return SPDR;
}

// -----------------------------------------------------------------------------
void
vSpiMasterXfer (const uint8_t * tx_buffer, uint8_t tx_len, uint8_t *rx_buffer, uint8_t rx_len) {
  uint8_t b;

  vSpiSetSs();
  while ( (tx_len) || (rx_len)) {

    if ( (tx_len) && (tx_buffer)) {

      b = *tx_buffer++;
      tx_len--;
    }
    else {

      b = 0;
    }

    SPDR = b;
    while ( (SPSR & _BV (SPIF)) == 0)
      ;

    if ( (rx_len) && (rx_buffer)) {

      *rx_buffer++ = SPDR;
      rx_len--;
    }
  }
  vSpiClearSs();
}

// -----------------------------------------------------------------------------
uint8_t
ucSpiMasterReadReg8 (uint8_t reg) {
  uint8_t data;
  
  vSpiSetSs();
  vSpiMasterWrite (reg & ~_BV (SPI_REG_RW_BIT));
  data = ucSpiMasterRead();
  vSpiClearSs();
  return data;
}

// -----------------------------------------------------------------------------
void
vSpiMasterWriteReg8 (uint8_t reg, uint8_t data) {

  vSpiSetSs();
  vSpiMasterWrite (reg | _BV (SPI_REG_RW_BIT));
  vSpiMasterWrite (data);
  vSpiClearSs();
}

// -----------------------------------------------------------------------------
void
vSpiMasterWriteRegBlock (uint8_t reg, const uint8_t * tx_buffer, uint8_t tx_len) {

  vSpiSetSs();
  vSpiMasterWrite (reg | _BV (SPI_REG_RW_BIT));
  vSpiMasterXfer (tx_buffer, tx_len, NULL, 0);
  vSpiClearSs();
}

// -----------------------------------------------------------------------------
void 
vSpiMasterReadRegBlock (uint8_t reg, uint8_t * rx_buffer, uint8_t rx_len) {
  
  vSpiSetSs();
  vSpiMasterWrite (reg & ~_BV (SPI_REG_RW_BIT));
  vSpiMasterXfer (NULL, 0, rx_buffer, rx_len);
  vSpiClearSs();
}

#endif /* AVRIO_SPI_ENABLE defined */
/* ========================================================================== */
