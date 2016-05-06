/**
 * Copyright © 2011-2015 epsilonRT. All rights reserved.
 *
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
#endif  /* _AVRIO_LCD_IO_PIO_H_ not defined */
