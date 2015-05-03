/**
 * Copyright © 2011-2015 Pascal JEAN aka epsilonRT. All rights reserved.
 *
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
 *
 * @file switch.h
 * @brief Gestion des micro-interrupteurs.
 */
#ifndef _AVRIO_SWITCH_H_
#define _AVRIO_SWITCH_H_

#include <avrio/defs.h>

/* *INDENT-OFF* */
__BEGIN_C_DECLS
/* ========================================================================== */
#  include "avrio-config.h"
#  ifdef AVRIO_SWITCH_ENABLE
#   include "avrio-board-switch.h"
/**
 * @addtogroup dev_group
 * @{
 *
 * @defgroup switch_module Micro-interrupteurs
 *
 * Ce module permet de gérer les micro-interrupteurs  de façons indépendantes de
 * la plate-forme matérielle.\n
 * La macro AVRIO_SWITCH_ENABLE doit être définie dans avrio-config.h et dans ce
 * cas, la partie interface matérielle doit être implémentée dans le fichier
 * avrio-board-switch.h se trouvant dans le même répertoire.
 * @{
 * @example switch/demo_switch.c
 * Lit les micro-interrupteurs et fait clignoter la led le nombre de fois
 * correspondant à la valeur s'y trouvant.
 */

/* internal public functions ================================================ */
/**
 * Initialise le module
 */
void vSwitchInit (void);

/**
 * Lecture des micro-interrupteurs
 */
xSwitch xSwitchGet (void);

#  if defined(__DOXYGEN__)
/*
 * __DOXYGEN__ defined
 * Partie documentation ne devant pas être compilée.
 * =============================================================================
 */

/* types ==================================================================== */
/**
 * @typedef xSwitch
 * @brief Type renvoyé pour la lecture des switchs
 *
 * Doit être défini dans avrio-board-switch.h pour chaque carte.
 */
typedef xSwitch;

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
#  else /* AVRIO_SWITCH_ENABLE not defined */
#warning "AVRIO_SWITCH_ENABLE undefined, using a bogus implementation !"
typedef uint8_t xSwitch;
#  define vSwitchInit()
#  define xSwitchGet() (xSwitch)(0)

#  endif /* AVRIO_SWITCH_ENABLE defined */
/* ========================================================================== */
__END_C_DECLS
/* *INDENT-ON* */
#endif /* _AVRIO_SWITCH_H_ */
