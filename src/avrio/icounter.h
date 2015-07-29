/**
 * @file icounter.h
 * @brief Comptage des signaux d'interruption
 *
 * Copyright © 2015 Pascal JEAN aka epsilonRT. All rights reserved.
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
 */
#ifndef _AVRIO_ICOUNTER_H_
#define _AVRIO_ICOUNTER_H_
#include <avrio/task.h>
#include <avrio/irq.h>
#include <avrio/mutex.h>

__BEGIN_C_DECLS
/* ========================================================================== */

/**
 * @addtogroup dev_group
 * @{
 *
 *  @defgroup icounter_module Comptage de signaux d'interruption
 *
 *  Ce module permet de compter le nombre de signaux d'interruption sur une
 *  durée donnée (fenêtre temporelle). Il permet d'en déterminer la fréquence.
 *  Il utilise le module Task pour générer la fenêtre de mesure.
 *  @{
 */

/* constants ================================================================ */
/**
 * @brief Mode de fonctionnement d'un compteur
 */
typedef enum eICounterMode {
  eICounterSingle      = 0, /**< Une mesure à la fois */
  eICounterFreeRunning = 1, /**< Mesure permanente*/
} eICounterMode;

/* structures =============================================================== */

/**
 * @brief Compteur
 *
 * La structure est opaque pour l'utilisateur
 */
struct xICounter;

/* types ==================================================================== */

/* internal public functions ================================================ */
/**
 * @brief Initialisation d'un compteur
 *
 * @param c pointeur sur le compteur
 * @param i numéro de l'interruption (INT0, INT1 ....)
 */
void vICounterInit (struct xICounter * c, xIrqHandle i);

/**
 * @brief Démarre la mesure
 *
 * @param c pointeur sur le compteur
 */
void vICounterStart (struct xICounter * c);

/**
 * @brief Modifie le mode de fonctionnement
 *
 * @param c pointeur sur le compteur
 * @param m mode de fonctionnement
 */
void vICounterSetMode (struct xICounter * c, eICounterMode m);

/**
 * @brief Modifie la durée de la fenêtre de mesure
 *
 * @param c pointeur sur le compteur
 * @param usWindowMs durée en ms de la fenêtre de mesure
 */
void vICounterSetWindow (struct xICounter * c, uint16_t usWindowMs);

/**
 * @brief Teste si la mesure est terminée
 *
 * @param c pointeur sur le compteur
 * @return true si la mesure est terminée
 */
bool bICounterIsComplete (struct xICounter * c);

/**
 * @brief Attends que la mesure se termine
 *
 * @param c pointeur sur le compteur
 */
void vICounterWaitForComplete (struct xICounter * c);

/**
 * @brief Dernière fréqunence
 *
 * @param c pointeur sur le compteur
 * @return fréquence en Hertz
 */
double dICounterFreq (struct xICounter * c);

/**
 * @brief Dernier comptage
 *
 * @param c pointeur sur le compteur
 * @return Nombre d'inpulsion sur la fenêtre
 */
uint16_t usICounterCount (struct xICounter * c);

#if defined(__DOXYGEN__)
/*
 * __DOXYGEN__ defined
 * Partie documentation ne devant pas être compilée.
 * =============================================================================
 */
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
typedef struct xICounter {
  xTaskHandle xTask;      /**< Tâche gérant la fenêtre de mesure */
  xMutex xReady;          /**< Mutex indiquant la fin de la mesure */
  xIrqHandle xInt;        /**< Numéro de la broche d'interruption */
  eICounterMode eMode;   /**< Mode de fonctionnement */
  volatile uint16_t usCounter; /**< Compteur d'impulsions */
  uint16_t usLastValue;   /**< Dernier décompte d'impulsions */
  uint16_t usWindow;      /**< Largeur de la fenêtre en ms */
} xICounter;
#endif /* __DOXYGEN__ not defined */

/* ========================================================================== */
__END_C_DECLS
#endif /* _AVRIO_ICOUNTER_H_ */
