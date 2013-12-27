/**
 * @file lcd_io_pio.h
 * @brief Interface LCD PIO
 * @author epsilonRT. Copyright © 2012 All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * @version $Id$
 * Revision History ---
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
