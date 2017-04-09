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
   AREF Pin Disconnected
 * ADC_AREF     Tension de référence sélectionnée
   0            VCC
   1            Internal 1.1V reference 
   2            Internal 2.2V reference 
   3            Internal 4.096V reference
   AREF Pin Connected, with external bypass capacitor connected to pin
 * ADC_AREF     Tension de référence sélectionnée
   4            AREF pin (internal reference turned off)
   5            Internal 1.1V reference 
   6            Internal 2.2V reference 
   7            Internal 4.096V reference
 */
#define ADC_AREF            7
// Valeur de la tension de référence (à ajuster en fonction de ADC_AREF)
#define ADC_VREF            4.096
// Rapport de division de l'horloge ADC qui fixe le temps d'échantillonage
#define ADC_CKDIV           32

// Nombre de voies ADC utilisées
#define ADC_CHAN_QUANTITY   1

// Liste des voies ADC utilisées
#define ADC_CHAN_LIST       {3}

// Liste des valeurs pleine échelle
#define ADC_FULLSCALE_LIST  { ADC_VREF }

/* inline public functions ================================================== */
// -----------------------------------------------------------------------------
static inline void
vAdcSetChannel (uint8_t ucChan) {

  ADMUXA = (ADMUXA & ~0x3F) | (ucChan & 0x3F);
}

// -----------------------------------------------------------------------------
#endif  /* _AVRIO_BOARD_ADC_H_ not defined */
