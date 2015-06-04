/**
 * @file tcapture.h
 * @brief Entrée de capture timer 16 bits (Implémentation)
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
#include <util/atomic.h>
#include "tcapture.h"


/* macros =================================================================== */
#ifdef AVRIO_TCAPTURE_TIMER1
#if defined(TIM1_CAPT_vect)
#define TCAPTURE1_vect TIM1_CAPT_vect
#elif defined(TIMER1_CAPT1_vect)
#define TCAPTURE1_vect TIMER1_CAPT1_vect
#elif defined(TIMER1_CAPT_vect)
#define TCAPTURE1_vect TIMER1_CAPT_vect
#else
#warning No TCAPTURE1 for this MCU, check AVRIO_TCAPTURE_TIMER1 !
#endif
#endif

#ifdef AVRIO_TCAPTURE_TIMER3
#if defined(TIMER3_CAPT_vect)
#define TCAPTURE3_vect TIMER3_CAPT_vect
#else
#warning No TCAPTURE3 for this MCU, check AVRIO_TCAPTURE_TIMER3 !
#endif
#endif

#ifdef AVRIO_TCAPTURE_TIMER4
#if defined(TIMER4_CAPT_vect)
#define TCAPTURE4_vect TIMER4_CAPT_vect
#else
#warning No TCAPTURE4 for this MCU, check AVRIO_TCAPTURE_TIMER4 !
#endif
#endif

#ifdef AVRIO_TCAPTURE_TIMER5
#if defined(TIMER5_CAPT_vect)
#define TCAPTURE5_vect TIMER5_CAPT_vect
#else
#warning No TCAPTURE5 for this MCU, check AVRIO_TCAPTURE_TIMER5 !
#endif
#endif

#ifndef AVRIO_TCAPTURE_FCLK
#define AVRIO_TCAPTURE_FCLK F_CPU
#endif

#if defined(TIMER5_CAPT_vect)
#define TCAPTURE_CONTEXT_SIZE 4
#elif defined(TIMER4_CAPT_vect)
#define TCAPTURE_CONTEXT_SIZE 3
#elif defined(TIMER3_CAPT_vect)
#define TCAPTURE_CONTEXT_SIZE 2
#else
#define TCAPTURE_CONTEXT_SIZE 1
#endif

/* structures =============================================================== */
typedef struct xTCaptureContext {
  uint16_t usLast;
  uint16_t usCount;
  eTCapturePrescaler ePrescaler;
  xMutex xReady;          /**< Mutex indiquant la fin de la mesure */
} xTCaptureContext;

/* public variables ========================================================= */
static xTCaptureContext xTCapture[TCAPTURE_CONTEXT_SIZE];
static const int iDiv[] = { -1, 1, 8, 64, 256, 1024, 1, 1};

/* private functions ======================================================== */

// -----------------------------------------------------------------------------
static void
vTCaptureEnable (xTCaptureHandle t) {

  switch (t) {
#if defined(TCAPTURE1_vect)
    case TCAPTURE1:
      TIFR1  |= _BV (ICF1);
      TCNT1 = 0;
      TIMSK1 |= _BV (ICIE1);
      break;
#endif
#if defined(TCAPTURE3_vect)
    case TCAPTURE3:
      TIFR3  |= _BV (ICF3);
      TCNT3 = 0;
      TIMSK3 |= _BV (ICIE3);
      break;
#endif
#if defined(TCAPTURE4_vect)
    case TCAPTURE4:
      TIFR4  |= _BV (ICF4);
      TCNT4 = 0;
      TIMSK4 |= _BV (ICIE4);
      break;
#endif
#if defined(TCAPTURE5_vect)
    case TCAPTURE5:
      TIFR5  |= _BV (ICF5);
      TCNT5 = 0;
      TIMSK5 |= _BV (ICIE5);
#endif
    default:
      break;
  }
}

// -----------------------------------------------------------------------------
static void
vTCaptureDisable (xTCaptureHandle t) {

  switch (t) {
#if defined(TCAPTURE1_vect)
    case TCAPTURE1:
      TIMSK1 &= ~_BV (ICIE1);
      break;
#endif
#if defined(TCAPTURE3_vect)
    case TCAPTURE3:
      TIMSK3 &= ~_BV (ICIE3);
      break;
#endif
#if defined(TCAPTURE4_vect)
    case TCAPTURE4:
      TIMSK4 &= ~_BV (ICIE4);
      break;
#endif
#if defined(TCAPTURE5_vect)
    case TCAPTURE5:
      TIMSK5 &= ~_BV (ICIE5);
#endif
    default:
      break;
  }
}

// -----------------------------------------------------------------------------
static void
vUpdateContext (xTCaptureHandle t, uint16_t usICR) {

  if (xTCapture[t].usCount) {

    xTCapture[t].usCount--;
  }
  else {

    xTCapture[t].usLast = usICR;
    vMutexUnlock (&xTCapture[t].xReady);
    vTCaptureDisable (t);
  }
}

/* ================= Routines d'interruptions =============================== */
#if defined(TCAPTURE1_vect)
ISR (TCAPTURE1_vect) {

  vUpdateContext (TCAPTURE1, ICR1);
  TCNT1 = 0;
}
#endif

#if defined(TCAPTURE3_vect)
ISR (TCAPTURE3_vect) {

  vUpdateContext (TCAPTURE3, ICR3);
  TCNT3 = 0;
}
#endif

#if defined(TCAPTURE4_vect)
ISR (TCAPTURE4_vect) {

  vUpdateContext (TCAPTURE4, ICR4);
  TCNT4 = 0;
}
#endif

#if defined(TCAPTURE5_vect)
ISR (TCAPTURE5_vect) {

  vUpdateContext (TCAPTURE5, ICR5);
  TCNT5 = 0;
}
#endif

/* internal public functions ================================================ */

// -----------------------------------------------------------------------------
void
vTCaptureInit (void) {

  for (xTCaptureHandle t = 0; t < TCAPTURE_CONTEXT_SIZE; t++) {
    xTCapture[t].xReady = MUTEX_INITIALIZER;
  }
}

// -----------------------------------------------------------------------------
void
vTCaptureSetNoiseFilter (xTCaptureHandle t, bool bEnable) {

  if (bEnable) {
    switch (t) {
#if defined(TCAPTURE1_vect)
      case TCAPTURE1:
        TCCR1B |= _BV (ICNC1);
        break;
#endif
#if defined(TCAPTURE3_vect)
      case TCAPTURE3:
        TCCR3B |= _BV (ICNC3);
        break;
#endif
#if defined(TCAPTURE4_vect)
      case TCAPTURE4:
        TCCR4B |= _BV (ICNC4);
        break;
#endif
#if defined(TCAPTURE5_vect)
      case TCAPTURE5:
        TCCR5B |= _BV (ICNC5);
#endif
      default:
        break;
    }
  }
  else {

    switch (t) {
#if defined(TCAPTURE1_vect)
      case TCAPTURE1:
        TCCR1B &= ~_BV (ICNC1);
        break;
#endif
#if defined(TCAPTURE3_vect)
      case TCAPTURE3:
        TCCR3B &= ~_BV (ICNC3);
        break;
#endif
#if defined(TCAPTURE4_vect)
      case TCAPTURE4:
        TCCR4B &= ~_BV (ICNC4);
        break;
#endif
#if defined(TCAPTURE5_vect)
      case TCAPTURE5:
        TCCR5B &= ~_BV (ICNC5);
#endif
      default:
        break;
    }
  }
}

// -----------------------------------------------------------------------------
void
vTCaptureSetEvent (xTCaptureHandle t, eTCaptureEvent eEvent) {

  switch (t) {
#if defined(TCAPTURE1_vect)
    case TCAPTURE1:
      TCCR1B &= ~_BV (ICES1);
      TCCR1B |= eEvent << ICES1;
      break;
#endif
#if defined(TCAPTURE3_vect)
    case TCAPTURE3:
      TCCR3B &= ~_BV (ICES3);
      TCCR3B |= eEvent << ICES3;
      break;
#endif
#if defined(TCAPTURE4_vect)
    case TCAPTURE4:
      TCCR4B &= ~_BV (ICES4);
      TCCR4B |= eEvent << ICES4;
      break;
#endif
#if defined(TCAPTURE5_vect)
    case TCAPTURE5:
      TCCR5B &= ~_BV (ICES5);
      TCCR5B |= _BV (ICES5);
#endif
    default:
      break;
  }
}

// -----------------------------------------------------------------------------
void
vTCaptureSetPrescaler (xTCaptureHandle t, eTCapturePrescaler ePrescaler) {

  switch (t) {
#if defined(TCAPTURE1_vect)
    case TCAPTURE1:
      TCCR1B &= ~ (_BV (CS12) | _BV (CS11) | _BV (CS10));
      TCCR1B |= ePrescaler & (_BV (CS12) | _BV (CS11) | _BV (CS10));
      break;
#endif
#if defined(TCAPTURE3_vect)
    case TCAPTURE3:
      TCCR3B &= ~ (_BV (CS32) | _BV (CS31) | _BV (CS30));
      TCCR3B |= ePrescaler & (_BV (CS32) | _BV (CS31) | _BV (CS30));
      break;
#endif
#if defined(TCAPTURE4_vect)
    case TCAPTURE4:
      TCCR4B &= ~ (_BV (CS42) | _BV (CS41) | _BV (CS40));
      TCCR4B |= ePrescaler & (_BV (CS42) | _BV (CS41) | _BV (CS40));
      break;
#endif
#if defined(TCAPTURE5_vect)
    case TCAPTURE5:
      TCCR5B &= ~ (_BV (CS52) | _BV (CS51) | _BV (CS50));
      TCCR5B |= ePrescaler & (_BV (CS52) | _BV (CS51) | _BV (CS50));
#endif
    default:
      return;
  }
  xTCapture[t].ePrescaler = ePrescaler;
}


// -----------------------------------------------------------------------------
uint16_t
usTCaptureValue (xTCaptureHandle t) {
  uint16_t  usValue;

  ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {
    usValue =  xTCapture[t].usLast;
  }
  return usValue;
}

// -----------------------------------------------------------------------------
void
vTCaptureStart (xTCaptureHandle t) {

  vMutexLock (&xTCapture[t].xReady);
  ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {

    xTCapture[t].usCount = 2;
    vTCaptureEnable (t);
  }
}

// -----------------------------------------------------------------------------
bool
bTCaptureIsComplete (xTCaptureHandle t) {

  if (xMutexTryLock (&xTCapture[t].xReady) == 0) {

    vMutexUnlock (&xTCapture[t].xReady);
    return true;
  }
  return false;
}

// -----------------------------------------------------------------------------
void
vTCaptureWaitForComplete (xTCaptureHandle t) {

  vMutexLock (&xTCapture[t].xReady);
  vMutexUnlock (&xTCapture[t].xReady);
}

// -----------------------------------------------------------------------------
double
dTCaptureValueToTime (xTCaptureHandle t, uint16_t usValue) {
  double dTickTime = (double) iDiv[xTCapture[t].ePrescaler] / (double) AVRIO_TCAPTURE_FCLK;

  return (double) usValue * dTickTime;
}

// -----------------------------------------------------------------------------
double
dTCaptureValueToFreq (xTCaptureHandle t, uint16_t usValue) {

  return 1. / dTCaptureValueToTime (t, usValue);
}


/* ========================================================================== */
