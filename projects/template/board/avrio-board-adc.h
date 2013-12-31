/*
 * @file avrio-board-adc.h
 * @brief Configuration ADC
 * @author Copyright © 2012 epsilonRT. All rights reserved.
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
/* Différentes valeurs possibles pour ADC_CLKDIV */
#define ADC_CLKDIV_2    1
#define ADC_CLKDIV_4    2
#define ADC_CLKDIV_8    3
#define ADC_CLKDIV_16   4
#define ADC_CLKDIV_32   5
#define ADC_CLKDIV_64   6
#define ADC_CLKDIV_128  7

/* setup ==================================================================== */
/* Facteur de prédivision de l'horloge ADC */
#define ADC_CLKDIV  ADC_CLKDIV_64

/* Choix de la tension de référence */
#define ADC_REF ((1<<REFS1)|(1<<REFS1))

/* inline public functions ================================================== */
// -----------------------------------------------------------------------------
static inline void
vAdcSetChannel (uint8_t ucChan) {

  ADMUX = (ADMUX & ~0x1F) | (ucChan & 0x1F);
}

// -----------------------------------------------------------------------------
#endif  /* _AVRIO_BOARD_ADC_H_ not defined */
