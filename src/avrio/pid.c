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
 * @file pid.c
 * @brief Filtrage PID
 */
#include "pid.h"
#include "../config.h"

/* 
 * Implémentation filtre PID basée sur la note d'application d'ATMEL
 * AVR221 - Discrete PID controller
 */

/* config =================================================================== */
#define SCALING_FACTOR 128

/* constants ================================================================ */
#define MAX_INT         INT16_MAX
#define MAX_LONG        INT32_MAX
#define MAX_I_TERM      (MAX_LONG / 2)

/* private functions ======================================================== */

/* internal public functions ================================================ */
/* 
 *  Initialise le filtre PID
 *  @param fKp  Coefficient du terme proportionnel (fKp=128.Kp)
 *  @param fKi  Coefficient du terme intégral (fKi=128.Ki=128.Kp.T/Ti)
 *  @param fKd  Coefficient du terme dérivé (fKd=128.Kd=128.Kp.Td/T)
 *  @param pid Filtre à initialiser
 *  \note T est la période d'appel de la fonction iPidProcess()
 */
void
vPidInit (float fKp, float fKi, float fKd, xPidData * pid) {

  pid->iSumError = 0;
  pid->iLastProcessValue = 0;
  pid->fKp = fKp;
  pid->fKi = fKi;
  pid->fKd = fKd;
}

/* 
 *  Calcul PID
 *  @param consigne Valeur de consigne  Desired value.
 *  @param mesure   Valeur mesuréeMeasured value.
 *  @param pid      Filtre à utiliser
 *  @return la valeur filtrée
 */
int32_t
iPidProcess (int32_t consigne, int32_t mesure, xPidData * pid) {
  int32_t erreur, sortie;
  int32_t terme_p, terme_d, terme_i;

  erreur = consigne - mesure;
  pid->iSumError += erreur;

  terme_p = (int32_t) (pid->fKp * (float) erreur);
  terme_i = (int32_t) (pid->fKi * (float) pid->iSumError);
  terme_d = (int32_t) (pid->fKd * (float) (pid->iLastProcessValue - mesure));
  pid->iLastProcessValue = mesure;
  sortie = (terme_p + terme_i + terme_d);

  return sortie;
}

/* 
 *  Remise à zéro de la somme des erreurs
 */
void
vPidResetIntegrator (xPidData * pid) {

  pid->iSumError = 0;
}

/* ========================================================================== */
