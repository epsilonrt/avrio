/**
 * @file lcd.c
 * @brief Afficheur LCD alphanumérique
 * @author epsilonRT. Copyright © 2012 All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>

 */
#include "avrio-config.h"

#ifdef AVRIO_LCD_ENABLE
/* ========================================================================== */
#include "avrio-board-lcd.h"

#if defined(LCD_BARGRAPH_ENABLE) && !defined(LCD_CTRL_ICON_ENABLE)
# warning "LCD_CTRL_ICON_ENABLE was defined as LCD_BARGRAPH_ENABLE is defined" 
# define LCD_CTRL_ICON_ENABLE
#endif

#include "lcd.h"
#include <avrio/delay.h>

/* internal public functions ================================================ */
// -----------------------------------------------------------------------------
int8_t
iLcdInit (void) {
  int8_t iError;
  
  iError = iLcdCtrlInit ();
  if (iError == 0) {

#ifdef LCD_DEFAULT_CONTRAST  
    ucLcdCtrlContrastSet (LCD_DEFAULT_CONTRAST);
#endif
    vLcdCtrlClear();
  }
  return iError;
}

// -----------------------------------------------------------------------------
void
vLcdPutString (const char *pcString) {

  while (*pcString) {

    vLcdPutChar (*pcString++);
  }
}

// -----------------------------------------------------------------------------
void
vLcdClearChars (xLcdCoord ucX, xLcdCoord ucY, xLcdCoord uLen) {

  vLcdGotoXY (ucX, ucY);
  while (uLen--) {

    vLcdPutChar (' ');
  }
  vLcdGotoXY (ucX, ucY);
}

// -----------------------------------------------------------------------------
void
vLcdGotoXY (xLcdCoord colonne, xLcdCoord ligne) {

  vLcdCtrlSetX (MIN (colonne, LCD_WIDTH - 1));
  vLcdCtrlSetY (MIN (ligne, LCD_HEIGHT - 1));
}

// -----------------------------------------------------------------------------
void
vLcdPutChar (char cChar) {

  switch (cChar) {

    case '\n': /* Retour chariot + Saut de ligne */
      vLcdGotoXY (0, (xLcdCtrlGetY() + 1) % LCD_HEIGHT);
      break;

    case '\r': /* Retour chariot */
      vLcdGotoXY (0, xLcdCtrlGetY());
      break;

    default:
      /* Autre caractère */
#ifndef AVRIO_LCD_NOAUTO_RETURN
      if ((xLcdCtrlGetX() >= LCD_WIDTH) && (xLcdCtrlGetY() < (LCD_HEIGHT - 1))) {
        /* A la fin de la ligne, passe à la suivante si possible */
        vLcdGotoXY (0, xLcdCtrlGetY() + 1);
      }
#endif
      if ((xLcdCtrlGetX() < LCD_WIDTH) && (xLcdCtrlGetY() < LCD_HEIGHT)) {

        vLcdCtrlPutChar (cChar);
      }
      break;
  }
}

#ifdef LCD_BARGRAPH_ENABLE
/* ========================================================================== */

/* constants ================================================================ */
#define BAR_PATTERN_WIDTH (LCD_PATTERN_WIDTH + 1)

/* private variables ======================================================== */
static const uint8_t * pucLcdBarPatterns;
static xLcdPos xLcdBarHome;

/* internal public functions ================================================ */
// -----------------------------------------------------------------------------
void
vLcdBargraphInit (const uint8_t * pucBarPatterns) {
  uint8_t ucIndex;

  pucLcdBarPatterns = pucBarPatterns;
  // Mémorise la position de départ
  xLcdBarHome.xX = xLcdCtrlGetX();
  xLcdBarHome.xY = xLcdCtrlGetY();
  
  /* Initialise les patterns de bargraphe */
  for (ucIndex = 0; ucIndex < BAR_PATTERN_WIDTH; ucIndex++) {

    vLcdCtrlSetIcon (ucIndex, &pucLcdBarPatterns[ucIndex << 3]);
  }
}

// -----------------------------------------------------------------------------
void
vLcdBargraph (uint16_t usValue, uint16_t usMax, uint8_t ucLen) {
  uint8_t ucX, ucCode;
  uint32_t ulPixelsWidth;


  // Replace le curseur à la position de départ
  vLcdGotoXY (xLcdBarHome.xX, xLcdBarHome.xY);

  ulPixelsWidth = ((usValue * (ucLen * BAR_PATTERN_WIDTH)) / usMax);
  for (ucX = 0; (ucX < ucLen); ucX++) {

    if (((ucX * (uint16_t) BAR_PATTERN_WIDTH) + 5) > ulPixelsWidth) {

      if (((ucX * (uint16_t) BAR_PATTERN_WIDTH)) > ulPixelsWidth) {
      
        ucCode = 0;  /* caractère vide */
      }
      else {
      
        ucCode = ulPixelsWidth % BAR_PATTERN_WIDTH;
      }
    } 
    else {
    
      ucCode = BAR_PATTERN_WIDTH - 1;
    }

    vLcdCtrlPutChar (ucCode);
  }
}
/* ========================================================================== */
#endif  /* LCD_BARGRAPH_ENABLE */


/* avr-libc stdio interface ================================================= */
static int
prvPutChar (char cChar, FILE * pxStream) {

  vLcdPutChar (cChar);
  return 0;
}

FILE xLcd = FDEV_SETUP_STREAM (prvPutChar, NULL, _FDEV_SETUP_WRITE);

#endif /* AVRIO_LCD_ENABLE defined */
/* ========================================================================== */
