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
 * @file gpio.h
 * @brief Port GPIO
 */
#ifndef _AVRIO_GPIO_H_
#define _AVRIO_GPIO_H_

#include <avrio/defs.h>
__BEGIN_C_DECLS
/* ========================================================================== */
#include <avrio/dpin.h>

/**
 * @addtogroup dev_group
 * @{
 *
 *  @defgroup gpio_module Port GPIO
 *  Ce module permet de gérer les broches numériques (binaires). \n
 *  La composition du port est décrite dans un fichier avrio-board-gpio.h
 *  spécifique à la carte utilisée.
 *  @{
 *  @example gpio/demo_gpio.c
 * Demo port GPIO
 * Ce programme fait clignoter la LED1 16 fois puis lit l'état du bouton 
 * poussoir et bascule l'état de la LED1 à chaque appui.
 */

/* constants ================================================================ */

/* types ==================================================================== */
/**
 * Variable pouvant contenir le masque
 */
typedef unsigned int uGpioMask;

/**
 * Gestionnaire de changement d'état d'une broche
 *
 * Déclenché sur modification de niveau d'une broche.
 * Seuls les fronts valides déclenchent un traitement.
 *
 * @param uPinValues valeur des broches (bit 0 pour broche 0, ...)
 * @param ucPin le numéro de l'broche qui a changée
 * @param edge  front déclenchant
 * @param udata pointeur sur les données utilisateur défini lors de l'appel à iGpioSetGrpCallback()
 */
typedef void (*vGpioCallback) (uGpioMask uPinValues, uint8_t ucPin, eDpEdge edge, void *udata);

/* internal public functions ================================================ */

/**
 * @brief Modifie l'état d'une sortie
 * 
 * @param ucPin le numéro de la broche
 * @param bValue valeur
 */
void vGpioWrite (uint8_t ucPin, bool bValue);

/**
 * @brief Bascule l'état d'une sortie
 *
 * @param ucPin le numéro de la broche
 */
void vGpioToggle (uint8_t ucPin);

/**
 * @brief Réglage du ou des front(s) déclenchant d'une entrée
 *
 * @param ucPin le numéro logique de l'entrée concernée
 * @param edge front déclenchant (eEdgeNone désactive la détection de front et
 *              l'éxécution du gestionnaire d'interruption)
 */
void vGpioSetPinEdge (uint8_t ucPin, eDpEdge edge);

/**
 * @brief Modification de l'état binaire de plusieurs sorties
 *
 * Cette fonction n'a aucune action sur les broches qui ne sont pas en sortie.
 *
 * @param uPinMask masque de sélection des broches à modifier, un bit à 1 indique
 *        que la broche correspondante doit être modifiée (bit 0 pour la
 *        broche 0, bit 1 pour la broche 1 ...). -1 pour modifier toutes les
 *        broches.
 * @param bValue valeur binaire à appliquer aux broches
 */
void vGpioWriteAll (uGpioMask uPinMask, bool bValue);

/**
 * @brief Bascule de l'état binaire de plusieurs sorties
 *
 * @param uPinMask masque de sélection des broches à modifier, un bit à 1 indique
 *        que la broche correspondante doit être modifiée (bit 0 pour la
 *        broche 0, bit 1 pour la broche 1 ...). -1 pour modifier toutes les
 *        broches.
 */
void vGpioToggleAll (uGpioMask uPinMask);

/**
 * @brief Lecture de l'état binaire d'un ensemble de broches
 *
 * La lecture est possible sur les entrées et les sorties.
 *
 * @param uPinMask masque de sélection des broches à lire, un bit à 1 indique
 *        que la broche correspondante doit être lue (bit 0 pour la
 *        broche 0, bit 1 pour la broche 1 ...). -1 pour lire toutes les
 *        broches.
 * @return l'état des broches, chaque bit correspond à l'état de la broche
 * correspondante (bit 0 pour la broche 0, bit 1 pour la broche 1 ...)
 */
uGpioMask uGpioReadAll (uGpioMask uPinMask);

/**
 * @brief Scrutation des broches
 *
 * Cette fonction scrute les broches qui ont un front de déclenchement différent
 * de eEdgeNone (modifié à l'aide de vGpioSetPinEdge()). \n
 * Si un front valide survient sur une de ses broches, la fonction définie à
 * l'aide de vGpioSetCallback() est appelée. \n
 * Cette fonction doit être appelée par la boucle principale le plus souvent
 * possible. Un traitement permet de supprimer les rebonds.
 */
void vGpioPoll (void);

#  if defined(__DOXYGEN__)
/*
 * __DOXYGEN__ defined
 * Partie documentation ne devant pas être compilée.
 * =============================================================================
 */
/**
 * @brief Modifie le mode de la broche
 * @param ucPin le numéro de la broche
 * @param eMode mode
 */
static inline void vGpioSetPinMode (uint8_t ucPin, eDpMode eMode);

/**
 * @brief Lecture du type actuel d'une broche
 *
 * @param ucPin le numéro de la broche
 * @param gpio pointeur sur le GPIO
 * @return le type, eModeError si erreur
 */
static inline eDpMode eGpioPinMode (uint8_t ucPin);

/**
 * @brief Front(s) déclenchant d'une entrée
 *
 * @param ucPin le numéro logique de l'entrée concernée
 * @return le ou les fronts déclenchants, eEdgeNone si erreur
 */
static inline eDpEdge eGpioPinEdge (uint8_t ucPin);

/**
 * @brief Modification du gestionnaire de changement d'état
 *
 * @param callback fonction définie par l'utilisateur, NULL pour désactiver
 */
static inline void vGpioSetCallback (vGpioCallback callback);

/**
 * @brief gestionnaire de changement d'état
 *
 * @return callback
 */
static inline vGpioCallback pvGpioCallback (void);

/**
 * @brief Données d'un gestionnaire d'interruption utilisateur
 *
 * @return pointeur sur les données, NULL si pas de données ou si erreur
 */
static inline void * pGpioUserData (void);

/**
 * @brief Modification données  utilisateur
 *
 * @param udata données passée au callback
 */
static inline void vGpioSetUserData (void * udata);

/**
 * @brief Lecture de l'état d'une broche (entrée ou sortie)
 *
 * @param ucPin le numéro de la broche
 * @return false ou true, si le mode de la broche est eModeInputPullUp ou
 * eModeOutputHigh la valeur booléenne renvoyée est l'inverse de l'état électrique.
 */
static inline bool bGpioRead (uint8_t ucPin);

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
#ifdef AVRIO_GPIO_ENABLE
#include "avrio-board-gpio.h"

/* structures =============================================================== */
struct xGpioContext {
  xDPin pin[GPIO_SIZE];
  vGpioCallback callback;
  uGpioMask mask;
  uGpioMask values;
  void * udata;
};

/* structures =============================================================== */
extern struct xGpioContext gpio;

/* internal public functions ================================================ */
// -----------------------------------------------------------------------------
INLINE uint8_t
ucGpioSize (void) {

  return GPIO_SIZE;
}

// -----------------------------------------------------------------------------
INLINE void
vGpioSetPinMode (uint8_t ucPin, eDpMode eMode) {

  vDpSetMode (&gpio.pin[ucPin], eMode);
}

// -----------------------------------------------------------------------------
INLINE eDpMode
eGpioPinMode (uint8_t ucPin) {

  return gpio.pin[ucPin].mode;
}

// -----------------------------------------------------------------------------
INLINE eDpEdge
eGpioPinEdge (uint8_t ucPin) {

  return gpio.pin[ucPin].edge;
}

// -----------------------------------------------------------------------------
INLINE bool
bGpioRead (uint8_t ucPin) {

  return bDpRead (&gpio.pin[ucPin]);
}

// -----------------------------------------------------------------------------
INLINE void
vGpioSetCallback (vGpioCallback callback) {

  gpio.callback = callback;
}

// -----------------------------------------------------------------------------
INLINE vGpioCallback
pvGpioCallback (void) {

  return gpio.callback;
}

// -----------------------------------------------------------------------------
INLINE void *
pvGpioUserData (void) {

  return gpio.udata;
}

// -----------------------------------------------------------------------------
INLINE void
vGpioSetUserData (void * udata) {

  gpio.udata = udata;
}

#endif /* AVRIO_GPIO_ENABLE defined */
#endif /* __DOXYGEN__ not defined */

/* ========================================================================== */
__END_C_DECLS
#endif /* _AVRIO_GPIO_H_ */
