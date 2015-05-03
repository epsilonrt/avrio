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
