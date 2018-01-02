/*
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
 */
#ifndef _WHEREAVR_CONFIG_H_
#define _WHEREAVR_CONFIG_H_

#include <avrio/defs.h>
#include <avr/interrupt.h>

#if defined(__cplusplus)
  extern "C" {
#endif
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
#if defined(__cplusplus)
  }
#endif
#endif  /* _WHEREAVR_CONFIG_H_ not defined */
