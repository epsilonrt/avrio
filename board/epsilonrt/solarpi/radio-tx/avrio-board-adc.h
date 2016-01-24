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
/*
 * Valeur nominale ADC_VREF de 1.1V, modifier ADC_VREF en fonction de la valeur
 * mesurée sur la broche AREF avec un voltmètre de référence. Cette mesure
 * doit être faite pendant que le test unitaire ADC est en cours d'exécution.
 */
#define ADC_VREF            1.073
#define ADC_CKDIV           128
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
