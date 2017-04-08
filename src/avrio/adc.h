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
 * @example adc/demo_adc.c
 * Demo Convertisseur Analogique-Numérique (ADC)
 * Ce programme affiche des valeurs en LSB (0 à 1023) des voies ADC de la façon
 * suivante sur la liaison série (UART0).
 * 
 * @example sensor/ntc/demo_sensor_ntc.c
 * Demo capteur analogique de température (CTN)
 * Ce programme affiche la valeur brute en entrée ADC0 (en LSB de 0 à 1023),
 * la résistance de la CTN et la température sur la liaison série (UART0):
 */

/* constants ================================================================ */
/**
 * @brief Tension de référence
 * 
 * Ces valeurs correspondent aux bits REFS0 et REFS1 alignés à 
 * droite sur la plupart des modèles, mais sur certains modèles les 
 * valeurs sont spécifiques... cf la paragraphe ADC du datasheet.
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
 * @param ucRef cette valeur correspond aux bits REFS0, REFS1... alignés à 
 * droite et dépend du modèle de MCU. Sur la plupart des modèles, les valeurs 
 * correspondent à l'énumération \c eAdcRef, mais sur certains modèles les 
 * valeurs sont spécifiques... cf la paragraphe ADC du datasheet.
 */
static inline void vAdcSetRef (uint8_t ucRef);

/**
 * @brief Renvoie la tension de référence
 * @return cette valeur correspond aux bits REFS0, REFS1... alignés à droite et 
 * dépend du modèle de MCU. Sur la plupart des modèles, les valeurs 
 * correspondent à l'énumération \c eAdcRef, mais sur certains modèles les 
 * valeurs sont spécifiques... cf la paragraphe ADC du datasheet.
 */
static inline uint8_t eAdcGetRef (void)


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
#if defined(REFS2) && defined(REFS1) && defined(REFS0)
#define ADCREF_MASK (_BV(REFS2)|_BV(REFS1)|_BV(REFS0))
#elif defined(REFS1) && defined(REFS0)
#define ADCREF_MASK (_BV(REFS1)|_BV(REFS0))
#else
#define ADCREF_MASK (_BV(REFS0))
#endif

#if !defined(ADMUX) && defined(ADMUXB)
#define ADMUX ADMUXB
#endif


/* -----------------------------------------------------------------------------
 * Valeurs courantes:
    00 AREF, Internal V ref turned off
    01 AV CC with external capacitor at AREF pin
    10 Reserved
    11 Internal Voltage Reference with external capacitor at AREF pin
* attiny441/841: ADMUXB & REFS2 defined
    AREF Pin Disconnected
    000 VCC
    001 Internal 1.1V reference 
    010 Internal 2.2V reference 
    011 Internal 4.096V reference

    AREF Pin Connected, with external bypass capacitor connected to pin
    100 AREF pin (internal reference turned off)
    101 Internal 1.1V reference 
    110 Internal 2.2V reference 
    111 Internal 4.096V reference
 */
INLINE void vAdcSetRef (eAdcRef eRef) {

  ADMUX = (ADMUX & ~ADCREF_MASK) | (eRef << REFS0);
  delay_ms (100);
}

INLINE eAdcRef eAdcGetRef (void) {

  return (ADMUX & ADCREF_MASK)  >> REFS0;
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
