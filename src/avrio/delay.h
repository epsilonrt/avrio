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
 * @file delay.h
 * @brief Fonctions de temporisation
 */
#ifndef _AVRIO_DELAY_H_
#  define _AVRIO_DELAY_H_

#  include <avrio/defs.h>

/* *INDENT-OFF* */
__BEGIN_C_DECLS
  /* ======================================================================== */
/**
 * @addtogroup sys_group
 * @{
 *
 *  @defgroup delay_module Fonctions de temporisation
 *
 *  Ce module fournit des fonctions de temporisation qui permettent d'attendre
 *  un temps déterminé en millisecondes ou en microsecondes.\n
 *  @{
 */
#  if defined(__DOXYGEN__)
   /*
    * __DOXYGEN__ defined
    * Partie documentation ne devant pas être compilée.
    * =============================================================================
    */
  /* internal public functions ============================================== */
/**
 * @brief Temporisation en millisecondes
 *
 * Si la macro AVRIO_TASK_ENABLE n'est pas définie à la compilation,
 * l'implémentation utilise des fonctions en lignes qui utilisent des boucles.
 * Les calculs de temps sont effectués par le préprocesseur si une optimisation
 * est enclenchée.\n
 * Dans le cas où la macro AVRIO_TASK_ENABLE est définie, la temporisation est
 * effectuée par la routine d'inrerruption du timer utilisé par le module
 * \ref task_module ou \ref avrx_module
 *
 * @param usMs délai de temporisation en millisecondes
 */
void vDelayWaitMs (time_t usMs);

/**
 * @def delay_ms(__ms)
 * @brief Temporisation en millisecondes
 *
 * Synonyme de vDelayWaitMs() pour la compatibilité avec d'autres
 * bibliothèques.
 * @param __ms nombre de millisecondes (paramètre constant obligatoirement)
 */
#define delay_ms(__ms) vDelayWaitMs(__ms)

/**
 * @def delay_us(__us)
 * @brief Temporisation en microsecondes
 *
 * La temporisation s'effectue par des boucles. Cette fonction doit être
 * utilisée pour des petits délais et avec prudence dans une contexte
 * "temps réel".
 * @param __us nombre de microsecondes (paramètre constant obligatoirement)
 */
#define delay_us(__us)

/**
 * @brief Convertit une valeur de temps (millisecondes) en ticks
 */
ticks_t xDelayMsToTicks (time_t xTimeMs);

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
#    include "avrio-config.h"

#    ifdef AVRIO_TASK_ENABLE
#     include "avrio-board-kernel.h"

  __STATIC_ALWAYS_INLINE (ticks_t
  xDelayMsToTicks (time_t xTimeMs)) {

    return xTimeMs * AVRIO_KERNEL_TICK_RATE / 1000.0;
  }

#    endif

#    include <util/delay.h>

#    ifndef AVRIO_FAST_SIM
#      define delay_us(__us) _delay_us(__us)
#      define delay_ms(__ms) vDelayWaitMs(__ms)
#    else
#      define delay_us(__us)
#      define delay_ms(__ms)
#    endif


# if !defined(AVRIO_AVRX_ENABLE) && defined(AVRIO_TASK_ENABLE)

  /* Fonction défini dans les modules avrx ou task */
  void vDelayWaitMs (time_t usMs);

#    else /* !defined(AVRIO_AVRX_ENABLE) && defined(AVRIO_TASK_ENABLE) */

  /* ======================================================================== */

  // ---------------------------------------------------------------------------
  __STATIC_ALWAYS_INLINE (void
  vDelayWaitMs (time_t ms)) {

   while (ms--) {

#   if defined(__HAS_DELAY_CYCLES) && defined(__OPTIMIZE__) && !defined(__DELAY_BACKWARD_COMPATIBLE__)
      extern void __builtin_avr_delay_cycles (unsigned long);

      __builtin_avr_delay_cycles (AVRIO_CPU_FREQ / 1000);
#   else
      _delay_loop_2 (AVRIO_CPU_FREQ / (4.0 * 1000.0));
#   endif

    }
  }

#    endif /* !defined(AVRIO_AVRX_ENABLE) && defined(AVRIO_TASK_ENABLE) */
/* ========================================================================== */

#  endif /* __DOXYGEN__ not defined */
/* ========================================================================== */
__END_C_DECLS
/* *INDENT-ON* */
#endif /* _AVRIO_DELAY_H_ */
