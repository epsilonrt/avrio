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
#ifndef _AVRIO_BOARD_GLCD_H_
#  define _AVRIO_BOARD_GLCD_H_
/* ========================================================================== */

/* GLCD ===================================================================== */
#  include <avrio/defs.h>
#  include <avr/io.h>

/* constants ================================================================ */
#  define GLCD_IFACE_PIO_4BITS 1
#  define GLCD_IFACE_PIO_8BITS 2
#  define GLCD_IFACE_BUS       3
#  define LCD_CPU_KS0108       1

#  define GLCD_CPU LCD_CPU_KS0108

#  define GLCD_IFACE GLCD_IFACE_PIO_8BITS

#  define GLCD_XPIXELS    128
#  define GLCD_YPIXELS    64

#  define GLCD_PIO_DATA_PORT  PORTB
#  define GLCD_PIO_DATA_DDR   DDRB
#  define GLCD_IO_DATA_PIN   PINB

#  define GLCD_PIO_RS_PORT    PORTD
#  define GLCD_PIO_RS_DDR     DDRD
#  define GLCD_PIO_RS_BIT     6

#  define GLCD_PIO_RW_PORT    PORTD
#  define GLCD_PIO_RW_DDR     DDRD
#  define GLCD_PIO_RW_BIT     4

#  define GLCD_PIO_E_PORT     PORTC
#  define GLCD_PIO_E_DDR      DDRC
#  define GLCD_PIO_E_BIT      6

#  define GLCD_CS0_PORT   PORTC
#  define GLCD_CS0_DDR    DDRC
#  define GLCD_CS0_BIT    0

#  define GLCD_CS1_PORT   PORTC
#  define GLCD_CS1_DDR    DDRC
#  define GLCD_CS1_BIT    1

#  define GLCD_RESET_PORT PORTC
#  define GLCD_RESET_DDR  DDRC
#  define GLCD_RESET_BIT  7

/* ========================================================================== */
#endif /* _AVRIO_BOARD_GLCD_H_ */
