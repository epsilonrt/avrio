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
/* Choix de la tension de référence
 * ADC_AREF       Tension de référence sélectionnée
 * eAdcExternal   Broche AREF, référence interne arrêtée
 * eAdcVcc        Tension AVCC (condensateur sur AREF nécessaire)
 * eAdcInternal   Référence interne (sur tous les MCU AVR8 disposant d'un ADC)
 * eAdcInternal2  Référence interne supplémentaire (REFS = 10, seuleument sur certains MCU)
 */
#define ADC_AREF            eAdcVcc
// Valeur de la tension de référence (à ajuster en fonction de ADC_AREF)
#define ADC_VREF            3.3
// Rapport de division de l'horloge ADC qui fixe le temps d'échantillonage
#define ADC_CKDIV           32

// Nombre de voies ADC utilisées (ADC4 à 7 connectés au JTAG)
#define ADC_CHAN_QUANTITY   4

/* Liste des voies ADC utilisées :
 * ADC0 relié à une CTN NCP18WF104J03RB (peut être déconnecté avec CP11)
 * ADC1 relié à une LDR NSL19M51 (non montée par défaut, peut être déconnecté avec CP15)
 * ADC2 relié à VIN par un pont diviseur par 1/5 peut être déconnecté avec CP17)
 * ADC3 à ADC5 reliés directement à ANA CON
 * <ATTENTION> ADC4 à 7 connectés aussi au JTAG
 */
#define ADC_CHAN_LIST       {0,1,2,3}
// Valeur des résistances du pont diviseur sur ADC2 (VIN)
#define ADC2_R41              100000.
#define ADC2_R37_R38          400000.
#define ADC2_FULLSCALE       ((ADC2_R41 + ADC2_R37_R38) / ADC2_R41 * ADC_VREF)

// Liste des valeurs pleine échelle
#define ADC_FULLSCALE_LIST  { ADC_VREF, ADC_VREF, ADC2_FULLSCALE, ADC_VREF }

/* inline public functions ================================================== */
// -----------------------------------------------------------------------------
static inline void
vAdcSetChannel (uint8_t ucChan) {

  ADMUX = (ADMUX & ~0x1F) | (ucChan & 0x1F);
}

// -----------------------------------------------------------------------------
#endif  /* _AVRIO_BOARD_ADC_H_ not defined */
