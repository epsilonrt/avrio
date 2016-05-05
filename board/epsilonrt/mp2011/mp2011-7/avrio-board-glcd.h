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
