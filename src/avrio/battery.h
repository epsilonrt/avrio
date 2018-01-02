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
 * @file battery.h
 * @brief Chargeur de batterie
 *
 *
 * Revision History ------------------------------------------------------------
 *    20120511 - Initial version by epsilonRT
 */
#ifndef _AVRIO_BATTERY_H_
#define _AVRIO_BATTERY_H_

#include <avrio/defs.h>
#include <avrio/adc.h>

#if defined(__cplusplus)
  extern "C" {
#endif
/* ========================================================================== */
/**
 * @addtogroup dev_group
 * @{
 *
 *  @defgroup battery_module Chargeur de batterie NiCd/NiMh
 *  Ce module réalise un chargeur de batterie NiCd/NiMh, et utilise le module
 *  \ref adc_module pour mesurer la tension et \ref task_module pour cadencer
 *  le processus de charge. \n
 *  L'utilisateur doit implémenter lui même la partie générateur de charge
 *  dans avrio-board-battery.h
 *  @{
 * @example battery/demo_battery.c
 * Exemple qui effectue la charge d'un pack de 4 accus NiMH et qui
 * affiche les informations de suivi du processus
 */

/* constants ================================================================ */
/**
 * @enum eBatCharge
 * @brief Différents mode du générateur de charge
 */
typedef enum {
  BAT_CHARGE_OFF = 0, /**< Générateur arrêté */
  BAT_CHARGE_TRICKLE = 1, /**< Charge d'entretien */
  BAT_CHARGE_TOPOFF = 2,  /**< Charge de terminaison */
  BAT_CHARGE_FAST = 3 /**< Charge rapide */
} eBatCharge;

/**
 * @enum eBatState
 * @brief Différentes étapes du processus de charge
 */
typedef enum {

  BAT_STATE_LOW,    /**< Générateur non alimenté, batterie faible */
  BAT_STATE_IDLE,   /**< Générateur non alimenté, mode attente */
  BAT_STATE_PUTON,  /**< Générateur sous tension, début du processus */
  BAT_STATE_PRECHARGE,  /**< Phase précharge (cas d'une batterie neuve) */
  BAT_STATE_FAST, /**< Phase charge rapide */
  BAT_STATE_TOPOFF, /**< Phase charge de terminaison */
  BAT_STATE_TRICKLE,/**< Phase charge d'entretien */
  BAT_STATE_ERROR/**< Erreur batterie */
} eBatState;

/* internal public functions ================================================ */
/**
 * @brief Initialisation du module
 */
void vBatInit (void);

/**
 * @brief Fonction de traitement du processus
 *
 * Cette fonction doit être appelée le plus souvent possible par le programme
 * principal et gère l'ensemble du processus.
 * @return Etat du processus de charge
 */
eBatState eBatProcess (void);

/**
 * @brief Lecture de la tension batterie moyenne mesurée par le processus
 * @return Valeur de la tension moyenne en LSB (pleine échelle à 1024
 * correspondant à BAT_SCALE dans avrio-board-battery.h
 */
uint16_t usBatVoltageAverage (void);

/**
 * @brief Lecture de la tension batterie maximale mesurée par le processus
 * @return Valeur de la tension maximale en LSB (pleine échelle à 1024
 * correspondant à BAT_SCALE dans avrio-board-battery.h
 */
uint16_t usBatVoltageMax (void);

/**
 * @brief Effectue une mesure de la tension d'un paramètre
 * @param ucChannel Canal de l'ADC à mesurer
 * @return Valeur de la tension en LSB (pleine échelle à 1024
 * correspondant à BAT_SCALE dans avrio-board-battery.h
 */
uint16_t usBatVoltage (uint8_t ucChannel);

/**
 * @brief Lecture de l'état du générateur
 * @return Etat du générateur
 * @note Doit être implémentée par l'utilisateur 
 */
static inline eBatCharge eBatChargeGet (void);

/**
 * @brief Modification de l'état du générateur
 * @param eCharge 
 * @warning Cette fonction ne doit pas être utilisée directement car elle 
 * court-circuite le processus de surveillance.
 * @note Doit être implémentée par l'utilisateur 
 */
static inline void vBatChargeSet (eBatCharge eCharge);

/**
 * @brief Initialisation du générateur
 * @warning Cette fonction ne doit pas être utilisée directement car elle 
 * court-circuite le processus de surveillance.
 * @note Doit être implémentée par l'utilisateur 
 */
static inline void vBatChargeInit (void);

// -----------------------------------------------------------------------------
#  if defined(__DOXYGEN__)
/* 
 * __DOXYGEN__ defined
 * Partie documentation ne devant pas être compilée.
 * =============================================================================
 */
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
#  endif /* __DOXYGEN__ not defined */

/* ========================================================================== */
#if defined(__cplusplus)
  }
#endif
/* ========================================================================== */
#endif  /* _AVRIO_BATTERY_H_ not defined */
