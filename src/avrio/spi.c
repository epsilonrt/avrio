/**
 * @file spi.c
 * @brief Implémentation bus SPI
 * @author Copyright © 2011-2012 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * @version $Id$
 * Revision History ---
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
