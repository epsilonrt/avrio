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
