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
 * @file lcd_ctrl_hd44780.c
 * @brief Contrôleur LCD HD44780
 *
 *
 * Revision History ------------------------------------------------------------
 *    20120501 - Initial version by epsilonRT
 */
#include "avrio-config.h"

#ifdef AVRIO_LCD_ENABLE
#include "avrio-board-lcd.h"

#if LCD_CTRL == LCD_CTRL_HD44780
/* ========================================================================== */
#include <avrio/delay.h>
#include <avrio/lcd/io/lcd_io.h>
#include "lcd_ctrl.h"
#include "lcd_ctrl_hd44780.h"

/* constants ================================================================ */
/* Temps d'exécution d'une instruction (si BF non utilisé) en ṁs */
#define EXEC_DELAY 50
/* Temps d'exécution des instructions Clear Display et Return Home en ms */
#define HOME_DELAY 3

#if (LCD_IO & (1<<LCD_IO_4BITS)) != 0
/* Mode 4 bits */
# define HD44780_INIT_FSET ((LCD_CTRL_HD44780_INIT_FSET) & ~HD44780_DL)
#else
/* Mode 8 bits */
# define HD44780_INIT_FSET ((LCD_CTRL_HD44780_INIT_FSET) |  HD44780_DL)
#endif
/* private variables ======================================================== */
static xLcdCoord xLcdCtrlX, xLcdCtrlY;
static uint8_t ucLcdCtrlCursorAddr;

/* private functions ======================================================== */

// -----------------------------------------------------------------------------
static void 
prvvWrite (uint8_t ucData, uint8_t ucAddress) {

  vLcdIoWrite (ucData, ucAddress);

  delay_us (EXEC_DELAY);
  /* Délai supplémentaire pour HOME et CLR */
  if ((ucAddress == HD44780_IR) && (ucData <= HD44780_HOME))
    delay_ms (HOME_DELAY); /* 1.5 ms min */
}

// -----------------------------------------------------------------------------
static inline void
prvvSetPosition (void) {

  vLcdCtrlPutCmd (HD44780_DMEM | ucLcdCtrlCursorAddr);
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
  
  iError = iLcdIoInit ();
  
  if (iError == 0) {
  
    /* reset logiciel */
    delay_ms (15);  /* 15 ms min. */
    vLcdIoWriteNibble (0x30, HD44780_IR); /* 8 bits mode (1) */
    delay_ms (5); /* 4.1 ms min. */
    vLcdIoWriteNibble (0x30, HD44780_IR); /* 8 bits mode (2) */
    delay_ms (1); /* 100 us min. */
    vLcdIoWriteNibble (0x30, HD44780_IR); /* 8 bits mode (3) */
    delay_ms (1); /* 50 us min. */
    /* fin du reset logiciel, BF peut être lu à partir de maintenant */

#if (LCD_IO & (1<<LCD_IO_4BITS)) != 0
    /* Mode 4 bits */
    vLcdIoWriteNibble (0x20, HD44780_IR); /* 4 bits mode (4) */
    delay_ms (1); /* 50 us min. */
#endif
    /* Ok, configuration de l'afficheur */
    vLcdCtrlPutCmd (HD44780_INIT_FSET);  /* function set */
    vLcdCtrlPutCmd (LCD_CTRL_HD44780_INIT_DCTL);  /* display control */
    vLcdCtrlPutCmd (LCD_CTRL_HD44780_INIT_ENTM);  /* entry mode set */
    vLcdCtrlBacklightInit ();
  }
  
  return iError;
}

// -----------------------------------------------------------------------------
#ifndef LCD_CTRL_CONTRAST_ENABLE
uint8_t 
ucLcdCtrlContrastSet (uint8_t ucContrast) {

  // Non pris en charge
  return 0xFF;
}
#endif

// -----------------------------------------------------------------------------
void
vLcdCtrlPutChar (char cChar) {

  prvvWrite (cChar, HD44780_DR);
  xLcdCtrlX++;
}

// -----------------------------------------------------------------------------
void
vLcdCtrlPutCmd (char cCmd) {

  prvvWrite (cCmd, HD44780_IR);
}

// -----------------------------------------------------------------------------
void
vLcdCtrlClear (void) {

  xLcdCtrlY = xLcdCtrlX = 0;
  vLcdCtrlPutCmd (HD44780_CLR);
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

  vLcdCtrlPutCmd (HD44780_DCTL | HD44780_D | (uint8_t)eCursor);
}

#if defined(LCD_CTRL_ICON_ENABLE)
#include <avr/pgmspace.h>

// -----------------------------------------------------------------------------
void
vLcdCtrlSetIcon (uint8_t ucCode, const uint8_t * pucIconPattern) {
  uint8_t ucOffset;

  ucCode *= LCD_PATTERN_HEIGHT; /* Calcul de l'adresse en CGRAM */
  vLcdCtrlPutCmd  (HD44780_GMEM | (ucCode & 0x3F));

  for (ucOffset = 0; ucOffset < LCD_PATTERN_HEIGHT; ucOffset++) {

    prvvWrite (pgm_read_byte (pucIconPattern + ucOffset), HD44780_DR);
  }
  prvvSetPosition ();
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
#endif  /* LCD_CTRL == LCD_CTRL_HD44780 */
#endif /* AVRIO_LCD_ENABLE defined */
