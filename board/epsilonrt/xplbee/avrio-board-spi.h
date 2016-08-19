/*
 * Copyright © 2016 epsilonRT, All rights reserved.
 *
 * This software is governed by the CeCILL license under French law and
 * abiding by the rules of distribution of free software.  You can  use,
 * modify and/ or redistribute the software under the terms of the CeCILL
 * license as circulated by CEA, CNRS and INRIA at the following URL
 * <http://www.cecill.info>.
 *
 * The fact that you are presently reading this means that you have had
 * knowledge of the CeCILL license and that you accept its terms.
 */
#ifndef _AVRIO_BOARD_SPI_H_
#define _AVRIO_BOARD_SPI_H_
#include <avrio/defs.h>
#include <avrio/delay.h>
#include <avr/io.h>

/* constants ================================================================ */
#define SPI_SS_BIT       4
#define SPI_SCK_BIT      7
#define SPI_MOSI_BIT     5
#define SPI_MISO_BIT     6
#define SPI_DDR          DDRB
#define SPI_PORT         PORTB

#define SPI_VCC_BIT      3
#define SPI_VCC_DDR      DDRB
#define SPI_VCC_PORT     PORTB

/* internal public functions ================================================ */
// -----------------------------------------------------------------------------
INLINE void 
vSpiBoardInitMaster (void) {

  SPI_PORT &= ~(_BV (SPI_SCK_BIT) | _BV (SPI_MOSI_BIT));
  SPI_DDR  |= _BV (SPI_SCK_BIT) | _BV (SPI_MOSI_BIT);
  SPI_PORT |= _BV (SPI_MISO_BIT);
  SPI_DDR  &= ~_BV (SPI_MISO_BIT);
  SPI_VCC_PORT &= ~_BV (SPI_VCC_BIT);
  SPI_VCC_DDR  |=  _BV (SPI_VCC_BIT);
}


// -----------------------------------------------------------------------------
INLINE void 
vSpiBoardSetSsAsInput (void) {

  SPI_PORT |=  _BV (SPI_SS_BIT); /* Validation pull-up sur SS */
  SPI_DDR  &= ~_BV (SPI_SS_BIT);
}

// -----------------------------------------------------------------------------
INLINE void 
vSpiBoardSetSsAsOutput (void) {

  SPI_PORT |= _BV (SPI_SS_BIT); /* SS = 1 */
  SPI_DDR  |= _BV (SPI_SS_BIT);
}

// -----------------------------------------------------------------------------
INLINE void 
vSpiSetSs (void) {

  SPI_VCC_PORT  |= _BV (SPI_VCC_BIT);
  delay_ms(10);  // délai de démarrage max HSC 7.3 ms
  SPI_PORT &= ~_BV (SPI_SS_BIT); /* SS = 0 -> validé */
}

// -----------------------------------------------------------------------------
INLINE void 
vSpiClearSs (void) {

  SPI_PORT |= _BV (SPI_SS_BIT); /* SS = 1 -> invalidé */
  delay_ms(1);
  SPI_VCC_PORT &= ~_BV (SPI_VCC_BIT);
}

/* ========================================================================== */
#endif  /* _AVRIO_BOARD_SPI_H_ not defined */
