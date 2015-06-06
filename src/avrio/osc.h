/**
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
 *
 * @file delay.h
 * @brief Fonctions de temporisation
 */
#ifndef _AVRIO_OSC_H_
#define _AVRIO_OSC_H_

#include <avrio/defs.h>
__BEGIN_C_DECLS
/* ======================================================================== */
/**
 * @addtogroup sys_group
 * @{
 *
 *  @defgroup osc_module Fonctions de réglage de l'oscillateur RC
 *
 *  Ce module fournit des fonctions permettant le réglage de l'oscillateur RC.
 *  @{
 */

/* constants ================================================================ */
#define AVRIO_OSCCAL 0xFF /**< Adresse par défaut de OSCCAL en EEPROM */

#if defined(__DOXYGEN__)
/*
 * __DOXYGEN__ defined
 * Partie documentation ne devant pas être compilée.
 * =============================================================================
 */
/* internal public functions ================================================ */
/**
 * @brief Réglage du registre OSCCAL
 */
inline void vOscCalibrate (uint8_t ucOscCal);

/**
 * @brief Réglage du registre OSCCAL avec une valeur stockée en EEPROM
 *
 * @param usEeAddr adresse en EEPROM de l'octet d'étalonnage stocké lors de
 * l'étalonnage.
 */
inline void vOscCalibrateFromEE (uint16_t usEeAddr);

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
#include <avr/io.h>
#include <avr/eeprom.h>

// ---------------------------------------------------------------------------
INLINE void
vOscCalibrate (uint8_t ucOscCal) {
  OSCCAL = ucOscCal;
}

// ---------------------------------------------------------------------------
INLINE void
vOscCalibrateFromEE (uint16_t usEeAddr) {
  uint8_t ucOscCal = eeprom_read_byte ((const uint8_t *)usEeAddr);
  if (ucOscCal != 0xFF) {
    vOscCalibrate (ucOscCal);
  }
}

#endif /* __DOXYGEN__ not defined */
/* ========================================================================== */
__END_C_DECLS
#endif /* _AVRIO_OSC_H_ */
