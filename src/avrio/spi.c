/**
 * Copyright © 2011-2015 epsilonRT. All rights reserved.
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
