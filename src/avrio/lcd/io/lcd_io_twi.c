/**
 * @file lcd_io_twi.c
 * @brief Modèle d'interface d'afficheur LCD
 *
 * Liste des contrôleurs compatibles:
 * - PCF2119
 * - ST7032
 * .
 * @author epsilonRT. Copyright © 2012 All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * @version $Id$
 * Revision History ---
 *    20120501 - Initial version by epsilonRT
 */
#include "avrio-config.h"

#ifdef AVRIO_LCD_ENABLE
#include "avrio-board-lcd.h"

#if LCD_IO == LCD_IO_TWI
#include <avrio/twi.h>
#include "lcd_io.h"

/* constants ================================================================ */
/* Position des bits lors de l'envoi de l'octet de contrôle sur IĠC */
#define TWI_RS_BIT 6
#define TWI_CO_BIT 7

/* macros =================================================================== */
/* Formatage de l'octet de contrôle IĠc */
#define TWI_CTRL(__co, __rs) (((__co)<<TWI_CO_BIT) | ((__rs)<<TWI_RS_BIT))

/* private variables ======================================================== */
static eTwiStatus eLastTwiError;
static uint8_t pucLcdTwiBuffer[2];
static xTwiFrame xLcdTwiFrame = { .xAddr  = LCD_TWI_DEVICE_ADDR, 
                                  .pxData = pucLcdTwiBuffer,
                                  .xLen   = 2 };

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

  prvvReset();
#  ifdef LCD_TWI_MASTER_INIT
  vTwiInit ();
  eTwiSetSpeed (LCD_TWI_MASTER_INIT);
#  endif
  vLcdIoWrite (0, 0); /* Ecrit NOP pour vérifier la présence sur le bus IĠC */
  return (int8_t)eLastTwiError;
}

// -----------------------------------------------------------------------------
void 
vLcdIoWrite (uint8_t ucData, uint8_t ucAddress) {
  
  pucLcdTwiBuffer[0] = TWI_CTRL(0, ucAddress);
  pucLcdTwiBuffer[1] = ucData;
  eLastTwiError = eTwiSend (&xLcdTwiFrame);
}

// -----------------------------------------------------------------------------
#ifdef LCD_IO_READ_ENABLE
# error "Lecture non prise en charge en I2C"
#endif /* LCD_IO_READ_ENABLE */

/* ========================================================================== */
#endif  /* LCD_IO == LCD_IO_TWI */
#endif /* AVRIO_LCD_ENABLE defined */
