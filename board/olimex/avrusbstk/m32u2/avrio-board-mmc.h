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
#ifndef _AVRIO_BOARD_MMC_H_
#  define _AVRIO_BOARD_MMC_H_
/* ========================================================================== */

/* MMC ====================================================================== */
#  include <avrio/defs.h>
#  include <avr/io.h>

/* constants ================================================================ */
/*
 * Chip Select (Active Low)
 */
#define MMC_CS		  2
#define MMC_CS_PORT	PORTC
#define MMC_CS_DIR	DDRC

/*
 * Card Detect
 * Comment out the definition of MMC_CD to disable this option.
 */
#define MMC_CD		    1
#define MMC_CD_PORT	  PORTD
#define MMC_CD_PIN	  PIND
#define MMC_CD_DIR	  DDRD
#define MMC_CD_LEVEL  0     // 0 Active Low - 1 Active High

/*
 * Write Protect
 * Comment out the definition of MMC_WP to disable this option.
 */
#define MMC_WP		    5
#define MMC_WP_PORT	  PORTD
#define MMC_WP_PIN	  PIND
#define MMC_WP_DIR	  DDRD
#define MMC_WP_LEVEL  1     // 0 Active Low - 1 Active High

/* ========================================================================== */
#endif /* _AVRIO_BOARD_MMC_H_ */
