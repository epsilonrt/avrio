/**
 * @file freqmeter.h
 * @brief Mesure de fréquence d'un signal d'interruption
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
#ifndef _AVRIO_FREQ_METER_H_
#define _AVRIO_FREQ_METER_H_
#include <avrio/task.h>
#include <avrio/irq.h>
#include <avrio/mutex.h>

__BEGIN_C_DECLS
/* ========================================================================== */

/**
 * @addtogroup dev_group
 * @{
 *
 *  @defgroup freqirq_module Mesure de fréquence d'un signal d'interruption
 *
 *  Ce module permet de mesurer la fréquence d'un signal d'interruption. Il
 *  effectue un comptage des interruptions sur une fenêtre de mesure d'une durée
 *  prédéterminée.
 *  @{
 *  @example freqirq/demo_freqirq.c
 *  description
 */

/* constants ================================================================ */
/**
 * @brief Mode de fonctionnement d'un fréquencemètre
 */
typedef enum eFreqMeterMode {
  eFreqMeterSingle      = 0, /**< Une mesure à la fois */
  eFreqMeterFreeRunning = 1, /**< Mesure permanente*/
} eFreqMeterMode;

/* structures =============================================================== */

/**
 * @brief Fréquencemètre
 *
 * La structure est opaque pour l'utilisateur
 */
struct xFreqMeter;

/* types ==================================================================== */

/* internal public functions ================================================ */
/**
 * @brief Initialisation d'un fréquencemètre
 *
 * @param f pointeur sur le fréquencemètre
 * @param i numéro de l'interruption (INT0, INT1 ....)
 */
void vFreqMeterInit (struct xFreqMeter * f, xIrqHandle i);

/**
 * @brief Démarre la mesure
 *
 * @param f pointeur sur le fréquencemètre
 */
void vFreqMeterStart (struct xFreqMeter * f);

/**
 * @brief Modifie le mode de fonctionnement
 *
 * @param f pointeur sur le fréquencemètre
 * @param m mode de fonctionnement
 */
void vFreqMeterSetMode (struct xFreqMeter * f, eFreqMeterMode m);

/**
 * @brief Modifie la durée de la fenêtre de mesure
 *
 * @param f pointeur sur le fréquencemètre
 * @param usWindowMs durée en ms de la fenêtre de mesure
 */
void vFreqMeterSetWindow (struct xFreqMeter * f, uint16_t usWindowMs);

/**
 * @brief Teste si la mesure est terminée
 *
 * @param f pointeur sur le fréquencemètre
 * @return true si la mesure est terminée
 */
bool bFreqMeterIsComplete (struct xFreqMeter * f);

/**
 * @brief Attends que la mesure se termine
 *
 * @param f pointeur sur le fréquencemètre
 */
void vFreqMeterWaitForComplete (struct xFreqMeter * f);

/**
 * @brief Lit la dernière mesure
 *
 * @param f pointeur sur le fréquencemètre
 * @return fréquence en Hertz
 */
double dFreqMeterRead (struct xFreqMeter * f);

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
typedef struct xFreqMeter {
  xTaskHandle xTask;      /**< Tâche gérant la fenêtre de mesure */
  xMutex xReady;          /**< Mutex indiquant la fin de la mesure */
  xIrqHandle xInt;        /**< Numéro de la broche d'interruption */
  eFreqMeterMode eMode;   /**< Mode de fonctionnement */
  volatile uint16_t usCounter; /**< Compteur d'impulsions */
  uint16_t usLastValue;   /**< Dernier décompte d'impulsions */
  uint16_t usWindow;      /**< Largeur de la fenêtre en ms */
} xFreqMeter;
#endif /* __DOXYGEN__ not defined */

/* ========================================================================== */
__END_C_DECLS
#endif /* _AVRIO_FREQ_METER_H_ */
