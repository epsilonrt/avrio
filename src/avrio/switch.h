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
 * @file switch.h
 * @brief Gestion des micro-interrupteurs.
 */
#ifndef _AVRIO_SWITCH_H_
#define _AVRIO_SWITCH_H_

#include <avrio/defs.h>

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
 * @example input/switch/demo_switch.c
 * Demo micro-switch (switch)
 * Ce programme affiche l'état des interrupteurs en hexadécimal sur la liaison 
 * série.
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
#endif /* _AVRIO_SWITCH_H_ */
