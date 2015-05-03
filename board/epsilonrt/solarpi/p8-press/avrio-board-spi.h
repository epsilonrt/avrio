/*
 * @file avrio-board-wusb.h
 * @brief Partie matérielle SPI
 * @author Copyright © 2011-2012 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * Revision History ------------------------------------------------------------
 *    20111002 - Initial version by epsilonRT
 */
#ifndef _AVRIO_BOARD_SPI_H_
#define _AVRIO_BOARD_SPI_H_
#include <avrio/defs.h>
#include <avr/io.h>

/* constants ================================================================ */
#define SPI_SS_BIT       2
#define SPI_MOSI_BIT     3
#define SPI_MISO_BIT     4
#define SPI_SCK_BIT      5
#define SPI_DDR          DDRB
#define SPI_PORT         PORTB


/* internal public functions ================================================ */
// -----------------------------------------------------------------------------
INLINE void vSpiBoardInitMaster (void) {

  SPI_PORT &= ~(_BV (SPI_SCK_BIT) | _BV (SPI_MOSI_BIT));
  SPI_DDR  |= _BV (SPI_SCK_BIT) | _BV (SPI_MOSI_BIT);
  SPI_PORT |= _BV (SPI_MISO_BIT);
  SPI_DDR  &= ~_BV (SPI_MISO_BIT);
}

// -----------------------------------------------------------------------------
INLINE void vSpiBoardSetSsAsInput (void) {

  SPI_PORT |=  _BV (SPI_SS_BIT); /* Validation pull-up sur SS */
  SPI_DDR  &= ~_BV (SPI_SS_BIT);
}

// -----------------------------------------------------------------------------
INLINE void vSpiBoardSetSsAsOutput (void) {

  SPI_PORT |= _BV (SPI_SS_BIT); /* SS = 1 */
  SPI_DDR  |= _BV (SPI_SS_BIT);
}

// -----------------------------------------------------------------------------
INLINE void vSpiSetSs (void) {

  SPI_PORT &= ~_BV (SPI_SS_BIT); /* SS = 0 -> validé */
}

// -----------------------------------------------------------------------------
INLINE void vSpiClearSs (void) {

  SPI_PORT |= _BV (SPI_SS_BIT); /* SS = 1 -> invalidé */
}

/* ========================================================================== */
#endif  /* _AVRIO_BOARD_SPI_H_ not defined */
