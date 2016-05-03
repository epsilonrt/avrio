/**
 * Copyright © 2011-2015 epsilonRT. All rights reserved.
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
 * @file state.h
 * @brief Machine d'états
 *
 * Revision History ------------------------------------------------------------
 *    20111002 - Initial version by epsilonRT
 */
#ifndef _AVRIO_STATE_H_
#define _AVRIO_STATE_H_
#include <avrio/defs.h>

__BEGIN_C_DECLS
/* *INDENT-OFF* */
/* ========================================================================== */
/**
 * @addtogroup sys_group
 * @{
 *
 *  @defgroup state_module Machines d'états
 *
 * Les machines d'état sont des compteurs atomiques partagés par
 * plusieurs threads. Les opérations de base sur les machines d'état sont :
 * - modifier le compteur de manière atomique.
 * - attendre que le compteur soit à une certaine valeur et lui en affecter une 
 * autre (tout cela de façon atomique).
 * .
 *  @{
 */

/**
 * @brief Le type xState est une variable atomique qui peut évoluer entre 0 et
 * 255.
 */
typedef volatile uint8_t xState;

/**
 * @brief vStateWait suspend le thread appelant jusqu'à ce que le compteur de
 * la machine d'état pointée par pxState valle {ucValue}.
 * @param pxState pointeur sur la machine d'état
 * @param ucValue valeur à attendre
 */
void vStateWait(xState * pxState, uint8_t ucValue);

/**
 * @brief vStateSwitch suspend le thread appelant jusqu'à ce que le compteur de
 * la machine d'état pointée par pxState valle {ucFrom}. Alors, le compteur est
 * affecté à la valeur {ucTo}.
 * @param pxState pointeur sur la machine d'état
 * @param ucFrom valeur à attendre
 * @param ucTo valeur à assigner
 */
void vStateSwitch(xState * pxState, uint8_t ucFrom, uint8_t ucTo);

/**
 * @brief Affecte la valeur d'une machine d'état de façon atomique
 * @param pxState pointeur sur la machine d'état
 * @param ucValue valeur à assigner
 */
void vStateSet(xState * pxState, uint8_t ucValue);

/**
 * @brief Lit la valeur d'une machine d'état de façon atomique
 * @param pxState pointeur sur la machine d'état à lire
 * @return la valeur lue.
 */
uint8_t xStateGet (xState * pxState);

/**
 * @brief xStateTrywait est une variante non  bloquante de \ref vStateWait().
 * @param pxState pointeur sur la machine d'état à attendre
 * @return 0 Si la machine d'état pointé par {pxState} vaut {ucValue}, -1 sinon
 * @note Cette fonction ne bloque jamais  et peut être utilisée de manière 
 * fiable dans un gestionnaire de signaux.
 */
int8_t xStateTrywait(xState * pxState, uint8_t ucValue);

/**
 * @brief xStateTryswitch est une variante non bloquante de \ref vStateSwitch().
 * @param pxState pointeur sur la machine d'état à attendre
 * @return 0 si machine d'état affectée, -1 sinon
 * @note Cette fonction ne bloque jamais  et  peut être utilisée de manière 
 * fiable dans un gestionnaire de signaux.
 */
int8_t xStateTryswitch(xState * pxState, uint8_t ucFrom, uint8_t ucTo);

/**
 *   @}
 * @}
 */
/* ========================================================================== */
/* *INDENT-ON* */
__END_C_DECLS
#endif /* _AVRIO_STATE_H_ */
