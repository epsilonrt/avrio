/**
 * @file lcd_io_pio_4.c
 * @brief Interface d'afficheur LCD utilisant un port IO en 4 bits
 * @author epsilonRT. Copyright © 2012 All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * Revision History ------------------------------------------------------------
 *    20120501 - Initial version by epsilonRT
 */
#include "avrio-config.h"

#ifdef AVRIO_LCD_ENABLE
#include "avrio-board-lcd.h"

#if LCD_IO == LCD_IO_PIO_4
/* ========================================================================== */
#include <avrio/util.h>
#include "lcd_io.h"
#include "lcd_io_pio.h"

/* private functions ======================================================== */
#ifdef LCD_IO_RST_BIT
#include <avrio/delay.h>
/* ------ RESET défini ------ */

static inline void
prvvReset (void) {
  LCD_IO_RST_DDR |= _BV(LCD_IO_RST_BIT);
# if LCD_IO_RST_LEVEL == 0
  /* RESET actif à l'état bas */
  LCD_IO_RST_PORT &= ~_BV(LCD_IO_RST_BIT);
  delay_ms (LCD_IO_RST_PW);
  LCD_IO_RST_PORT |= _BV(LCD_IO_RST_BIT);
  delay_ms (LCD_IO_RST_DELAY);
# else
  /* RESET actif à l'état haut */
  LCD_IO_RST_PORT |= _BV(LCD_IO_RST_BIT);
  delay_ms (LCD_IO_RST_PW);
  LCD_IO_RST_PORT &= ~_BV(LCD_IO_RST_BIT);
  delay_ms (LCD_IO_RST_DELAY);
# endif /* LCD_IO_RST_LEVEL == 0 */ 
}

#else
/* ------ RESET non défini ------ */
# define prvvReset()
#endif /* LCD_IO_RST_BIT defined */

/* internal public functions ================================================ */

// -----------------------------------------------------------------------------
int8_t 
iLcdIoInit (void) {

  /* E, RS, RW en sorties */
  LCD_PIO_E_DDR  |= _BV(LCD_PIO_E_BIT);
  LCD_PIO_RS_DDR |= _BV(LCD_PIO_RS_BIT);
  RW_INIT ();
  RD_INIT ();

  /* Data en sorties */
  LCD_PIO_DATA_DDR |= 0x0F << LCD_PIO_DATA_DB4;

  /* E, RS, WR à l'état bas */
  LCD_PIO_E_PORT  &= ~_BV(LCD_PIO_E_BIT);
  LCD_PIO_RS_PORT &= ~_BV(LCD_PIO_RS_BIT);
  RW_CLR ();
  RD_SET ();

  prvvReset();
  return 0;
}

// -----------------------------------------------------------------------------
// Nécessaire si contrôleur LCD avec mode 4 bits (qu'il soit utilisé en 4 ou en 8)
void
vLcdIoWriteNibble (uint8_t ucHighNibble, uint8_t ucAddress) {

  if (ucAddress != 0)
    LCD_PIO_RS_PORT |= _BV(LCD_PIO_RS_BIT);  /* RS=1, donnée */
  else
    LCD_PIO_RS_PORT &= ~_BV(LCD_PIO_RS_BIT);  /* RS=0, instruction */

#ifdef LCD_PIO_DATA_REVERSE
  ucHighNibble = ucUtilReverseBits (ucHighNibble) << 4;
#endif
  LCD_PIO_DATA_PORT = (LCD_PIO_DATA_PORT & ~(0x0F << LCD_PIO_DATA_DB4)) |
     ((ucHighNibble >> 4) << (LCD_PIO_DATA_DB4));
  vEnablePulse();
}

// -----------------------------------------------------------------------------
void 
vLcdIoWrite (uint8_t ucData, uint8_t ucAddress) {

  /* Ecriture de l'octet en 2 fois */
  vLcdIoWriteNibble (ucData, ucAddress);  /* quartet poids fort */
  vLcdIoWriteNibble (ucData << 4, ucAddress); /* quartet poids faible */
}

// -----------------------------------------------------------------------------
#ifdef LCD_IO_READ_ENABLE
uint8_t 
ucLcdRead (uint8_t ucAddress) {
  
  // TODO
}
#endif /* LCD_IO_READ_ENABLE */

/* ========================================================================== */
#endif  /* LCD_IO == LCD_IO_PIO_4 */
#endif /* AVRIO_LCD_ENABLE defined */
