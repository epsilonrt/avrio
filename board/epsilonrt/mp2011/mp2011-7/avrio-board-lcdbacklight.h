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
#ifndef _AVRIO_BOARD_LCDBACKLIGHT_H_
#  define _AVRIO_BOARD_LCDBACKLIGHT_H_
/* ========================================================================== */

/* LCD_BACKLIGHT ============================================================ */
#  include <avrio/defs.h>
#  include <avr/io.h>

/* inline public functions ================================================== */
/*
 * Initialise la ressource matérielle (timer PWM de préférence) utilisée par
 * le module lcd (rétroéclairage).
 * A modifier en fonction du timer et de la sortie utilisée.
 */
static inline void
vLcdDriverBacklightInit (void) {

  DDRD |= _BV (7);  /* PD7 (OC2) en sortie */
  /* 
   * Mode PWM Phase correcte (1), F = FCPU / (8 * 510) = 3.9 KHz pour 16 MHz
   * Sortie inversée
   */
  TCCR2 = 0x62;
}

/*
 * Modifie le niveau de rétroéclairage du LCD
 * @param ucValue niveau entre 0 et 63
 */
static inline void
vLcdDriverBacklightSet (uint8_t ucValue) {
  uint16_t usOcr = ucValue << 2; // x4

  OCR2 = usOcr;
}

/* ========================================================================== */
#endif /* _AVRIO_BOARD_LCDBACKLIGHT_H_ */
