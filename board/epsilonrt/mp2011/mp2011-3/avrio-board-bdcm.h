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
#ifndef _AVRIO_BOARD_BDCM_H_
#  define _AVRIO_BOARD_BDCM_H_
/* ========================================================================== */

/* DC Motor ================================================================= */
#  include <avr/io.h>
#  include <avrio/bdcm.h>

/* constants ================================================================ */
#  define BDCM_MAX 2
                   /*< Nombre de moteurs */

/*
 * Signaux utilisés pour la commande du L298
 */
#  define EN_DDR  DDRD
#  define EN_PORT PORTD
#  define ENA 5
#  define ENB 4

#  define IN_DDR  DDRB
#  define IN_PORT PORTB
#  define IN1 0
#  define IN2 1
#  define IN3 2
#  define IN4 3

/* inline public functions ================================================== */
/*
 * Initialise la ressource matérielle (timer) utilisée par le module BDCM
 * A modifier en fonction du timer et de la sortie utilisée.
 * @param  Fréquence en Hertz du signal PWM
 * @return Valeur TOP correspondant à la fréquence
 */
static inline uint16_t
usBdcmHarwareInit (uint16_t usFreq) {

  // ENA-ENB du L298
  EN_PORT &= ~(_BV (ENA) | _BV (ENB));
  EN_DDR |= (_BV (ENA) | _BV (ENB));
  // IN1-IN4 du L298
  IN_PORT &= ~(_BV (IN1) | _BV (IN2) | _BV (IN3) | _BV (IN4));
  IN_DDR |= (_BV (IN1) | _BV (IN2) | _BV (IN3) | _BV (IN4));
  // TIMER1 -> Mode 8 PWM Phase et fréquence correcte
  TCCR1B = 0;
  TCCR1A = 0xA0;
  TCNT1 = 0;
  ICR1 = AVRIO_CPU_FREQ / (2 * usFreq);
  TCCR1B = 0x11;  /* N = 1 */

  return ICR1;
}

/*
 * Modifie la commande (roue libre, avant, arrière, stop) d'un moteur
 * @param ucControl Commande à effectuer
 * @param ucMotor Numéro du moteur
 */
static inline void
vBdcmHardwareControl (uint8_t ucControl, uint8_t ucMotor) {

  switch (ucMotor) {

    case 0:
      switch (ucControl) {

        case BDCM_FREE:
          IN_PORT &= ~(_BV (IN1) | _BV (IN2));
          break;
        case BDCM_FORWARD:
          IN_PORT |= _BV (IN1);
          IN_PORT &= ~_BV (IN2);
          break;
        case BDCM_REVERSE:
          IN_PORT |= _BV (IN2);
          IN_PORT &= ~_BV (IN1);
          break;
        case BDCM_STOP:
          IN_PORT |= (_BV (IN1) | _BV (IN2));
          break;
      }
      break;

    case 1:
      switch (ucControl) {

        case BDCM_FREE:
          IN_PORT &= ~(_BV (IN3) | _BV (IN4));
          break;
        case BDCM_FORWARD:
          IN_PORT |= _BV (IN3);
          IN_PORT &= ~_BV (IN4);
          break;
        case BDCM_REVERSE:
          IN_PORT |= _BV (IN4);
          IN_PORT &= ~_BV (IN3);
          break;
        case BDCM_STOP:
          IN_PORT |= (_BV (IN3) | _BV (IN4));
          break;
      }
      break;
  }
}

/*
 * Modifie le rapport cyclique du signal PWM d'un moteur
 * @param usSetting Réglage PWM (min. = 0 pour r = 0 / max. = TOP pour r = 1)
 * @param ucMotor Numéro du moteur
 */
static inline void
vBdcmHardwareSet (uint16_t usSetting, uint8_t ucMotor) {

  switch (ucMotor) {

    case 0:
      OCR1A = usSetting;
      break;
    case 1:
      OCR1B = usSetting;
      break;
  }
}

/* ========================================================================== */
#endif /* _AVRIO_BOARD_BDCM_H_ */
