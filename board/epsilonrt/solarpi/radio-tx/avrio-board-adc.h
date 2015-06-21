/**
 * Copyright © 2015 Pascal JEAN aka epsilonRT <pascal.jean--AT--btssn.net>
 * All rights reserved.
 */
#ifndef _AVRIO_BOARD_ADC_H_
#define _AVRIO_BOARD_ADC_H_

#include <avr/io.h>

/* constants ================================================================ */
#define ADC_AREF            eAdcInternal
#define ADC_VREF            1.1
#define ADC_CKDIV           32
#define ADC_CHAN_QUANTITY   2
#define ADC_CHAN_LIST       {0, 1}
#define ADC_FULLSCALE_LIST { (39000.0 + 11000.0) / 11000.0 * ADC_VREF, (16000.0 + 2200.0) / 2200.0 * ADC_VREF}

/* inline public functions ================================================== */
// -----------------------------------------------------------------------------
static inline void
vAdcSetChannel (uint8_t ucChan) {

  ADMUX = (ADMUX & ~0x1F) | (ucChan & 0x1F);
}

// -----------------------------------------------------------------------------
#endif  /* _AVRIO_BOARD_ADC_H_ not defined */
