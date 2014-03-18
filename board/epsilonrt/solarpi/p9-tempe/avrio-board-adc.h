/*
 * @file avrio-board-adc.h
 * @brief Configuration ADC
 * @author Copyright � 2012 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * @version $Id$
 * Revision History ---
 *    20120519 - Initial version by epsilonRT
 */

#ifndef _AVRIO_BOARD_ADC_H_
#define _AVRIO_BOARD_ADC_H_

#include <avr/io.h>

/* constants ================================================================ */
/* Pour le programme de test */
#if   defined(AVRIO_BOARD_P1_HUM)
#define ADC_CHAN_LIST {0}
#define ADC_FULL_SCALE 4.95
#define ADC_CHAN_QUANTITY 1
#elif defined(AVRIO_BOARD_P6_LUM)
#define ADC_CHAN_LIST {0, 1, 2}
#define ADC_CHAN_QUANTITY 3
#define ADC_FULL_SCALE 1.1
#elif defined(AVRIO_BOARD_P8_PRESS)
#define ADC_CHAN_LIST {0}
#define ADC_CHAN_QUANTITY 1
#define ADC_FULL_SCALE 4.95
#elif defined(AVRIO_BOARD_P9_TEMPE)
#define ADC_CHAN_LIST {0}
#define ADC_CHAN_QUANTITY 1
#define ADC_FULL_SCALE 2.49
#elif defined(AVRIO_BOARD_P10_TEMPI)
#define ADC_CHAN_LIST {0}
#define ADC_CHAN_QUANTITY 1
#define ADC_FULL_SCALE 4.95
#endif

/* Diff�rentes valeurs possibles pour ADC_CLKDIV */
#define ADC_CLKDIV_2    1
#define ADC_CLKDIV_4    2
#define ADC_CLKDIV_8    3
#define ADC_CLKDIV_16   4
#define ADC_CLKDIV_32   5
#define ADC_CLKDIV_64   6
#define ADC_CLKDIV_128  7

/* setup ==================================================================== */
/* Facteur de pr�division de l'horloge ADC */
#define ADC_CLKDIV  ADC_CLKDIV_64

/* Choix de la tension de r�f�rence
 * Par exemple pour ATmega168
 * REFS1  REFS0   Voltage Reference Selection
 * 0      0       AREF, Internal Vref turned off
 * 0      1       AVCC with external capacitor at AREF pin
 * 1      0       Reserved
 * 1      1       Internal 1.1V Voltage Reference
 */
#define ADC_REF 0

/* inline public functions ================================================== */
// -----------------------------------------------------------------------------
static inline void
vAdcSetChannel (uint8_t ucChan) {

  ADMUX = (ADMUX & ~0x1F) | (ucChan & 0x1F);
}

// -----------------------------------------------------------------------------
#endif  /* _AVRIO_BOARD_ADC_H_ not defined */
