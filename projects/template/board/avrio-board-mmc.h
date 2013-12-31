/*
 * @file avrio-board-mmc.h
 * @brief Configuration interface matérielle MMC/SD
 * @author Copyright © 2011-2012 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * @version $Id: avrio-board-mmc.h 260 2012-05-17 14:25:51Z epsilonrt $
 * Revision History ---
 *    20120323 - Initial version by epsilonRT
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
#define MMC_CS		  4
#define MMC_CS_PORT	PORTB
#define MMC_CS_DIR	DDRB

/*
 * Card Detect
 * Comment out the definition of MMC_CD to disable this option.
 */
#define MMC_CD		    5
#define MMC_CD_PORT	  PORTD
#define MMC_CD_PIN	  PIND
#define MMC_CD_DIR	  DDRD
#define MMC_CD_LEVEL  0     // 0 Active Low - 1 Active High

/*
 * Write Protect
 * Comment out the definition of MMC_WP to disable this option.
 */
#define MMC_WP		    4
#define MMC_WP_PORT	  PORTD
#define MMC_WP_PIN	  PIND
#define MMC_WP_DIR	  DDRD
#define MMC_WP_LEVEL  1     // 0 Active Low - 1 Active High

/*
 * Card Enable
 * Comment out the definition of MMC_EN to disable this option.
 */
//#define MMC_EN		    7
#define MMC_EN_PORT	  PORTB
#define MMC_EN_DIR	  DDRB
#define MMC_EN_LEVEL  0     // 0 Active Low - 1 Active High

/* ========================================================================== */
#endif /* _AVRIO_BOARD_MMC_H_ */
