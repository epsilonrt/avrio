/**
 * @file freqirq.h
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
 *  Ce module permet de mesurer la fréquence d'un signal d'interruption
 *  @{
 *  @example freqirq/demo_freqirq.c
 *  description
 */

/* constants ================================================================ */
/**
 * @brief Type d'évement de déclenchement
 */
typedef enum eFreqMeterMode {
  eFreqMeterSingle      = 0, /**< Une mesure à la fois */
  eFreqMeterFreeRunning = 1, /**< Mesure permanente*/
} eFreqMeterMode;

/* structures =============================================================== */
/**
 * @brief Structure
 */
typedef struct xFreqMeter {
  xTaskHandle xTask;
  xMutex xReady;
  xIrqHandle xInt;
  eFreqMeterMode eMode;
  uint16_t usCounter;
  uint16_t usLastValue;
  uint16_t usWindow;
} xFreqMeter;

/* types ==================================================================== */

/* internal public functions ================================================ */
/**
 * @brief Attache une fonction à une broche d'interruption
 *
 * @param i numéro de l'interruption (INT0, INT1 ....)
 */
void vFreqMeterInit (xFreqMeter * f, xIrqHandle i);
void vFreqMeterStart (xFreqMeter * f);
void vFreqMeterSetMode (xFreqMeter * f, eFreqMeterMode m);
void vFreqMeterSetWindow (xFreqMeter * f, uint16_t usWindowMs);
bool bFreqMeterIsComplete (xFreqMeter * f);
void vFreqMeterWaitForComplete (xFreqMeter * f);
double dFreqMeterRead (xFreqMeter * f);

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
#endif /* __DOXYGEN__ not defined */

/* ========================================================================== */
__END_C_DECLS
#endif /* _AVRIO_FREQ_METER_H_ */
