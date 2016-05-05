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
 * @file lcd_ctrl_st7032.c
 * @brief Contrôleur LCD ST7032
 *
 *
 * Revision History ------------------------------------------------------------
 *    20120501 - Initial version by epsilonRT
 */
#include "avrio-config.h"

#ifdef AVRIO_LCD_ENABLE
#include "avrio-board-lcd.h"

#if LCD_CTRL == LCD_CTRL_ST7032
/* ========================================================================== */
#include <avrio/delay.h>
#include <avrio/lcd/io/lcd_io.h>
#include "lcd_ctrl.h"
#include "lcd_ctrl_st7032.h"

/* constants ================================================================ */
/* Temps d'exécution de l'instruction Clear Display et Retrun Home en ms */
#define HOME_DELAY 1
/* Délai de stabilisation de l'alim interne en ms */
#define FOLOW_DELAY 200
/* Délai d'exécution en ṁs */
#define EXEC_DELAY 30

/* Calcul des réglages de l'alimentation LCD interne en fonction de VDD */
#if LCD_CTRL_ST7032_VDD > 0
# if LCD_CTRL_ST7032_VDD <= 3600
  /* VDD inférieure à 3.5 V */
#  define ST7032_BON 1
#  define ST7032_FON 1
#  define ST7032_RAB 4
# else
  /* VDD supérieure ou égale à 3.5 V (Max. 5V !) */
#  define ST7032_BON 0
#  define ST7032_FON 1
#  define ST7032_RAB 4
# endif /* LCD_CTRL_ST7032_VDD < 3500 */
#else
/* V0 externe */
# define ST7032_BON 0
# define ST7032_FON 0
# define ST7032_RAB 0
#endif /* LCD_CTRL_ST7032_VDD > 0 */

/* private variables ======================================================== */
static xLcdCoord xLcdCtrlX, xLcdCtrlY; /* Position du curseur */
static uint8_t ucLcdCtrlCursorAddr; /* Adresse DDRAM du curseur */
static uint8_t ucFsetCmd; /* Octet Function Set à l'init (pour accès mode étendu) */

/* private functions ======================================================== */

// -----------------------------------------------------------------------------
static void 
prvvWrite (uint8_t ucData, uint8_t ucAddress) {

  vLcdIoWrite (ucData, ucAddress);

  /* Délai d'exécution des commandes */
  if (ucAddress == ST7032_IR) {
  
    if ((ucData & 0xE0) == ST7032_FSET) {
    
      ucFsetCmd = ucData;
    }
      
    if (ucFsetCmd & ST7032_IS) {
      /* Mode étendu */
    
      if ((ucData & ST7032_FOLOW) != 0)
        delay_ms (FOLOW_DELAY);
    }
    else {
      /* Mode HD44780 */
    
      if (ucData <= ST7032_HOME)
        delay_ms (HOME_DELAY);
    }
  }
  delay_us (EXEC_DELAY);
}

// -----------------------------------------------------------------------------
static inline void
prvvSetPosition (void) {

  vLcdCtrlPutCmd (ST7032_DMEM | ucLcdCtrlCursorAddr);
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
  
    vLcdCtrlPutCmd (LCD_CTRL_ST7032_INIT_FSET | ST7032_DL);  /* function set */
    
    vLcdCtrlPutCmd (ucFsetCmd | ST7032_IS); /* passe en mode étendu */
    vLcdCtrlPutCmd (ST7032_SETOSC(0,4));  /* BS=0, F=4 -> F. Frame ~ 190 Hz */
    vLcdCtrlPutCmd (ST7032_SETPOWER(0,ST7032_BON,0));
    vLcdCtrlPutCmd (ST7032_SETFOLOW(ST7032_FON,ST7032_RAB));
    
    vLcdCtrlPutCmd (ucFsetCmd & ~ST7032_IS); /* repasse en mode standard */
    vLcdCtrlPutCmd (LCD_CTRL_ST7032_INIT_DCTL);  /* display control */
    vLcdCtrlPutCmd (LCD_CTRL_ST7032_INIT_ENTM);  /* entry mode set */
    vLcdCtrlBacklightInit ();
  }
  
  return iError;
}

// -----------------------------------------------------------------------------
uint8_t 
ucLcdCtrlContrastSet (uint8_t ucContrast) {
  uint8_t ucCurrent;

  ucContrast &= 0x1F;
  ucCurrent = ucContrast << 1; /* Contraste interne ST7032 entre 0 et 63 */
  vLcdCtrlPutCmd (ucFsetCmd | ST7032_IS); /* passe en mode étendu */
  vLcdCtrlPutCmd (ST7032_SETCONTR (ucCurrent));
  vLcdCtrlPutCmd (ST7032_SETPOWER (0,ST7032_BON, ucCurrent >> 4));
  vLcdCtrlPutCmd (ucFsetCmd & ~ST7032_IS); /* repasse en mode standard */
  return ucContrast;
}

// -----------------------------------------------------------------------------
void
vLcdCtrlPutChar (char cChar) {

  prvvWrite (cChar, ST7032_DR);
  xLcdCtrlX++;
}

// -----------------------------------------------------------------------------
void
vLcdCtrlPutCmd (char cCmd) {

  prvvWrite (cCmd, ST7032_IR);
}

// -----------------------------------------------------------------------------
void
vLcdCtrlClear (void) {

  vLcdCtrlPutCmd (ST7032_CLR);
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

  vLcdCtrlPutCmd (ST7032_DCTL | ST7032_D | (uint8_t)eCursor);
}

#if defined(LCD_CTRL_ICON_ENABLE)
#include <avr/pgmspace.h>
// -----------------------------------------------------------------------------
void
vLcdCtrlSetIcon (uint8_t ucCode, const uint8_t * pucIconPattern) {
  uint8_t ucOffset;

  ucCode *= LCD_PATTERN_HEIGHT; /* Calcul de l'adresse en CGRAM */
  vLcdCtrlPutCmd  (ST7032_GMEM | (ucCode & 0x3F));

  for (ucOffset = 0; ucOffset < LCD_PATTERN_HEIGHT; ucOffset++) {

    prvvWrite (pgm_read_byte (pucIconPattern + ucOffset), ST7032_DR);
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
#endif  /* LCD_CTRL == LCD_CTRL_ST7032 */
#endif /* AVRIO_LCD_ENABLE defined */
