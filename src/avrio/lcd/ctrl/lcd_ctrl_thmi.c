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
 * @file lcd_ctrl_twihmi.c
 * @brief Contrôleur LCD THMI
 *
 *
 * Revision History ------------------------------------------------------------
 *    20120501 - Initial version by epsilonRT
 */
#include "avrio-config.h"

#ifdef AVRIO_LCD_ENABLE
#include "avrio-board-lcd.h"

#if LCD_CTRL == LCD_CTRL_THMI
/* ========================================================================== */
#include <avrio/delay.h>
#include <avrio/lcd/io/lcd_io.h>
#include "lcd_ctrl.h"
#include "lcd_ctrl_thmi.h"

/* constants ================================================================ */
/* Temps d'exécution d'une instruction en ṁs */
#define EXEC_DELAY 50
/* Temps d'exécution des instructions Clear Display et Return Home en ms */
#define HOME_DELAY 4
/* Temps d'exécution du reset logiciel en ms */
#define EXTENDED_DELAY 50

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
  if (ucAddress == THMI_IR) {
  
    if ((ucData & 0xE0) == THMI_FSET) {
      
      // Commande FSET
      ucFsetCmd = ucData;
    }
    else {
    
      if (ucFsetCmd & THMI_IS) {
      
        // Commande en mode étendu
        delay_ms (EXTENDED_DELAY);
      } 
      else {
      
        // Commande en mode normal
        if (ucData <= THMI_HOME)
          delay_ms (HOME_DELAY);
      }
    }
  }
  delay_us (EXEC_DELAY);
}

// -----------------------------------------------------------------------------
static inline void
prvvSetPosition (void) {

  vLcdCtrlPutCmd (THMI_DMEM | ucLcdCtrlCursorAddr);
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

#ifdef LCD_CTRL_THMI_NRDY 
  // Configuration de la broche relié à HIRQ en entrée sans R pullup
  LCD_CTRL_THMI_NRDY_DDR  &= ~_BV(LCD_CTRL_THMI_NRDY);
  LCD_CTRL_THMI_NRDY_PORT &= ~_BV(LCD_CTRL_THMI_NRDY);
  // Attente mise à 1 de HIRQ (fin d'initialisation)
  while ((LCD_CTRL_THMI_NRDY_PIN & _BV(LCD_CTRL_THMI_NRDY)) == 0)
    delay_ms(50);
#else
  delay_ms (200);
#endif

  iError = iLcdIoInit ();
  
  if (iError == 0) {
  
    /* Ok, configuration de l'afficheur */
    
    vLcdCtrlPutCmd (LCD_CTRL_THMI_INIT_FSET);  /* repasse en mode standard et function set */
    vLcdCtrlPutCmd (LCD_CTRL_THMI_INIT_DCTL);  /* display control */
    vLcdCtrlPutCmd (LCD_CTRL_THMI_INIT_ENTM);  /* entry mode set */
  }
  
  return iError;
}

// -----------------------------------------------------------------------------
uint8_t 
ucLcdCtrlContrastSet (uint8_t ucContrast) {

  ucContrast &= 0x1F;
  vLcdCtrlPutExtCmd (THMI_SETCONTR (THMI_BON, ucContrast));
  return ucContrast;
}

// -----------------------------------------------------------------------------
void
vLcdCtrlPutChar (char cChar) {

  prvvWrite (cChar, THMI_DR);
  xLcdCtrlX++;
}

// -----------------------------------------------------------------------------
void
vLcdCtrlPutCmd (char cCmd) {

  prvvWrite (cCmd, THMI_IR);
}

// -----------------------------------------------------------------------------
void 
vLcdCtrlPutExtCmd (char cCmd) {

  vLcdCtrlPutCmd (ucFsetCmd | THMI_IS); /* passe en mode étendu */
  vLcdCtrlPutCmd (cCmd);
  vLcdCtrlPutCmd (ucFsetCmd & ~THMI_IS); /* repasse en mode standard */
}

// -----------------------------------------------------------------------------
void
vLcdCtrlClear (void) {

  xLcdCtrlY = xLcdCtrlX = 0;
  vLcdCtrlPutCmd (THMI_CLR);
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

  vLcdCtrlPutCmd (THMI_DCTL | THMI_D | (uint8_t)eCursor);
}

#if defined(LCD_CTRL_ICON_ENABLE)
#include <avr/pgmspace.h>

// -----------------------------------------------------------------------------
void
vLcdCtrlSetIcon (uint8_t ucCode, const uint8_t * pucIconPattern) {
  uint8_t ucOffset;

  ucCode *= LCD_PATTERN_HEIGHT; /* Calcul de l'adresse en CGRAM */
  vLcdCtrlPutCmd  (THMI_GMEM | (ucCode & 0x3F));

  for (ucOffset = 0; ucOffset < LCD_PATTERN_HEIGHT; ucOffset++) {

    prvvWrite (pgm_read_byte (pucIconPattern + ucOffset), THMI_DR);
  }
  prvvSetPosition ();
}
#endif /* LCD_CTRL_ICON_ENABLE */

#ifdef LCD_CTRL_BACKLIGHT_ENABLE
/* ========================================================================== */

/* internal public functions ================================================ */

// -----------------------------------------------------------------------------
uint8_t 
ucLcdCtrlBacklightSet (uint8_t ucLevel) {

  ucLevel &= 0x3F;;
  vLcdCtrlPutExtCmd (THMI_SETBL (ucLevel));
  return ucLevel;
}

/* ========================================================================== */
#endif  /* LCD_CTRL_BACKLIGHT_ENABLE defined */

/* ========================================================================== */
#endif  /* LCD_CTRL == LCD_CTRL_THMI */
#endif /* AVRIO_LCD_ENABLE defined */
