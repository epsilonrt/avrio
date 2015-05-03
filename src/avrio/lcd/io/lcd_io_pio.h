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
 * @file lcd_io_pio.h
 * @brief Interface LCD PIO
 *
 *
 * Revision History ------------------------------------------------------------
 *    20120501 - Initial version by epsilonRT
 */
#ifndef _AVRIO_LCD_IO_PIO_H_
#define _AVRIO_LCD_IO_PIO_H_

#include <avrio/defs.h>
#include <avrio/delay.h>

/* *INDENT-OFF* */
__BEGIN_C_DECLS
/* ========================================================================== */
#ifndef __DOXYGEN__

/* macros =================================================================== */
# ifdef LCD_PIO_RW_PORT
#   define RW_INIT() do { LCD_PIO_RW_DDR  |=  _BV(LCD_PIO_RW_BIT); } while(0)
#   define RW_CLR()  do { LCD_PIO_RW_PORT &= ~_BV(LCD_PIO_RW_BIT); } while(0)
#   define RW_SET()  do { LCD_PIO_RW_PORT |=  _BV(LCD_PIO_RW_BIT); } while(0)
# else
#   define RW_INIT()
#   define RW_CLR()
#   define RW_SET()
# endif

# ifdef LCD_PIO_RD_PORT
#   define RD_INIT() do { LCD_PIO_RD_DDR  |=  _BV(LCD_PIO_RD_BIT); } while(0)
#   define RD_CLR()  do { LCD_PIO_RD_PORT &= ~_BV(LCD_PIO_RD_BIT); } while(0)
#   define RD_SET()  do { LCD_PIO_RD_PORT |=  _BV(LCD_PIO_RD_BIT); } while(0)
# else
#   define RD_INIT()
#   define RD_CLR()
#   define RD_SET()
# endif

/* private functions ======================================================== */

/* -----------------------------------------------------------------------------
 * Génére une impulsion sur Enable
 */
__STATIC_ALWAYS_INLINE (void
vEnablePulse (void)) {

  LCD_PIO_E_PORT |=  _BV(LCD_PIO_E_BIT);  /* E=1 */
  delay_us (LCD_PIO_E_PW);
  LCD_PIO_E_PORT &= ~_BV(LCD_PIO_E_BIT);  /* E=0 */
}

#endif /* __DOXYGEN__ not defined */
/* ========================================================================== */
__END_C_DECLS
/* *INDENT-ON* */
#endif  /* _AVRIO_LCD_IO_PIO_H_ not defined */
