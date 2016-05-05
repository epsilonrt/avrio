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
 * @file pid.h
 * @brief Filtrage PID
 *
 * @todo Documentation du module
 */
#ifndef _AVRIO_PID_H_
#  define _AVRIO_PID_H_

#  include <avrio/defs.h>

/* *INDENT-OFF* */
__BEGIN_C_DECLS
  /* ======================================================================== */
/**
 * @addtogroup robotic_group
 * @{
 *
 *  @defgroup pid_module Filtrage PID
 *
 * @htmlonly
 *              <center><h2>&lt;ATTENTION&gt;</h2>
 *      <p><b>Module en développement, non fonctionnel !</b></p></center>
 * @endhtmlonly
 *  @{
 */
  /* macros ================================================================= */
  /* constants ============================================================== */
#  define SCALING_FACTOR  128
  /* types ================================================================== */
  /* structures ============================================================= */
/**
 * @brief Structure contenant l'état du filtre PID
 *
 *    Erreur: e = consigne - mesure
 *    Facteur d'échelle: SK = SCALING_FACTOR (128)
 *    de = e - e_précédente
 *    Se = somme des erreurs
 *  alors u, consigne filtrée:
 *    u = Kp.e + Kd.de/dt + Ki.Se.dt
 *  On n'en déduit:
 *    fKp = Kp.SK
 *    fKd = Kd.SK/dt
 *    fKi = Ki.SK.dt
 */
   typedef struct xPidData {

  int32_t iLastProcessValue; /**< Dernière valeur mesurée, afin de calculer la dérivée */
  int32_t iSumError; /**< Somme de toutes les erreurs, utilisé pour le calcul de l'intégrale */
  float fKp; /**< Coefficient du terme proportionnel: fKp = 128.Kp */
  float fKi; /**< Coefficient du terme intégral: fKi = 128.Ki.dt */
  float fKd; /**< Coefficient du terme dérivé: fKd = 128.Kd/dt */
} xPidData;

/* internal public functions ================================================ */
/**
 *  @brief Initialise le filtre PID
 *  @param fKp  Coefficient du terme proportionnel (fKp=128.Kp)
 *  @param fKi  Coefficient du terme intégral (fKi=128.Ki=128.Kp.dT/Ti)
 *  @param fKd  Coefficient du terme dérivé (fKd=128.Kd=128.Kp.Td/dT)
 *  @param xPid Filtre à initialiser
 *  \note dT est la période d'appel de la fonction iPidProcess()
 */
void vPidInit (float fKp, float fKi, float fKd, xPidData * xPid);

/**
 *  @brief Calcul PID
 *  @param consigne Valeur de consigne  Desired value.
 *  @param mesure   Valeur mesuréeMeasured value.
 *  @param xPid      Filtre à utiliser
 *  @return la valeur filtrée
 */
int32_t iPidProcess (int32_t iSetPoint, int32_t mesure, xPidData * xPid);

/**
 *  @brief Remise à zéro de la somme des erreurs
 */
void vPidResetIntegrator (xPidData * xPid);

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
#endif /* _AVRIO_PID_H_ */
