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
 * @file timer.h
 * @brief Driver Timer
 *
 * Revision History ------------------------------------------------------------
 *    20120301 - Initial version by epsilonRT
 */
#include "itypes.h"

/* public variables ========================================================= */
/*
  Les variables ci-dessous doivent être incrémentées à chaque milliseconde
  dans une routine d'interruption.
 */
extern U8 gu8MsTimer1;
extern U8 gu8MsTimer2;
extern U8 gu8MsClock;

/* internal public functions ================================================ */
void TimerInit (void);
void TimerDelayUsec (U16 Us);
void TimerDelayMsec (U16 Ms);

/* ========================================================================== */
