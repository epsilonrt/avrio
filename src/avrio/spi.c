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
  SPSR = (uint8_t)(eFclkDiv >> 2);
  SPCR = _BV(MSTR) | ((_BV(SPR1) | _BV(SPR0)) & eFclkDiv);
  vSpiEnable ();
}

// -----------------------------------------------------------------------------
uint8_t 
ucSpiMasterWriteRead (uint8_t ucByte) {

  SPDR = ucByte;
  while ((SPSR & _BV (SPIF)) == 0) 
    ;
  return SPDR;
}

#endif /* AVRIO_SPI_ENABLE defined */
/* ========================================================================== */
