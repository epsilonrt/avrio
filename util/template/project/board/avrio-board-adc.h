/*
 * Copyright © 2016 epsilonRT, All rights reserved.
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
/* Choix de la tension de référence
 * ADC_AREF       Tension de référence sélectionnée
 * eAdcExternal   Broche AREF, référence interne arrêtée
 * eAdcVcc        Tension AVCC (condensateur sur AREF nécessaire)
 * eAdcInternal   Référence interne (sur tous les MCU AVR8 disposant d'un ADC)
 * eAdcInternal2  Référence interne supplémentaire (REFS = 10, seuleument sur certains MCU)
 */
#define ADC_AREF            eAdcInternal
// Valeur de la tension de référence (à ajuster en fonction de ADC_AREF)
#define ADC_VREF            1.1
// Rapport de division de l'horloge ADC qui fixe le temps d'échantillonage
#define ADC_CKDIV           32

// Nombre de voies ADC utilisées
#define ADC_CHAN_QUANTITY   6

// Liste des voies ADC utilisées
#define ADC_CHAN_LIST       {0,1,2,3,4,5}

// Liste des valeurs pleine échelle
#define ADC_FULLSCALE_LIST  { ADC_VREF,ADC_VREF,ADC_VREF,ADC_VREF,ADC_VREF,ADC_VREF,ADC_VREF,ADC_VREF }

/* inline public functions ================================================== */
// -----------------------------------------------------------------------------
static inline void
vAdcSetChannel (uint8_t ucChan) {

  ADMUX = (ADMUX & ~0x1F) | (ucChan & 0x1F);
}

// -----------------------------------------------------------------------------
#endif  /* _AVRIO_BOARD_ADC_H_ not defined */
