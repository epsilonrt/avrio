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
 * @file bdcm.h
 * @brief Contrôle PWM d'un moteur DC à balai (Brushed DC Motor).
 */
#ifndef _AVRIO_BDCM_H_
#  define _AVRIO_BDCM_H_

#  include <avrio/defs.h>

/* *INDENT-OFF* */
__BEGIN_C_DECLS
  /* ======================================================================== */
/**
 * @addtogroup robotic_group
 * @{
 *
 *  @defgroup bdcm_module Contrôle PWM de moteurs à courant continu
 *
 *  Ce module permet de contrôler la vitesse et le sens d'un moteur à courant
 *  continu à balais.\n
 *  La macro AVRIO_BDCM_ENABLE doit être définie dans avrio-config.h et dans ce
 *  cas, la partie interface matérielle doit être implémentée dans le fichier
 *  avrio-board-bdcm.h se trouvant dans le même répertoire.
 *  @{
 */
  /* constants ============================================================== */
/**
 * @enum eBdcmControl
 * @brief Enumération des commandes d'un moteur
 *
 * Utilisée dans l'implémentation de la partie matérielle (avrio-board-bdcm.h)
 */
   typedef enum {

  BDCM_FREE,    /**< Roue libre (moteur non alimenté, "en l'air" */
  BDCM_FORWARD, /**< Marche avant */
  BDCM_REVERSE, /**< Marche arrière */
  BDCM_STOP     /**< Stop (moteur court-circuité) */
} eBdcmControl;

/* internal public functions ================================================ */
/**
 * @brief Initialisation du module
 * @param  Fréquence en Hertz du signal PWM
 */
void vBdcmInit (uint16_t usFreq);

/**
 * @brief Modifie le rapport cyclique du signal PWM d'un moteur
 * @param sRatio 0 pour r = 0, 10000 pour r = 100. Négatif pour marche arrière
 * @param ucMotor Numéro du moteur
 */
void vBdcmSetRatio (uint8_t ucMotor, int16_t sRatio);

/**
 * @brief Renvoie le rapport cyclique du signal PWM d'un moteur
 * @param ucMotor Numéro du moteur
 * @return 0 pour r = 0, 10000 pour r = 100. Négatif pour marche arrière
 */
int16_t sBdcmRatio (uint8_t ucMotor);

/**
 * @brief Fréquence PWM actuellement utilisée
 */
uint16_t usBdcmFreq (void);

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
__END_C_DECLS
/* *INDENT-ON* */
#endif /* _AVRIO_BDCM_H_ */
