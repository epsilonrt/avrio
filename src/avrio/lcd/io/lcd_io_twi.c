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
 * @file lcd_io_twi.c
 * @brief Modèle d'interface d'afficheur LCD
 *
 * Liste des contrôleurs compatibles:
 * - PCF2119
 * - ST7032
 * .
 *
 *
 * Revision History ------------------------------------------------------------
 *    20120501 - Initial version by epsilonRT
 */
#include "avrio-config.h"

#ifdef AVRIO_LCD_ENABLE
#include "avrio-board-lcd.h"

#if LCD_IO == LCD_IO_TWI
#include <avrio/twi.h>
#include "lcd_io.h"

/* constants ================================================================ */
/* Position des bits lors de l'envoi de l'octet de contrôle sur I2C */
#define TWI_RS_BIT 6
#define TWI_CO_BIT 7

/* macros =================================================================== */
/* Formatage de l'octet de contrôle IĠc */
#define TWI_CTRL(__co, __rs) (((__co)<<TWI_CO_BIT) | ((__rs)<<TWI_RS_BIT))

/* private variables ======================================================== */
static eTwiStatus eLastTwiError;
static uint8_t pucLcdTwiBuffer[2];
static xTwiFrame xLcdTwiFrame = { .xAddr  = LCD_TWI_DEVICE_ADDR, 
                                  .pxData = pucLcdTwiBuffer,
                                  .xLen   = 2 };

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
#ifndef LCD_IO_RST_DELAY
/* ------ RESET non défini ------ */
# define prvvReset()
#else
# define prvvReset() delay_ms(LCD_IO_RST_DELAY)
#endif /* LCD_IO_RST_DELAY defined */
#endif /* LCD_IO_RST_BIT defined */

/* internal public functions ================================================ */

// -----------------------------------------------------------------------------
int8_t 
iLcdIoInit (void) {

  prvvReset();
#  ifdef LCD_TWI_MASTER_INIT
  vTwiInit ();
  eTwiSetSpeed (LCD_TWI_MASTER_INIT);
#  endif
  vLcdIoWrite (0, 0); /* Ecrit NOP pour vérifier la présence sur le bus I2C */
  return (int8_t)eLastTwiError;
}

// -----------------------------------------------------------------------------
void 
vLcdIoWrite (uint8_t ucData, uint8_t ucAddress) {
  
  pucLcdTwiBuffer[0] = TWI_CTRL(0, ucAddress);
  pucLcdTwiBuffer[1] = ucData;
  eLastTwiError = eTwiSend (&xLcdTwiFrame);
}

// -----------------------------------------------------------------------------
#ifdef LCD_IO_READ_ENABLE
# error "Lecture non prise en charge en I2C"
#endif /* LCD_IO_READ_ENABLE */

/* ========================================================================== */
#endif  /* LCD_IO == LCD_IO_TWI */
#endif /* AVRIO_LCD_ENABLE defined */
