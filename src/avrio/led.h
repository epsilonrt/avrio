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
 * @file led.h
 * @brief LED (diodes électroluminescentes)
 */
#ifndef _AVRIO_LED_H_
#  define _AVRIO_LED_H_

#  include <avrio/defs.h>

__BEGIN_C_DECLS
  /* ======================================================================== */
#  include "avrio-config.h"
#  ifdef AVRIO_LED_ENABLE
#  include "avrio-board-led.h"
/**
 * @addtogroup dev_group
 * @{
 *
 *  @defgroup led_module Leds

 *  Ce module permet de gérer les leds de façons indépendantes de
 *  la plate-forme matérielle ou de façon plus générale tout actionneur
 *  fonctionnant en tout ou rien.\n
 *  La macro AVRIO_LED_ENABLE doit être définie dans avrio-config.h et dans ce
 *  cas, la partie interface matérielle doit être implémentée dans le fichier
 *  avrio-board-led.h se trouvant dans le même répertoire.
 *  @{
 *  @example output/led/demo_led.c
 *  Fait clignoter la LED1
 */
  /* inline public functions ================================================ */
/**
 * @brief Initialise les lignes de port utilisées par les leds
 */
static inline void vLedInit (void);

/**
 * @brief Allume une ou plusieurs leds
 *
 * L'état des leds dont les bits correspondant sont à 0, n'est pas modifié.
 * @param xMask Masque de sélection des leds
 */
static inline void vLedSet (xLedMask xMask);

/**
 * @brief Eteint une ou plusieurs leds
 *
 * L'état des leds dont les bits correspondant sont à 0, n'est pas modifié.
 * @param xMask Masque de sélection des leds
 */
static inline void vLedClear (xLedMask xMask);

/**
 * @brief Bascule une ou plusieurs leds (éteinte -> allumée / allumée -> éteinte)
 *
 * L'état des leds dont les bits correspondant sont à 0, n'est pas modifié.
 * @param xMask Masque de sélection des leds
 */
static inline void vLedToggle (xLedMask xMask);

#  if defined(__DOXYGEN__)
/*
 * __DOXYGEN__ defined
 * Partie documentation ne devant pas être compilée.
 * =============================================================================
 */

/* constants ================================================================ */
/**
 * @def LED_QUANTITY
 * @brief Nombre de leds disponible
 */
#    define LED_QUANTITY

/**
 * @def LED_LED1
 * @brief Masque de sélection pour la LED1
 * @note Un define par LED (LED2, LED3 ...)
 *
 * Doit être défini dans avrio-board-led.h pour chaque carte et pour chaque led.
 */
#    define LED_LED1

/**
 * @def LED_ALL_LEDS
 * @brief Masque de sélection pour toutes les leds
 *
 * Doit être défini dans avrio-board-led.h pour chaque carte.
 */
#    define LED_ALL_LEDS

/**
 * @def LED_NO_LED
 * @brief Masque de sélection pour aucune led
 *
 * Doit être défini dans avrio-board-led.h pour chaque carte.
 */
#    define LED_NO_LED

/* types ==================================================================== */
/**
 * @typedef xLedMask
 * @brief Type utilisé pour les masques de LED.
 *
 * Le type réel n'est pas forcément uint8_t mais dépend de la carte utilisée.\n
 * Il doit être défini dans avrio-board-led.h
 */
typedef uint8_t xLedMask;

/* internal public functions ================================================ */
/**
 * @brief Modifie l'état de toutes les leds
 *
 * Les leds sont le bit est à 1 sont allumées, les autres sont éteintes.
 * @param xMask Masque de sélection des leds
 */
static inline void vLedSetAll (xLedMask xMask);

/**
 * @brief Renvoie le masque associé à une led
 *
 * La macro LED_MASK_ARRAY_ENABLE doit être défini à la compilation pour
 * utiliser cette variable.\n
 *
 * @param ucLed Numéro de la led (1 pour LED_LED1, etc...)
 * @return le masque avec le bit associé à la led ucLed mis à 1
 */
static inline xLedMask xLedGetMask (uint8_t ucLed);

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
#    ifdef LED_MASK_ARRAY_ENABLE
extern const xLedMask xLedMaskArray[LED_QUANTITY];

static inline xLedMask
xLedGetMask (uint8_t ucLed) {

  return xLedMaskArray[ucLed];
}
#    endif

#  endif /* __DOXYGEN__ not defined */
# endif /* AVRIO_LED_ENABLE defined */
/* ========================================================================== */
__END_C_DECLS
#endif /* _AVRIO_LED_H_ */
