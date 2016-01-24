/**
 * Copyright © 2016 Pascal JEAN aka epsilonRT. All rights reserved.
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
 * @file servo.h
 * @brief Servo-moteurs
 */
#include <avrio/defs.h>

__BEGIN_C_DECLS
/* ========================================================================== */
/**
 * @addtogroup robotic_group
 * @{
 *
 *  @defgroup  servo_module Servo-moteurs
 *  @{
 */

/* macros =================================================================== */
/* constants ================================================================ */
/* structures =============================================================== */
/* types ==================================================================== */
/* private variables ======================================================== */
/* private functions ======================================================== */
/* public variables ========================================================= */
/* internal public functions ================================================ */
void vServoInit (void);
uint8_t ucServoChannels (void);
void vServoSetPulse (uint8_t ucChannel, uint16_t usPulseUs);
int iServoGet (uint8_t ucChannel);

// -----------------------------------------------------------------------------
#  if defined(__DOXYGEN__)
/* 
 * __DOXYGEN__ defined
 * Partie documentation ne devant pas être compilée.
 * =============================================================================
 */
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
#  endif /* __DOXYGEN__ not defined */

/* ========================================================================== */
__END_C_DECLS
