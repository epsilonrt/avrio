/**
 * @file lcd_io_twi_pcf8574.c
 * @brief Interface d'afficheur LCD utilisant un PCF8574 (4 bits uniquement)
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

#if LCD_IO == LCD_TWI_PCF8574
/* ========================================================================== */
#include <avrio/twi.h>
#include "lcd_io.h"

/* macros =================================================================== */
# ifdef LCD_PCF8574_RW_PORT
#   define RW_CLR()  do { ucLcdIoPort &= ~_BV(LCD_PCF8574_RW_BIT); } while(0)
#   define RW_SET()  do { ucLcdIoPort |=  _BV(LCD_PCF8574_RW_BIT); } while(0)
# else
#   define RW_CLR()
#   define RW_SET()
# endif

/* private variables ======================================================== */
static uint8_t ucLcdIoPort;
static eTwiStatus eLastTwiError;

/* internal private functions =============================================== */
static void
prvvPortWrite (void) {

  eLastTwiError = eTwiWrite (LCD_TWI_DEVICE_ADDR, ucLcdIoPort);
}

/* 
 * Génére une impulsion sur Enable
 */
static inline void
prvvEnablePulse (void) {

  ucLcdIoPort |= _BV(LCD_PCF8574_E_BIT);  /* E=1 */
  prvvPortWrite ();
  ucLcdIoPort &= ~_BV(LCD_PCF8574_E_BIT);  /* E=0 */
  prvvPortWrite ();
}

/* internal public functions ================================================ */

// -----------------------------------------------------------------------------
int8_t 
iLcdIoInit (void) {

#  ifdef LCD_TWI_MASTER_INIT
  vTwiInit ();
  eTwiSetSpeed (LCD_TWI_MASTER_INIT);
#  endif
  ucLcdIoPort = 0;
  RW_CLR();
  prvvPortWrite ();
  return (int8_t)eLastTwiError;
}

// -----------------------------------------------------------------------------
// Nécessaire si contrôleur LCD avec mode 4 bits (qu'il soit utilisé en 4 ou en 8)
void
vLcdIoWriteNibble (uint8_t ucHighNibble, uint8_t ucAddress) {

  if (ucAddress != 0)
    ucLcdIoPort |= _BV(LCD_PCF8574_RS_BIT);  /* RS=1, donnée */
  else
    ucLcdIoPort &= ~_BV(LCD_PCF8574_RS_BIT);  /* RS=0, instruction */

#ifdef LCD_PCF8574_DATA_REVERSE
  ucHighNibble = ucUtilReverseBits (ucHighNibble) << 4;
#endif
  ucLcdIoPort = (ucLcdIoPort & ~(0x0F << LCD_PCF8574_DATA_DB4)) |
     ((ucHighNibble >> 4) << (LCD_PCF8574_DATA_DB4));

  prvvEnablePulse ();
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
# error "Lecture non prise en charge en I2C"
#endif /* LCD_IO_READ_ENABLE */

/* ========================================================================== */
#endif  /* LCD_IO == LCD_TWI_PCF8574 */
#endif /* AVRIO_LCD_ENABLE defined */
