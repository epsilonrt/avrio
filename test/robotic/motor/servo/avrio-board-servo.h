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
#ifndef _AVRIO_BOARD_SERVO_H_
#define _AVRIO_BOARD_SERVO_H_
/* ========================================================================== */
#include <avrio/defs.h>
#include <avr/io.h>

/* constants ================================================================ */
#define SERVO_PERIOD_US 18000UL
#define SERVO_CHANNELS 2

/* private variables ======================================================== */

/* inline public functions ================================================== */
/*
 * Initialise la ressource matérielle (timer) utilisée par le module BDCM
 * A modifier en fonction du timer et de la sortie utilisée.
 * @param  Fréquence en Hertz du signal PWM
 * @return Valeur TOP correspondant à la fréquence
 */
static inline uint16_t
usServoTimerInit (void) {

  /* Broche OC1A et OC1B en sortie */
  DDRB |= _BV(2) | _BV(1);
  
  /*
   * TIMER1 -> Mode 8 PWM Phase et fréquence correcte, TOP ICR1
   * F_CPU: 8 MHz
   * N = 8 
   */
  uint16_t usTop = (SERVO_PERIOD_US * F_CPU) / (2 * 8 * 1E6);
  TCCR1B = 0;
  TCCR1A = 0xA0; /* Clear OC1A/OC1B on Compare Match */
  TCNT1 = 0;
  ICR1 = usTop;
  TCCR1B = 0x12;  /* N = 8 */

  return ICR1;
}

/*
 * Réglage
 * @param ucChannel Canal du servo
 * @param usSetting Réglage  (min. = 0 pour r = 0 / max. = TOP pour r = 1)
 */
static inline void
vServoTimerSet (uint8_t ucChannel, uint16_t usSetting) {

  switch (ucChannel) {

    case 0:
      OCR1A = usSetting;
      break;
    case 1:
      OCR1B = usSetting;
      break;
  }
}

/*
 * Lecture réglage servo
 * @param ucChannel Canal du servo
 */
static inline uint16_t
usServoTimerGet (uint8_t ucChannel) {

  switch (ucChannel) {

    case 0:
      return OCR1A;
      break;
    case 1:
      return OCR1B;
      break;
  }
  return -1;
}

/* ========================================================================== */
#endif /* _AVRIO_BOARD_SERVO_H_ */
