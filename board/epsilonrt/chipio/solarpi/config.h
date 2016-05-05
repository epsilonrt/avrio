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
#ifndef _CONFIG_H_
#define _CONFIG_H_

/* constants ================================================================ */
#define CFG_TWI_ADDR      (0x46<<1)
#define CFG_LCD_CONTRAST  LCD_DEFAULT_CONTRAST
#define CFG_LCD_BACKLIGHT 32

#define CFG_LCDBIAS_MIN   0x80
#define CFG_LCDBIAS_MAX   0xFF

/* ========================================================================== */
#endif  /* _CONFIG_H_ not defined */
