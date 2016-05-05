/*
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
