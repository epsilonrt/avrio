/**
 * Copyright © 2011-2015 epsilonRT. All rights reserved.
 *
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
 *
 * @file adc.h
 * @brief Convertisseur analogique-numérique
 *
 * Revision History ------------------------------------------------------------
 *    20120519 - Initial version by epsilonRT
 */
#ifndef _AVRIO_ADC_H_
#define _AVRIO_ADC_H_

#include <avrio/defs.h>

__BEGIN_C_DECLS
/* ========================================================================== */
/**
 * @addtogroup dev_group
 * @{
 *
 *  @defgroup adc_module Convertisseur Analogique-Numérique
 *  Ce module permet de gérer le convertisseur intégré au MCU.
 *  @{
 *    @example analog/adc/demo_adc.c
 *    Affiche les voies de l'ADC sur la liaison série.
 */

/* constants ================================================================ */
/**
 * @brief Tension de référence
 */
typedef enum {
  eAdcExternal = 0,  ///< Broche AREF
  eAdcVcc = 1,       ///< Tension AVCC avec un condensateur sur AREF
  eAdcInternal  = 3, ///< Référence interne (sur tous les MCU AVR8 disposant d'un ADC)
  eAdcInternal2 = 2  ///< Référence interne supplémentaire (REFS = 10, seuleument sur certains MCU)
} eAdcRef;

/**
 * @brief Type de filtrage de la mesure
 */
typedef enum {
  eAdcRaw = 0,      ///< Mesure directe de l'ADC
  eAdcAverage = 1,  ///< Mesure avec moyennage
  eAdcMin = 2,      ///< Mesure valeur maximale
  eAdcMax = 4,      ///< Mesure valeur minimale
} eAdcFilter;

/* internal public functions ================================================ */
/**
 * @brief Initialise et valide l'ADC pour l'utilisation
 *
 * Les constantes d'initialisation dans avrio-board-adc.h sont les suivantes :
 * - ADC_CLKDIV: facteur de division de l'horloge
 * - ADC_REF: Tension de référence (codage conforme au registre ADMUX)
 * .
 */
void vAdcInit (void);

/**
 * @brief Valide l'ADC
 *
 * Si le MCU dispose d'un registre PRR, le bit PRADC est dévalidé pour permettre
 * le fonctionnement de l'ADC.
 */
void vAdcEnable (void);

/**
 * @brief Dévalide l'ADC
 *
 * Si le MCU dispose d'un registre PRR, le bit PRADC est validé pour permettre
 * d'économiser sur la consommation de courant en cas d'endormissement du MCU.
 */
void vAdcDisable (void);

/**
 * @brief Lecture de l'ADC
 * @param ucChannel Numéro de la voie du multiplexeur ADC
 * @return Valeur mesurée sur 10 bits
 */
uint16_t usAdcRead (uint8_t ucChannel);

/**
 * @brief Lecture de l'ADC avec filtrage
 *
 * Effectue ucTerms mesures à 100 µs d'intervalle et effectue un filtrage.
 * La valeur de l'écart de temps entre deux mesures peut être modifié
 * en définissant la macro ADC_FILTER_DELAYUS dans le fichier avrio-board-adc.h
 * à une valeur en µs. La somme est effectuée sur 32 bits.
 *
 * @param ucChannel Numéro de la voie du multiplexeur ADC
 * @param ucTerms Nombre de termes pour le filtrage
 * @param eFilter Type de filtrage
 * @return Valeur filtrée sur 10 bits sur ucTerms mesures
 */
uint16_t usAdcReadFilter (uint8_t ucChannel, uint8_t ucTerms, eAdcFilter eFilter);

/**
 * @brief Modifie la voie du multiplexeur
 *
 * Cette fonction ne devrait pas être appellée par l'utilisateur mais
 * uniquement par les fonctions de lecture. \n
 * Cette fonction est implémentée dans avrio-board-adc.h
 * @param ucChan Numéro de la voie du multiplexeur ADC
 */
static inline void vAdcSetChannel (uint8_t ucChan);

/**
 * @brief Modifie le facteur de division de l'horloge de l'ADC
 *
 * Le facteur de division se trouver dans la liste {2,4,8,16,32,64,128}.
 * Une conversion prend 13 cycles d'horloge ADC (sauf la première 25 cycles).
 * Par exemple, si l'horloge du MCU est de 8 Mhz avec un facteur division de
 * 8, la conversion prendra 13 microsecondes.
 */
void vAdcSetDiv (uint8_t ucDiv);

/**
 * @brief Renvoie le facteur de division de l'horloge de l'ADC
 */
uint8_t ucAdcGetDiv (void);

// -----------------------------------------------------------------------------
#  if defined(__DOXYGEN__)
/*
 * __DOXYGEN__ defined
 * Partie documentation ne devant pas être compilée.
 * =============================================================================
 */
/**
 * @brief Lecture de l'ADC avec moyennage
 *
 * Effectue ucTerms mesures à 100 µs d'intervalle et en calcul la moyenne.
 * La valeur de l'écart de temps entre deux mesures peut être modifié
 * en définissant la macro ADC_FILTER_DELAYUS dans le fichier avrio-board-adc.h
 * à une valeur en µs. La somme est effectuée sur 32 bits.
 *
 * @param ucChannel Numéro de la voie du multiplexeur ADC
 * @param ucTerms Nombre de termes de la moyenne
 * @return Valeur moyennée sur 10 bits sur ucTerms mesures
 * @deprecated Cette fonction est obsolète, utiliser usAdcReadFilter() à la
 * place.
 */
static inline uint16_t usAdcReadAverage (uint8_t ucChannel, uint8_t ucTerms);

/**
 * @brief Modifie la tension de référence
 */
static inline void vAdcSetRef (eAdcRef eRef);

/**
 * @brief Renvoie la tension de référence
 */
static inline eAdcRef eAdcGetRef (void)


/* macros =================================================================== */
/**
 * @brief Retourne le nombre de LSB associé à une grandeur
 * @param _MEASUREMENT Grandeur à convertir
 * @param _FULL_SCALE  Pleine échelle de l'ADC exprimée dans la même unité que
 * _MEASUREMENT
 * @return le nombre de LSB associé à _MEASUREMENT
 */
#define ADC_VALUE(_MEASUREMENT,_FULL_SCALE)

/**
 * @brief Retourne la grandeur associée à un nombre de LSB
 * @param _VALUE Nombre de LSB à convertir
 * @param _FULL_SCALE  Pleine échelle de l'ADC exprimée dans la même unité que
 * que la valeur retournée
 * @return La grandeur associée à _VALUE
 */
#define ADC_MEASUREMENT(_VALUE,_FULL_SCALE)

/**
 *   @}
 * @}
 */
#  else
/*
 * __DOXYGEN__ not defined
 * Partie ne devant pas être documentée.
 * =============================================================================
 */
#include "avrio-config.h"
#ifdef AVRIO_ADC_ENABLE
#include "avrio-board-adc.h"
#endif
#include <avrio/delay.h>

/* inline public functions ================================================== */
INLINE void vAdcSetRef (eAdcRef eRef) {

  ADMUX = (ADMUX & ~(_BV(REFS1)|_BV(REFS0))) | (eRef << REFS0);
  delay_ms (100);
}

INLINE eAdcRef eAdcGetRef (void) {

  return ADMUX >> REFS0;
}

INLINE uint16_t usAdcReadAverage (uint8_t ucChannel, uint8_t ucTerms) {

  return usAdcReadFilter (ucChannel, ucTerms, eAdcAverage);
}

#if defined(ADC_SCALE_ENABLE)
/**
 * @brief Modifie l'échelle d'une voie
 */
//void vAdcSetScale (uint8_t ucChannel, uint8_t ucScale);

/**
 * @brief Lecture de l'échelle d'une voie
 */
//uint8_t ucAdcGetScale (uint8_t ucChannel);

/**
 * @brief Active la voie AutoScale
 */
//static inline void vAdcSetAutoscale (uint8_t ucChannel);

/**
 * @brief Désactive la voie AutoScale
 */
//static inline void vAdcClearAutoscale (uint8_t ucChannel);

void vAdcSetScale (uint8_t ucChannel, uint8_t ucScale);
uint8_t ucAdcGetScale (uint8_t ucChannel);
uint8_t ucAdcGetScaleMax (uint8_t ucChannel);
#else
INLINE void vAdcSetScale (uint8_t ucChannel, uint8_t ucScale) {

}
INLINE uint8_t ucAdcGetScale (uint8_t ucChannel) {

  return 0;
}
INLINE uint8_t ucAdcGetScaleMax (uint8_t ucChannel) {

  return 0;
}
#endif

#if defined(ADC_AUTOSCALE_ENABLE)
extern uint16_t usAdcAutoScaleFlag;

INLINE void vAdcSetAutoscale (uint8_t ucChannel) {

  usAdcAutoScaleFlag = _BV(ucChannel);
}

INLINE void vAdcClearAutoscale (uint8_t ucChannel) {

  usAdcAutoScaleFlag = 0;
}
#else
INLINE void vAdcSetAutoscale (uint8_t ucChannel) {

}

INLINE void vAdcClearAutoscale (uint8_t ucChannel) {

}
#endif

/* macros =================================================================== */
#define ADC_VALUE(_MEASUREMENT,_FULL_SCALE) \
  ((uint16_t)(((_MEASUREMENT)*1024.0)/(_FULL_SCALE)))

#define ADC_MEASUREMENT(_VALUE,_FULL_SCALE) \
  ((((double)(_VALUE))*((double)(_FULL_SCALE)))/1024.0)
#  endif /* __DOXYGEN__ not defined */

/* ========================================================================== */
__END_C_DECLS
#endif  /* _AVRIO_ADC_H_ not defined */
