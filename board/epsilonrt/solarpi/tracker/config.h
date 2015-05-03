/**
 * @file config.h
 * @brief Configuration
 * @author Copyright (c) 2013 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * Revision History ------------------------------------------------------------
 *    20131208 - Initial version by epsilonRT
 */
#ifndef _WHEREAVR_CONFIG_H_
#define _WHEREAVR_CONFIG_H_

#include <avrio/defs.h>
#include <avr/interrupt.h>

/* *INDENT-OFF* */
__BEGIN_C_DECLS
/* ========================================================================== */

/* constants ================================================================ */
#define TIMER0  0
#define TIMER1  1
#define TIMER2  2

/* configuration ============================================================ */
#define TONE_TIMER TIMER2
#define AX25_BAUDRATE 1200

//#define ALARM_DELAY_LOOP Serial_Processes
/* ========================================================================== */
__END_C_DECLS
/* *INDENT-ON* */
#endif  /* _WHEREAVR_CONFIG_H_ not defined */
