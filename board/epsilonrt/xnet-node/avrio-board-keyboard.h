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
#ifndef _AVRIO_BOARD_KEYBOARD_H_
#  define _AVRIO_BOARD_KEYBOARD_H_
/* ========================================================================== */

/* KEYBOARD ================================================================= */
#  include <avrio/defs.h>
#  include <avr/io.h>

/* constants ================================================================ */

/* Adresse I²C du THMI */
#define KEYB_THMI_ADDR THMI_BASE

/* Taille du buffer de réception */
#define KEYB_THMI_BUFFERSIZE  16

/* Broche reliée au signal HIRQ (à mettre en commentaire si non utilisé) */
#define KEYB_THMI_HIRQ 2

/* Registre PORT de la broche reliée au signal HIRQ */
#define KEYB_THMI_HIRQ_PORT PORTD

/* Registre DDR de la broche reliée au signal HIRQ */
#define KEYB_THMI_HIRQ_DDR  DDRD

/* Registre PIN de la broche reliée au signal HIRQ */
#define KEYB_THMI_HIRQ_PIN  PIND

/* Vecteur d'interruption */
#define KEYB_THMI_HIRQ_vect INT0_vect

/* inline public functions ================================================== */
/* Valide l'interruption */
static inline void
vKeybIrqEnable (void) {

  EIFR  |= _BV(INTF0);
  EIMSK |= _BV(INT0);
}

/* ========================================================================== */
#endif /* _AVRIO_BOARD_KEYBOARD_H_ */
