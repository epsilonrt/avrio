/*
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
 */
#ifndef _AVRIO_BOARD_SPI_H_
#define _AVRIO_BOARD_SPI_H_
#include <avrio/defs.h>
#include <avr/io.h>

/* constants ================================================================ */
#define SPI_SS_BIT       4
#define SPI_MOSI_BIT     5
#define SPI_MISO_BIT     6
#define SPI_SCK_BIT      7
#define SPI_DDR          DDRB
#define SPI_PORT         PORTB

/* internal public functions ================================================ */
// -----------------------------------------------------------------------------
__STATIC_ALWAYS_INLINE(void vSpiBoardInitMaster (void)) {

  SPI_PORT &= ~(_BV (SPI_SCK_BIT) | _BV (SPI_MOSI_BIT));
  SPI_DDR  |= _BV (SPI_SCK_BIT) | _BV (SPI_MOSI_BIT);
  SPI_PORT |= _BV (SPI_MISO_BIT);
  SPI_DDR  &= ~_BV (SPI_MISO_BIT);
}

// -----------------------------------------------------------------------------
__STATIC_ALWAYS_INLINE(void vSpiBoardSetSsAsInput (void)) {

  SPI_PORT |=  _BV (SPI_SS_BIT); /* Validation pull-up sur SS */
  SPI_DDR  &= ~_BV (SPI_SS_BIT);
}

// -----------------------------------------------------------------------------
__STATIC_ALWAYS_INLINE(void vSpiBoardSetSsAsOutput (void)) {

  SPI_PORT |= _BV (SPI_SS_BIT); /* SS = 1 */
  SPI_DDR  |= _BV (SPI_SS_BIT);
}

/* ========================================================================== */
#endif  /* _AVRIO_BOARD_SPI_H_ not defined */
