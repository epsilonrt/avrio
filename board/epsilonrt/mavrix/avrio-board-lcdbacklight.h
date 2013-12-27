/* Copyright © 2009-2012 epsilonRT. All rights reserved.
 *  $Id$ */
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

  DDRB |= _BV (7);  /* PB7 (OC2) en sortie */
  /* 
   * Mode PWM Phase correcte (1), F = FCPU / (8 * 510) = 3.6 KHz pour 14.7456 MHz
   * Sortie inversée
   */
  TCCR2 = 0b01100010;
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
