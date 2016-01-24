/*
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
