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
#ifndef _AVRIO_BOARD_BUTTON_H_
#  define _AVRIO_BOARD_BUTTON_H_
/* ========================================================================== */

/* BUTTON==================================================================== */
#  include <avrio/defs.h>
#  include <avr/io.h>

/* constants ================================================================ */
#  define BUTTON_QUANTITY  16
#  define BUTTON_BUTTON1  _BV(0)
#  define BUTTON_BUTTON2  _BV(1)
#  define BUTTON_BUTTON3  _BV(2)
#  define BUTTON_BUTTON4  _BV(3)
#  define BUTTON_BUTTON5  _BV(4)
#  define BUTTON_BUTTON6  _BV(5)
#  define BUTTON_BUTTON7  _BV(6)
#  define BUTTON_BUTTON8  _BV(7)
#  define BUTTON_BUTTON9  _BV(8)
#  define BUTTON_BUTTON10 _BV(9)
#  define BUTTON_BUTTON11 _BV(10)
#  define BUTTON_BUTTON12 _BV(11)
#  define BUTTON_BUTTON13 _BV(12)
#  define BUTTON_BUTTON14 _BV(13)
#  define BUTTON_BUTTON15 _BV(14)
#  define BUTTON_BUTTON16 _BV(15)
#  define BUTTON_ALL_BUTTONS (0xFFFF)
#  define BUTTON_NO_BUTTON (0)

/* Configuration Port IO */
#  define BUTTON_PORT PORTC
#  define BUTTON_DDR  DDRC
#  define BUTTON_PIN  PINC
/* Colonnes: broches en entrée */
#  define BUTTON_C1 0
#  define BUTTON_C2 1
#  define BUTTON_C3 2
#  define BUTTON_C4 3
/* Lignes: broches en sortie */
#  define BUTTON_R1 4
#  define BUTTON_R2 5
#  define BUTTON_R3 6
#  define BUTTON_R4 7

/* types ==================================================================== */
typedef uint16_t xButMask;

/* ========================================================================== */
#endif /* _AVRIO_BOARD_BUTTON_H_ */
