/*
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
