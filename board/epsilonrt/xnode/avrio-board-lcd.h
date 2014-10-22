/*
 * @file avrio-board-lcd.h
 * @brief Configuration afficheur LCD
 * @author epsilonRT. Copyright © 2012 All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * @version $Id: avrio-board-lcd.h 26 2013-02-14 22:30:34Z pjean $
 * Revision History ---
 *    20120501 - Initial version by epsilonRT
 */
#ifndef _AVRIO_BOARD_LCD_H_
#define _AVRIO_BOARD_LCD_H_

#if defined(AVRIO_LCD_ST7032)
#include "avrio-board-lcd-st7032.h"
#elif defined(AVRIO_LCD_THMI)
#include "avrio-board-lcd-thmi.h"
#else
#error Error: AVRIO_LCD_ST7032 or AVRIO_LCD_THMI must be defined !
#endif

/* ========================================================================== */
#endif  /* _AVRIO_BOARD_LCD_H_ not defined */
