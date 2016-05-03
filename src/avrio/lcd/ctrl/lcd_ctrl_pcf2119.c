/**
 * Copyright © 2011-2015 epsilonRT. All rights reserved.
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
 * @file lcd_ctrl_pcf2119.c
 * @brief Contrôleur LCD PCF2119
 *
 *
 * Revision History ------------------------------------------------------------
 *    20120501 - Initial version by epsilonRT
 */
#include "avrio-config.h"

#ifdef AVRIO_LCD_ENABLE
#include "avrio-board-lcd.h"

#if LCD_CTRL == LCD_CTRL_PCF2119
/* ========================================================================== */
#include <avrio/delay.h>
#include <avrio/lcd/io/lcd_io.h>
#include "lcd_ctrl.h"
#include "lcd_ctrl_pcf2119.h"

/* constants ================================================================ */
/* Temps d'exécution de l'instruction Clear Display en ms */
#define CLR_DELAY 2
/* Délai d'exécution en ṁs */
#define EXEC_DELAY 30

#if LCD_CTRL_PCF2119_CHAR_SET == 'A' || LCD_CTRL_PCF2119_CHAR_SET == 'D'
# define CHAR_OFFSET 0
#else
# define CHAR_OFFSET 0x80
#endif

/* private variables ======================================================== */
static xLcdCoord xLcdCtrlX, xLcdCtrlY; /* Position du curseur */
static uint8_t ucLcdCtrlCursorAddr; /* Adresse DDRAM du curseur */
static uint8_t ucFsetCmd; /* Octet Function Set à l'init (pour accès mode étendu */

/* private functions ======================================================== */

// -----------------------------------------------------------------------------
static void 
prvvWrite (uint8_t ucData, uint8_t ucAddress) {

  vLcdIoWrite (ucData, ucAddress);

  /* Délai d'exécution des commandes */
  if (ucAddress == PCF2119_IR) {
  
    if ((ucData & 0xE0) == PCF2119_FSET) {
      
      /* Mémorise le function set */
      ucFsetCmd = ucData;
    }
      
    if (ucFsetCmd & PCF2119_H) {
      /* Mode étendu */
    
    }
    else {
      /* Mode HD44780 */
    
      if (ucData == PCF2119_CLR)
        delay_ms (CLR_DELAY);
    }
  }
  delay_us (EXEC_DELAY);
}

// -----------------------------------------------------------------------------
static inline void
prvvSetPosition (void) {

  vLcdCtrlPutCmd (PCF2119_DMEM | ucLcdCtrlCursorAddr);
}

// -----------------------------------------------------------------------------
static void
prvvUpdatePosition (void) {

  ucLcdCtrlCursorAddr = xLcdCtrlX + 0x40 * xLcdCtrlY;
  prvvSetPosition();
}

/* internal public functions ================================================ */
// -----------------------------------------------------------------------------
int8_t
iLcdCtrlInit (void) {
  int8_t iError;
  
  iError = iLcdIoInit (); /* Effectue aussi un RESET */
  
  if (iError == 0) {
  
    vLcdCtrlPutCmd (LCD_CTRL_PCF2119_INIT_FSET | PCF2119_DL);  /* function set */
    vLcdCtrlPutCmd (LCD_CTRL_PCF2119_INIT_DCTL);  /* display control */
    vLcdCtrlPutCmd (LCD_CTRL_PCF2119_INIT_ENTM);  /* entry mode set */
    vLcdCtrlBacklightInit ();
  }
  
  return iError;
}

// -----------------------------------------------------------------------------
uint8_t 
ucLcdCtrlContrastSet (uint8_t ucContrast) {

  ucContrast &= 0x1F;
  vLcdCtrlPutCmd (ucFsetCmd | PCF2119_H); // passe en mode étendu
  vLcdCtrlPutCmd (PCF2119_SETVA (ucContrast + LCD_CTRL_PCF2119_VA_MIN));
  vLcdCtrlPutCmd (ucFsetCmd & ~PCF2119_H); // repasse en mode standard
  return ucContrast;
}

// -----------------------------------------------------------------------------
void
vLcdCtrlPutChar (char cChar) {

#if CHAR_OFFSET != 0
  if (cChar >= ' ')
    cChar += CHAR_OFFSET;
#endif
  prvvWrite (cChar, PCF2119_DR);
  xLcdCtrlX++;
}

// -----------------------------------------------------------------------------
void
vLcdCtrlPutCmd (char cCmd) {

  prvvWrite (cCmd, PCF2119_IR);
}

// -----------------------------------------------------------------------------
void
vLcdCtrlClear (void) {

#if LCD_CTRL_PCF2119_CHAR_SET == 'R'
  /* Le caractère 0x20 n'est pas un espace ! */
  uint8_t ucCount;
  
  vLcdCtrlPutCmd (PCF2119_HOME);
  for (ucCount = 0; ucCount < 80; ucCount++) {
  
    vLcdCtrlPutChar (' ');
  }
#else
  vLcdCtrlPutCmd (PCF2119_CLR);
#endif /* LCD_CTRL_PCF2119_CHAR_SET == 'R' */
  xLcdCtrlY = xLcdCtrlX = 0;
}

// -----------------------------------------------------------------------------
void 
vLcdCtrlSetX (xLcdCoord xX) {

  xLcdCtrlX = xX;
  prvvUpdatePosition ();
}

// -----------------------------------------------------------------------------
void 
vLcdCtrlSetY (xLcdCoord xY) {

  xLcdCtrlY = xY;
  prvvUpdatePosition ();
}

// -----------------------------------------------------------------------------
xLcdCoord 
xLcdCtrlGetX (void) {

  return xLcdCtrlX;
}

// -----------------------------------------------------------------------------
xLcdCoord 
xLcdCtrlGetY (void) {

  return xLcdCtrlY;
}

// -----------------------------------------------------------------------------
void
vLcdCtrlEnableCursor (eLcdCursor eCursor) {

  vLcdCtrlPutCmd (PCF2119_DCTL | PCF2119_D | (uint8_t)eCursor);
}

#if defined(LCD_CTRL_ICON_ENABLE)
#include <avr/pgmspace.h>
// -----------------------------------------------------------------------------
void
vLcdCtrlSetIcon (uint8_t ucCode, const uint8_t * pucIconPattern) {
  uint8_t ucOffset;

  /* L'affectation du bit 6 de l'adresse CGRAM se fait par une instruction
   * Set DDRAM Address ! (Voir paragraphe 8.7 datasheet PCF2119, p. 29) */
  if (ucCode < 8) {
    
    // Bit 6 de l'adresse à 0
    vLcdCtrlPutCmd  (PCF2119_DMEM | 0);
  }
  else {
  
    // Bit 6 de l'adresse à 1
    vLcdCtrlPutCmd  (PCF2119_DMEM | _BV(6));
  }
  ucCode *= LCD_PATTERN_HEIGHT; /* Calcul de l'adresse en CGRAM */
  vLcdCtrlPutCmd  (PCF2119_GMEM | (ucCode & 0x3F));

  for (ucOffset = 0; ucOffset < LCD_PATTERN_HEIGHT; ucOffset++) {

    prvvWrite (pgm_read_byte (pucIconPattern + ucOffset), PCF2119_DR);
  }
  prvvSetPosition (); // Ré-initialise la position du curseur
}
#endif /* LCD_CTRL_ICON_ENABLE */

#ifdef LCD_CTRL_BACKLIGHT_ENABLE
/* ========================================================================== */
#include "avrio-board-lcdbacklight.h"

/* internal public functions ================================================ */

// -----------------------------------------------------------------------------
void 
vLcdCtrlBacklightInit (void) {

  vLcdDriverBacklightInit ();
}

// -----------------------------------------------------------------------------
uint8_t 
ucLcdCtrlBacklightSet (uint8_t ucLevel) {

  ucLevel &= 0x3F;
  vLcdDriverBacklightSet (ucLevel);
  return ucLevel;
}

#else /* LCD_CTRL_BACKLIGHT_ENABLE not defined */
// -----------------------------------------------------------------------------
void 
vLcdCtrlBacklightInit (void) {

}
/* ========================================================================== */
#endif  /* LCD_CTRL_BACKLIGHT_ENABLE defined */

/* ========================================================================== */
#endif  /* LCD_CTRL == LCD_CTRL_PCF2119 */
#endif /* AVRIO_LCD_ENABLE defined */
