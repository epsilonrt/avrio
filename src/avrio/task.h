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
 * @file task.h
 * @brief Multitâche collaboratif
 */
#ifndef _AVRIO_KERNEL_H_
#  define _AVRIO_KERNEL_H_

#  include <avrio/delay.h>

/* *INDENT-OFF* */
__BEGIN_C_DECLS
  /* ======================================================================== */
/**
 * @addtogroup sys_group
 * @{
 *
 *  @defgroup task_module Multitâche collaboratif
 *
 *  Ce module permet d'exécuter des fonctions de façon périodique.\n
 *  Cela revient à mettre en place une organisation dans le temps de plusieurs
 *  tâches. Le passage d'une tâche à l'autre est cadencé par une routine
 *  d'interruption déclenchée périodiquement par un timer matériel.\n
 *  La macro AVRIO_TASK_ENABLE doit être définie au moment de la compilation
 *  et dans ce cas, la partie interface matérielle doit être implémentée dans le
 *  fichier avrio-board-kernel.h .\n
 *  L'intervalle maximal de temps pouvant être géré dépend de la taille de
 *  ticks_t.
 *  Si AVRIO_TIME_UINT32 n'est pas défini à la compilation, ticks_t est codé
 *  sur 16 bits soit une valeur max de 65535 ticks. Pour un tick de 1 ms cela ne
 *  fait qu'un peut plus d'une minute !
 *  Dans le cas contraire, ticks_t est codé sur 32 bits, soit une valeur max de
 *  4096 Mi ticks soit un peu plus de 1193 heures !
 *  @{
 *  @example task/demo_task.c
 *  Utilise une tâche pour générer un train d'impulsion sur la LED1.
 */
  /* constants ============================================================== */
/**
 * @brief Indique une erreur
 */
#  define AVRIO_KERNEL_ERROR (-1)
  /* types ================================================================== */
/**
 * @brief xTaskHandle Identifiant d'une tâche
 */
typedef int8_t xTaskHandle;

/**
 * @brief La fonction d'une tâche ne renvoit rien et reçoit son identifiant.
 */
typedef void (*xTaskFunction) (xTaskHandle);

/* internal public functions ================================================ */
/**
 * @brief Ajout d'une tâche
 * @param xIntervalTicks Période de la tâche en ticks
 * @param xFunction  Fonction à exécuter à chaque période écoulée
 * @return L'indentifiant de la tâche. AVRIO_KERNEL_ERROR si échec.
 * @note Cette fonction ajoute une tâche à la liste des tâches actives.\n
 * Pour que la tâche fonctionne, il faut ensuite la valider avec vTaskStart().
 */
xTaskHandle xTaskCreate (ticks_t xIntervalTicks, xTaskFunction xFunction);

/**
 * @brief Modifie la période d'une tâche
 */
void vTaskSetInterval (xTaskHandle xTask, ticks_t xIntervalTicks);

/**
 * @brief Démarre une tâche
 * @param xTask tâche à démarrer
 * @note La tâche est démarrée, sa fonction de service est executée lorsque
 * interval_ms est écoulée (mais il n'est pas redémarré)
 */
void vTaskStart (xTaskHandle xTask);

/**
 * @brief Arrête une tâche
 * @param xTask tâche à arrêter
 * @note La tâche est arrêtée, sa fonction de service n'est plus executée
 */
void vTaskStop (xTaskHandle xTask);

/**
 * @brief Indique si une tâche est lancée
 * @param xTask tâche à tester
 * @return true si la tâche est lancée
 */
bool xTaskIsStarted (xTaskHandle xTask);

/**
 * @brief Convertit une valeur de temps (millisecondes) en ticks
 */
ticks_t xTaskConvertMs (time_t xTimeMs);

/**
 * @brief Renvoie le nombre de ticks depuis le démarrage du système
 */
ticks_t xTaskSystemTime (void);

#  if defined(__DOXYGEN__)
/*
 * __DOXYGEN__ defined
 * Partie documentation ne devant pas être compilée.
 * =============================================================================
 */

  /**
   * @brief Rembobine le décompte d'une tâche
   * @param xTask tâche à rembobiner
   */
  inline void vTaskRewind (xTaskHandle xTask);

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
  __STATIC_ALWAYS_INLINE (void
    vTaskRewind (xTaskHandle xTask)) {

    vTaskStop(xTask);
    vTaskStart(xTask);
  }
#  endif /* __DOXYGEN__ not defined */
/* ========================================================================== */
__END_C_DECLS
/* *INDENT-ON* */
#endif /* _AVRIO_KERNEL_H_ */
