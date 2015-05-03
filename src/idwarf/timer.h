/**
 * @file timer.h
 * @brief Driver Timer
 * @author Copyright © 2011-2012 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
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
