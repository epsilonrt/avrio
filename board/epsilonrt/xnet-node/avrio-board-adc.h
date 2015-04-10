/*
 * @file avrio-board-adc.h
 * @brief Configuration ADC
 * Copyright © 2014 Pascal JEAN aka epsilonRT <pascal.jean--AT--btssn.net>
 * All rights reserved.
 * This software is governed by the CeCILL license <http://www.cecill.info>
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

/* Choix de la tension de référence
 * Par exemple pour ATmega168
 * REFS1  REFS0   Voltage Reference Selection
 * 0      0       AREF, Internal Vref turned off
 * 0      1       AVCC with external capacitor at AREF pin
 * 1      0       Reserved
 * 1      1       Internal 1.1V Voltage Reference
 */
#define ADC_REF ((1<<REFS1)|(1<<REFS0))

/* inline public functions ================================================== */
// -----------------------------------------------------------------------------
static inline void
vAdcSetChannel (uint8_t ucChan) {

  ADMUX = (ADMUX & ~0x1F) | (ucChan & 0x1F);
}

// -----------------------------------------------------------------------------
#endif  /* _AVRIO_BOARD_ADC_H_ not defined */
