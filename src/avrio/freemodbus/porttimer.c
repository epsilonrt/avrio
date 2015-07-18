/* Copyright _ 2009-2012 epsilonRT. All rights reserved.
 *  $Id$ */
/* ========================================================================== */
#include "avrio-config.h"

#ifdef AVRIO_MODBUS_ENABLE
/* ========================================================================== */
#  include "avrio-board-kernel.h"
#  include "avrio-board-modbus.h"

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

/* ----------------------- Static variables --------------------------------- */
static xTaskHandle xMbTimer;

/* internal public functions ================================================ */
// ------------------------------------------------------------------------------
static void
prvTaskRoutine (xTaskHandle xTask __attribute__ ((unused))) {

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

#endif /* AVRIO_MODBUS_ENABLE defined */
/* ========================================================================== */
