/*
 * @file avrio-board-mmc.h
 * @brief Configuration interface matérielle MMC/SD
 *
 * Revision History ------------------------------------------------------------
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
#define MMC_CD_PORT	  PORTE
#define MMC_CD_PIN	  PINE
#define MMC_CD_DIR	  DDRE
#define MMC_CD_LEVEL  0     // 0 Active Low - 1 Active High

/*
 * Write Protect
 * Comment out the definition of MMC_WP to disable this option.
 */
#define MMC_WP		    4
#define MMC_WP_PORT	  PORTE
#define MMC_WP_PIN	  PINE
#define MMC_WP_DIR	  DDRE
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
