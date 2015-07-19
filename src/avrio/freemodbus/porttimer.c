/* Copyright _ 2009-2012 epsilonRT. All rights reserved.
 *  $Id$ */
/* ========================================================================== */
#include "avrio-config.h"

#ifdef AVRIO_MODBUS_ENABLE
/* ========================================================================== */
#  include "avrio-board-kernel.h"
#  include "avrio-board-modbus.h"

#ifndef MODBUS_USE_AVRIO_TASK
#define MODBUS_USE_AVRIO_TASK 1
#endif

/* ----------------------- AVR includes ------------------------------------- */
#  include <avr/io.h>
#  include <avr/interrupt.h>

/* ----------------------- Platform includes -------------------------------- */
#  include "port.h"
#  include <avrio/task.h>
#  include <avrio/led.h>

/* ----------------------- Modbus includes ---------------------------------- */
#  include "mb.h"
#  include "mbport.h"

/* ----------------------- Defines ------------------------------------------ */
// Nombre de tranches de 50us en 1 s
#  define MB_50US_TICKS           ( 20000UL )

#if MODBUS_USE_AVRIO_TASK
// ------------------------------------------------------------------------------

/* ----------------------- Static variables --------------------------------- */
static xTaskHandle xMbTimer;

/* internal public functions ================================================ */
// ------------------------------------------------------------------------------
static void
prvTaskRoutine (xTaskHandle xTask __attribute__ ( (unused))) {

#  ifndef MB_TIMER_DEBUG
  (void) pxMBPortCBTimerExpired ();
#  else
  vLedToggle (LED_LED1);
  vMBPortTimersEnable ();
#  endif
}

// ------------------------------------------------------------------------------
BOOL
xMBPortTimersInit (USHORT usTim1Timerout50us) {

#  ifdef MB_TIMER_DEBUG
  vLedInit ();
#  endif

  xMbTimer =
    xTaskCreate (usTim1Timerout50us * AVRIO_KERNEL_TICK_RATE / MB_50US_TICKS,
                 prvTaskRoutine);

  return (xMbTimer != AVRIO_KERNEL_ERROR);
}

// ------------------------------------------------------------------------------
inline void
vMBPortTimersEnable (void) {

  vTaskStart (xMbTimer);
}

// ------------------------------------------------------------------------------
inline void
vMBPortTimersDisable (void) {

  vTaskStop (xMbTimer);
}

#else /* MODBUS_USE_AVRIO_TASK = 0 */
// ------------------------------------------------------------------------------
// Utilisation du Timer 1 16 bits

/* ----------------------- Defines ------------------------------------------*/
#define MB_TIMER_PRESCALER      ( 256UL )
#define MB_TIMER_TICKS          ( F_CPU / MB_TIMER_PRESCALER )

/* ----------------------- Static variables ---------------------------------*/
static USHORT   usTimerOCRADelta;

/* ----------------------- Start implementation -----------------------------*/

// ------------------------------------------------------------------------------
BOOL
xMBPortTimersInit (USHORT usTim1Timerout50us) {

#  ifdef MB_TIMER_DEBUG
  vLedInit ();
#  endif
  /* Calculate overflow counter an OCR values for Timer1. */
  usTimerOCRADelta =
    (MB_TIMER_TICKS * usTim1Timerout50us) / (MB_50US_TICKS);

  TCCR1A = 0x00;
  TCCR1B = 0x00;
  TCCR1C = 0x00;

  vMBPortTimersDisable();

  return TRUE;
}


// ------------------------------------------------------------------------------
inline void
vMBPortTimersEnable() {
  TCNT1 = 0x0000;
  if (usTimerOCRADelta > 0) {

    TIMSK1 |= _BV (OCIE1A);
    OCR1A = usTimerOCRADelta;
  }

  TCCR1B |= _BV (CS12);
}

// ------------------------------------------------------------------------------
inline void
vMBPortTimersDisable() {
  /* Disable the timer. */
  TCCR1B &= ~ _BV (CS12);
  /* Disable the output compare interrupts for channel A/B. */
  TIMSK1 &= ~ (_BV (OCIE1A));
  /* Clear output compare flags for channel A/B. */
  TIFR1 |= _BV (OCF1A) ;
}

// ------------------------------------------------------------------------------
ISR (TIMER1_COMPA_vect) {
#  ifndef MB_TIMER_DEBUG
  (void) pxMBPortCBTimerExpired ();
#  else
  vLedToggle (LED_LED1);
  vMBPortTimersEnable ();
#  endif
}
#endif

#endif /* AVRIO_MODBUS_ENABLE defined */
/* ========================================================================== */
