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
#ifndef _AVRIO_BOARD_ADC_H_
#define _AVRIO_BOARD_ADC_H_

#include <avr/io.h>

/* constants ================================================================ */
/* Pour le programme de test */
#define ADC_CHAN_QUANTITY 4
#define ADC_CHAN_LIST {0, 1, 2, 8}
#define ADC_FULLSCALE_LIST {4.95, 8.0, 4.95, 1.1}

#define ADC_SCALE_ENABLE
#define ADC_SCALE_MAX_LIST { 3, 0, 0, 0 }

#define ADC_AUTOSCALE_ENABLE
#define ADC_AUTOSCALE_MIN 80
#define ADC_AUTOSCALE_MAX 1010
#define ADC_AUTOSCALE_MAXLOOP 4

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
#define ADC_REF (1<<REFS0)

/* inline public functions ================================================== */
// -----------------------------------------------------------------------------
static inline void
vAdcSetChannel (uint8_t ucChan) {

  ADMUX = (ADMUX & ~0x1F) | (ucChan & 0x1F);
}

// -----------------------------------------------------------------------------
#include <avrio/bisrelay.h>
#include <avrio/delay.h>
static inline void
vAdcHwSetScale (uint8_t ucChannel, uint8_t ucScale) {

  if (ucChannel == 0) {
    static const uint8_t ucMask[] = { 0b000, 0b010, 0b001, 0b101 };

    vBisRelaySetAll (ucMask[ucScale]);
    // delay_ms(20);
  }
}

// -----------------------------------------------------------------------------
static inline void
vAdcHwInitScale (void) {

  vBisRelayInit();
}
// -----------------------------------------------------------------------------
#endif  /* _AVRIO_BOARD_ADC_H_ not defined */
