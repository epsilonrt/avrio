/**
 * Copyright © 2011-2015 Pascal JEAN aka epsilonRT. All rights reserved.
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
 * @file lcd_io_pio_8.c
 * @brief Interface d'afficheur LCD utilisant un port IO en 8 bits
 *
 *
 * Revision History ------------------------------------------------------------
 *    20120501 - Initial version by epsilonRT
 */
#include "avrio-config.h"

#ifdef AVRIO_LCD_ENABLE
#include "avrio-board-lcd.h"

#if LCD_IO == LCD_IO_PIO_8
/* ========================================================================== */
#include <avrio/util.h>
#include "lcd_io.h"
#include "lcd_io_pio.h"

/* private functions ======================================================== */
#ifdef LCD_IO_RST_BIT
#include <avrio/delay.h>
/* ------ RESET défini ------ */

static inline void
prvvReset (void) {
  LCD_IO_RST_DDR |= _BV(LCD_IO_RST_BIT);
# if LCD_IO_RST_LEVEL == 0
  /* RESET actif à l'état bas */
  LCD_IO_RST_PORT &= ~_BV(LCD_IO_RST_BIT);
  delay_ms (LCD_IO_RST_PW);
  LCD_IO_RST_PORT |= _BV(LCD_IO_RST_BIT);
  delay_ms (LCD_IO_RST_DELAY);
# else
  /* RESET actif à l'état haut */
  LCD_IO_RST_PORT |= _BV(LCD_IO_RST_BIT);
  delay_ms (LCD_IO_RST_PW);
  LCD_IO_RST_PORT &= ~_BV(LCD_IO_RST_BIT);
  delay_ms (LCD_IO_RST_DELAY);
# endif /* LCD_IO_RST_LEVEL == 0 */ 
}

#else
/* ------ RESET non défini ------ */
# define prvvReset()
#endif /* LCD_IO_RST_BIT defined */

/* internal public functions ================================================ */

// -----------------------------------------------------------------------------
int8_t 
iLcdIoInit (void) {

  /* E, RS, RW en sorties */
  LCD_PIO_E_DDR  |= _BV(LCD_PIO_E_BIT);
  LCD_PIO_RS_DDR |= _BV(LCD_PIO_RS_BIT);
  RW_INIT ();
  RD_INIT ();

  /* Data en sorties */
  LCD_PIO_DATA_DDR |= 0xFF;

  /* E, RS, WR à l'état bas */
  LCD_PIO_E_PORT  &= ~_BV(LCD_PIO_E_BIT);
  LCD_PIO_RS_PORT &= ~_BV(LCD_PIO_RS_BIT);
  RW_CLR ();
  RD_SET ();

  prvvReset();
  return 0;
}

// -----------------------------------------------------------------------------
// Nécessaire si contrôleur LCD avec mode 4 bits (qu'il soit utilisé en 4 ou en 8)
void
vLcdIoWriteNibble (uint8_t ucHighNibble, uint8_t ucAddress) {

  vLcdIoWrite (ucHighNibble, ucAddress);
}

// -----------------------------------------------------------------------------
void 
vLcdIoWrite (uint8_t ucData, uint8_t ucAddress) {

  if (ucAddress != 0)
    LCD_PIO_RS_PORT |= _BV(LCD_PIO_RS_BIT);  /* RS=1, donnée */
  else
    LCD_PIO_RS_PORT &= ~_BV(LCD_PIO_RS_BIT);  /* RS=0, instruction */

#ifdef LCD_PIO_DATA_REVERSE
  ucData = ucUtilReverseBits (ucData);
#endif
  LCD_PIO_DATA_PORT = ucData;
  vEnablePulse();
}

// -----------------------------------------------------------------------------
#ifdef LCD_IO_READ_ENABLE
uint8_t 
ucLcdRead (uint8_t ucAddress) {
  
  // TODO
}
#endif /* LCD_IO_READ_ENABLE */

/* ========================================================================== */
#endif  /* LCD_IO == LCD_IO_PIO_8 */
#endif /* AVRIO_LCD_ENABLE defined */
