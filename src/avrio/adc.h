/**
 * @file adc.h
 * @brief Convertisseur analogique-numérique
 * @author Copyright © 2012 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * @version $Id$
 * Revision History ---
 *    20120519 - Initial version by epsilonRT
 */
#ifndef _AVRIO_ADC_H_
#define _AVRIO_ADC_H_

#include <avrio/defs.h>

/* *INDENT-OFF* */
__BEGIN_C_DECLS
/* ========================================================================== */
/**
 * @addtogroup dev_group
 * @{
 *
 *  @defgroup adc_module Convertisseur Analogique-Numérique
 *  Ce module permet de gérer le convertisseur intégré au MCU.
 *  @{
 *    @example adc/demo_adc.c
 *    Exemple qui affiche les voies de l'ADC sur la liaison série ou l'afficheur
 *    LCD.
 */

/* constants ================================================================ */
/**
 * @brief Tension de référence
 */
typedef enum {
  eAdcExternal = 0, ///< Broche AREF
  eAdcVcc = 1,      ///< Tension AVCC avec un condensateur sur AREF
  eAdcInternal = 3  ///< Référence interne
} eAdcRef;

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
 * @brief Lecture de l'ADC avec moyennage
 *
 * Effectue ucTerms mesures à 100 µs d'intervalle et en calcul la moyenne.
 * La valeur de l'écart de temps entre deux mesures peut être modifié
 * en définissant la macro ADC_AVERAGE_DELAYUS dans le fichier avrio-board-adc.h
 * à une valeur en µs.
 *
 * @param ucChannel Numéro de la voie du multiplexeur ADC
 * @param ucTerms Nombre de termes de la moyenne
 * @return Valeur moyennée sur 10 bits sur ucTerms mesures
 */
uint16_t usAdcReadAverage (uint8_t ucChannel, uint8_t ucTerms);


/**
 * @brief Modifie la voie du multiplexeur
 *
 * Cette fonction ne devrait pas être appellée par l'utilisateur mais
 * uniquement par les fonctions de lecture. \n
 * Cette fonction est implémentée dans avrio-board-adc.h
 * @param ucChannel Numéro de la voie du multiplexeur ADC
 */
static inline void vAdcSetChannel (uint8_t ucChan);

// -----------------------------------------------------------------------------
#  if defined(__DOXYGEN__)
/*
 * __DOXYGEN__ defined
 * Partie documentation ne devant pas être compilée.
 * =============================================================================
 */
/**
 * @brief Modifie l'échelle d'une voie
 */
void vAdcSetScale (uint8_t ucChannel, uint8_t ucScale);

/**
 * @brief Lecture de l'échelle d'une voie
 */
uint8_t ucAdcGetScale (uint8_t ucChannel);

/**
 * @brief Modifie la tension de référence
 */
static inline void vAdcSetRef (eAdcRef eRef);

/**
 * @brief Renvoie la tension de référence
 */
static inline eAdcRef eAdcGetRef (void)

/**
 * @brief Active la voie AutoScale
 */
static inline void vAdcSetAutoscale (uint8_t ucChannel);

/**
 * @brief Désactive la voie AutoScale
 */
static inline void vAdcClearAutoscale (uint8_t ucChannel);

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

#if defined(ADC_SCALE_ENABLE)
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
  ((((float)(_VALUE))*((float)(_FULL_SCALE)))/1024.0)
#  endif /* __DOXYGEN__ not defined */

/* ========================================================================== */
__END_C_DECLS
/* *INDENT-ON* */
#endif  /* _AVRIO_ADC_H_ not defined */
