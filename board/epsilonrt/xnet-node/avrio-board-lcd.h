/*
 * @file avrio-board-lcd.h
 * @brief Configuration afficheur LCD
 * Copyright © 2014 Pascal JEAN aka epsilonRT <pascal.jean--AT--btssn.net>
 * All rights reserved.
 * This software is governed by the CeCILL license <http://www.cecill.info>
 * Revision History ------------------------------------------------------------
 *    20120501 - Initial version by epsilonRT
 */
#ifndef _AVRIO_BOARD_LCD_H_
#define _AVRIO_BOARD_LCD_H_

// #define AVRIO_LCD_NOAUTO_RETURN

#if defined(AVRIO_LCD_ST7032)
#include "avrio-board-lcd-st7032.h"
#elif defined(AVRIO_LCD_THMI)
#include "avrio-board-lcd-thmi.h"
#else
#error Error: AVRIO_LCD_ST7032 or AVRIO_LCD_THMI must be defined !
#endif

/* ========================================================================== */
#endif  /* _AVRIO_BOARD_LCD_H_ not defined */
