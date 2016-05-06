/**
 * @file serial_private.h
 * @brief Entête privé liaison série asynchrone
 *
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
 */
#ifndef _AVRIO_TC_IRQ_PRIVATE_H_
#define _AVRIO_TC_IRQ_PRIVATE_H_
/* ========================================================================== */
#if ! defined(__DOXYGEN__)
#include "avrio-config.h"

#if defined(AVRIO_TC_ENABLE)
/* ========================================================================== */
#include "tc_private.h"

#if (AVRIO_TC_FLAVOUR == TC_FLAVOUR_IRQ)
/* ========================================================================== */
#include <avrio/queue.h>

/*
 * Device Control Block pour interruption
 */
typedef struct xTcIrqDcb {

  union {
    volatile uint8_t flag;
    struct {
      volatile uint8_t txen: 4;
      volatile uint8_t rxen: 3;
      volatile uint8_t rxstop: 1;
    };
  };
  xQueue * rxbuf;
  xQueue * txbuf;
} xTcIrqDcb;

/* private functions ======================================================== */

// -----------------------------------------------------------------------------
INLINE xTcIrqDcb *
pxTcIrqDcb (xTcPort * p) {

  return (xTcIrqDcb *) p->dcb;
}

/* ========================================================================== */
#endif /* (AVRIO_TC_FLAVOUR & TC_FLAVOUR_IRQ) */
#endif /* AVRIO_TC_ENABLE defined */
#endif /* __DOXYGEN__ not defined */
#endif /* _AVRIO_TC_IRQ_PRIVATE_H_ */
