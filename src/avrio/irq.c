/**
 * @file irq.c
 * @brief Broches d'interruption (Implémentation)
 *
  * Copyright © 2015 epsilonRT. All rights reserved.
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
#include "avrio-config.h"

#ifdef AVRIO_IRQ_ENABLE
/* ========================================================================== */
#include "irq_private.h"
#include <util/atomic.h>

/* private functions ======================================================== */

/* ======================= Multiplexeur d'interruption ====================== */
static void
vIrqHandler (xIrqHandle i) {

  if (xIrq[i].func) {

    xIrq[i].func (i);
  }
}

/* ================= Routines d'interruptions =============================== */
#if defined(INT0_vect) && (IRQ_ISR_MASK & _BV(0))
#undef IRQ_CONTEXT_SIZE
#define IRQ_CONTEXT_SIZE 1
ISR (INT0_vect) {

  vIrqHandler (0);

}
#endif

#if defined(INT1_vect) && (IRQ_ISR_MASK & _BV(1))
#undef IRQ_CONTEXT_SIZE
#define IRQ_CONTEXT_SIZE 2
ISR (INT1_vect) {

  vIrqHandler (1);

}
#endif

#if defined(INT2_vect) && (IRQ_ISR_MASK & _BV(2))
#undef IRQ_CONTEXT_SIZE
#define IRQ_CONTEXT_SIZE 3
ISR (INT2_vect) {

  vIrqHandler (2);

}
#endif

#if defined(INT3_vect) && (IRQ_ISR_MASK & _BV(3))
#undef IRQ_CONTEXT_SIZE
#define IRQ_CONTEXT_SIZE 4
ISR (INT3_vect) {

  vIrqHandler (3);

}
#endif

#if defined(INT4_vect) && (IRQ_ISR_MASK & _BV(4))
#undef IRQ_CONTEXT_SIZE
#define IRQ_CONTEXT_SIZE 5
ISR (INT4_vect) {

  vIrqHandler (4);

}
#endif

#if defined(INT5_vect) && (IRQ_ISR_MASK & _BV(5))
#undef IRQ_CONTEXT_SIZE
#define IRQ_CONTEXT_SIZE 6
ISR (INT5_vect) {

  vIrqHandler (5);

}
#endif

#if defined(INT6_vect) && (IRQ_ISR_MASK & _BV(6))
#undef IRQ_CONTEXT_SIZE
#define IRQ_CONTEXT_SIZE 7
ISR (INT6_vect) {

  vIrqHandler (6);

}
#endif

#if defined(INT7_vect) && (IRQ_ISR_MASK & _BV(7))
#undef IRQ_CONTEXT_SIZE
#define IRQ_CONTEXT_SIZE 8
ISR (INT7_vect) {

  vIrqHandler (7);

}
#endif

/* public variables ========================================================= */
xIrqContext xIrq[IRQ_CONTEXT_SIZE];

/* internal public functions ================================================ */

// -----------------------------------------------------------------------------
void
vIrqAttach (xIrqHandle i, vIrqFunc func, eIrqMode mode) {

  xIrq[i].func = func;
  xIrq[i].mode = mode;
  vIrqInitPin (i);
  vIrqSetMode (i, mode);
  vIrqEnable (i);
}
#endif /* AVRIO_IRQ_ENABLE defined */
/* ========================================================================== */
