/**
 * @file thmi.c
 * @brief tiny HMI
 * @author Copyright _ 2011-2012 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * @version $Id$
 */
#include <avrio/keyb.h>
#include <avrio/lcd.h>
#include "avrio-config.h"

#if defined(AVRIO_KEYB_ENABLE) && defined(AVRIO_LCD_ENABLE)
#include "avrio-board-lcd.h"
#include "avrio-board-keyboard.h"

#if LCD_CTRL == LCD_CTRL_THMI && defined(KEYB_THMI_BUFFERSIZE)
#include <avrio/lcd/ctrl/lcd_ctrl_thmi.h>

/* internal public functions ================================================ */
// -----------------------------------------------------------------------------
uint16_t
usThmiVersion (void) {
  uint16_t usVersion = 0;
  
  if (usKeybHit() == 0) {
    char c;
    
    vLcdCtrlPutExtCmd (THMI_FVER);
    c = cKeybGetChar();
    usVersion  = c << 8; // Major
    c = cKeybGetChar();
    usVersion |= c; // Minor
  }
  
  return usVersion;
}
#endif /* LCD_CTRL == LCD_CTRL_THMI && defined(KEYB_THMI_BUFFERSIZE) */
#endif /* AVRIO_KEYB_ENABLE defined */
/* ========================================================================== */
