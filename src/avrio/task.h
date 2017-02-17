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
 * @file task.h
 * @brief Multitâche collaboratif
 */
#ifndef _AVRIO_KERNEL_H_
#define _AVRIO_KERNEL_H_

#include <util/atomic.h>
#include <avrio/delay.h>
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
 *  @example sys/task/demo_task.c
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
 * @brief Renvoie le nombre de ticks depuis le démarrage du système
 */
ticks_t xTaskSystemTime (void);

#if defined(__DOXYGEN__)
/*
 * __DOXYGEN__ defined
 * Partie documentation ne devant pas être compilée.
 * =============================================================================
 */

/**
 * @brief Lit la période d'une tâche
 */
ticks_t xTaskGetInterval (xTaskHandle xTask);

/**
 * @brief Convertit une valeur de temps (millisecondes) en ticks
 */
ticks_t xTaskConvertMs (time_t xTimeMs);

/**
 * @brief Convertit une valeur de ticks en millisecondes
 */
time_t xTaskConvertTicks (ticks_t xTicks);

/**
 * @brief Rembobine le décompte d'une tâche
 * @param xTask tâche à rembobiner
 */
inline void vTaskRewind (xTaskHandle xTask);

/**
 * @brief Modifie la variable utilisateur d'une tâche
 *
 * @param xTask tâche concernée
 * @param data pointeur sur la variable utilisateur
 */
inline void vTaskSetUserData (xTaskHandle xTask, void * data);

/**
 * @brief Renvoie la variable utilisateur d'une tâche
 *
 * @param xTask tâche concernée
 * @return la variable utilisateur d'une tâche
 */
inline void * pvTaskGetUserData (xTaskHandle xTask);

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

#define xTaskConvertMs(ms) xDelayMsToTicks(ms)
#define xTaskConvertTicks(t) xDelayTicksToMs(t)

/* structures =============================================================== */
typedef struct xTask {
  ticks_t xInterval;  /* période en ticks d'exécution de la fonction */
  ticks_t xValue; /* valeur courante en ticks de la tâche */
  xTaskFunction xFunction;  /* pointeur sur la fonction de la tâche */
  void * udata;
} xTask;

/* public variables ========================================================= */
extern volatile xTask pxTasks[];

/* inline public functions ================================================== */
INLINE void
vTaskRewind (xTaskHandle xTask) {

  vTaskStop (xTask);
  vTaskStart (xTask);
}

// -----------------------------------------------------------------------------
INLINE void
vTaskSetUserData (xTaskHandle i, void * data) {

  pxTasks[i].udata = data;
}

// -----------------------------------------------------------------------------
INLINE void *
pvTaskGetUserData (xTaskHandle i) {

  return pxTasks[i].udata;
}

// -----------------------------------------------------------------------------
INLINE ticks_t
xTaskGetInterval (xTaskHandle i) {

  return pxTasks[i].xInterval + 1;
}

#  endif /* __DOXYGEN__ not defined */
/* ========================================================================== */
__END_C_DECLS
#endif /* _AVRIO_KERNEL_H_ */
