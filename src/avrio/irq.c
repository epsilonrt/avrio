/**
 * @file irq.c
 * @brief Broches d'interruption
 *
  * Copyright © 2011 - 2015 Pascal JEAN aka epsilonRT. All rights reserved.
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
