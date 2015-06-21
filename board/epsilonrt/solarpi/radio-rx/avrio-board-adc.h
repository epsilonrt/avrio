/*
 * @file avrio-board-adc.h
 * @brief Configuration ADC
 * Copyright � 2012 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * Revision History ------------------------------------------------------------
 *    20120519 - Initial version by epsilonRT
 */

#ifndef _AVRIO_BOARD_ADC_H_
#define _AVRIO_BOARD_ADC_H_

#include <avr/io.h>

/* constants ================================================================ */
#define ADC_AREF            eAdcVcc
#define ADC_VREF            5.0
#define ADC_CKDIV           16
#define ADC_CHAN_QUANTITY   2
#define ADC_CHAN_LIST       {0, 3}
#define ADC_FULLSCALE_LIST {ADC_VREF, ADC_VREF}

/* inline public functions ================================================== */
// -----------------------------------------------------------------------------
static inline void
vAdcSetChannel (uint8_t ucChan) {

  ADMUX = (ADMUX & ~0x1F) | (ucChan & 0x1F);
}

// -----------------------------------------------------------------------------
#endif  /* _AVRIO_BOARD_ADC_H_ not defined */
