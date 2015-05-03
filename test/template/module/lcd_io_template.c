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
 * @file lcd_io_template.c
 * @brief Modèle d'interface d'afficheur LCD
 *
 *
 * Revision History ------------------------------------------------------------
 *    20120501 - Initial version by epsilonRT
 */
#include "avrio-config.h"

#ifdef AVRIO_LCD_ENABLE
#include "avrio-board-lcd.h"

#if LCD_IO == LCD_IO_TEMPLATE
/* macros =================================================================== */
/* constants ================================================================ */
/* structures =============================================================== */
/* types ==================================================================== */
/* private variables ======================================================== */
/* private functions ======================================================== */
/* public variables ========================================================= */
/* internal public functions ================================================ */

// -----------------------------------------------------------------------------
void 
vLcdIoInit (void) {

}

// -----------------------------------------------------------------------------
// Nécessaire si contrôleur LCD avec mode 4 bits (qu'il soit utilisé en 4 ou en 8)
void
vLcdIoWriteNibble (uint8_t ucHighNibble, uint8_t ucAddress) {

}

// -----------------------------------------------------------------------------
void 
vLcdIoWrite (uint8_t ucData, uint8_t ucAddress) {

}

// -----------------------------------------------------------------------------
#ifdef LCD_IO_READ_ENABLE
uint8_t 
ucLcdRead (uint8_t ucAddress) {

}
#endif /* LCD_IO_READ_ENABLE */

/* ========================================================================== */
#endif  /* LCD_IO == LCD_IO_TEMPLATE */
#endif /* AVRIO_LCD_ENABLE defined */
