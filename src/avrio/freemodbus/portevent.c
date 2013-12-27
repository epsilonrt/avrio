/* Copyright _ 2009-2012 epsilonRT. All rights reserved.
 *  $Id$ */
/* ========================================================================== */
#include "avrio-config.h"

#ifdef AVRIO_MODBUS_ENABLE
/* ========================================================================== */
#  include "avrio-board-modbus.h"

/* ----------------------- Platform includes -------------------------------- */
#  include "port.h"

/* ----------------------- Modbus includes ---------------------------------- */
#  include "mb.h"
#  include "mbport.h"

/* ----------------------- Variables ---------------------------------------- */
static eMBEventType eQueuedEvent;
static BOOL xEventInQueue;

// ------------------------------------------------------------------------------
BOOL
xMBPortEventInit (void) {
  xEventInQueue = FALSE;
  return TRUE;
}

// ------------------------------------------------------------------------------
BOOL
xMBPortEventPost (eMBEventType eEvent) {
  xEventInQueue = TRUE;
  eQueuedEvent = eEvent;
  return TRUE;
}

// ------------------------------------------------------------------------------
BOOL
xMBPortEventGet (eMBEventType * eEvent) {
  BOOL xEventHappened = FALSE;

  if (xEventInQueue) {
    *eEvent = eQueuedEvent;
    xEventInQueue = FALSE;
    xEventHappened = TRUE;
  }
  return xEventHappened;
}

#endif /* AVRIO_MODBUS_ENABLE defined */
/* ========================================================================== */
