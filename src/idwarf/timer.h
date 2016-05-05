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
