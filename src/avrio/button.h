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
 * @file button.h
 * @brief Gestion des boutons poussoirs.
 */
#ifndef _AVRIO_BUTTON_H_
#define _AVRIO_BUTTON_H_

#include <avrio/defs.h>

/* *INDENT-OFF* */
__BEGIN_C_DECLS
  /* ======================================================================== */
#include "avrio-config.h"
#ifdef AVRIO_BUTTON_ENABLE
#include "avrio-board-button.h"
/**
 * @addtogroup dev_group
 * @{
 *
 * @defgroup button_module Boutons poussoirs
 *
 * Ce module permet de gérer les boutons pousoirs de façons indépendantes de
 * la plate-forme matérielle.\n
 * La macro AVRIO_BUTTON_ENABLE doit être définie dans avrio-config.h et dans ce
 * cas, la partie interface matérielle doit être implémentée dans le fichier
 * avrio-board-button.h se trouvant dans le même répertoire.
 * @{
 * @example button/demo_button.c
 * Un appui sur le bouton BUTTON1 allume la LED1. Un nouvel appui l'éteind.
 * Si on reste appuyé sur BUTTON1, la LED1 clignote.
 */
  /* internal public functions ============================================== */
/**
 * @brief Initialise les boutons poussoirs
 */
void vButInit (void);

/**
 * @brief Lecture d'un ou plusieurs boutons poussoirs
 *
 * Cette fonction effectue une lecture avec contrôle anti-rebond
 * @param xMask masque des boutons à tester
 * @return masque des boutons activés (1 si activé)
 */
xButMask xButGet (xButMask xMask);

#if defined(__DOXYGEN__)
/*
 * __DOXYGEN__ defined
 * Partie documentation ne devant pas être compilée.
 * =============================================================================
 */

/* constants ================================================================ */
/**
 * @def BUTTON_QUANTITY
 * @brief Nombre de boutons poussoirs disponibles
 *
 * Doit être défini dans avrio-board-button.h pour chaque carte.
 */
#define BUTTON_QUANTITY

/**
 * @def BUTTON_BUTTON1
 * @brief Masque de sélection pour le BUTTON1
 *
 * Doit être défini dans avrio-board-button.h pour chaque carte.
 *
 * @note Un define par BUTTON (BUTTON2, BUTTON3 ...)
 */
#define BUTTON_BUTTON1

/**
 * @def BUTTON_ALL_BUTTONS
 * @brief Masque de sélection pour tous les boutons poussoirs
 *
 * Doit être défini dans avrio-board-button.h pour chaque carte.
 */
#define BUTTON_ALL_BUTTONS

/**
 * @def BUTTON_NO_BUTTON
 * @brief Masque de sélection pour aucun bouton poussoir
 *
 * Doit être défini dans avrio-board-button.h pour chaque carte.
 */
#define BUTTON_NO_BUTTON

/* types ==================================================================== */
/**
 * @typedef xButMask
 * @brief Type utilisé pour les masques de bouton poussoir
 *
 * Doit être défini dans avrio-board-button.h pour chaque carte.
 */
typedef xButMask;

/* internal public functions ================================================ */
/**
 * @brief Renvoie le masque associé à un bouton poussoir
 *
 * La macro BUTTON_MASK_ARRAY_ENABLE doit être défini à la compilation pour
 * utiliser cette variable.\n
 *
 * @param ucBut Numéro du bouton poussoir (1 pour BUTTON_BUTTON1, etc...)
 * @return le masque avec le bit associé au bouton ucBut mis à 1
 */
static inline xButMask xButGetMask (uint8_t ucBut);

  /**
   *   @}
   * @}
   */
#else
/*
 * __DOXYGEN__ not defined
 * Partie ne devant pas être documentée.
 * =============================================================================
 */

#ifdef BUTTON_MASK_ARRAY_ENABLE
extern const xButMask xButMaskArray[BUTTON_QUANTITY];

static inline xButMask
xButGetMask (uint8_t ucBut) {

  return xButMaskArray[ucBut];
}
#endif

#endif /* __DOXYGEN__ not defined */
#else /* AVRIO_BUTTON_ENABLE not defined */
#warning "AVRIO_BUTTON_ENABLE undefined, using a bogus implementation !"
typedef uint8_t xButMask;
#define BUTTON_QUANTITY  1
#define BUTTON_BUTTON1 1
#define BUTTON_ALL_BUTTONS (BUTTON_BUTTON1)
#define BUTTON_NO_BUTTON (0)
#define vButInit()
#define xButGet(m) (xButMask)(0)
/* ========================================================================== */
__END_C_DECLS
/* *INDENT-ON* */
#endif /* AVRIO_BUTTON_ENABLE defined */
#endif /* _AVRIO_BUTTON_H_ */
