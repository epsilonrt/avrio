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
#define ADC_AREF            eAdcInternal
#define ADC_VREF            1.073
#define ADC_CKDIV           32
#define ADC_CHAN_QUANTITY   1
#define ADC_CHAN_LIST       {0}
#define ADC_R7              20000.
#define ADC_R8              10161.
#define ADC0_FULLSCALE       ((ADC_R7 + ADC_R8) / ADC_R8 * ADC_VREF)
#define ADC_FULLSCALE_LIST  { ADC0_FULLSCALE }

/* inline public functions ================================================== */
// -----------------------------------------------------------------------------
static inline void
vAdcSetChannel (uint8_t ucChan) {

  ADMUX = (ADMUX & ~0x1F) | (ucChan & 0x1F);
}

// -----------------------------------------------------------------------------
#endif  /* _AVRIO_BOARD_ADC_H_ not defined */
