/**
 * @file serial_rs485_private.h
 * @brief Entête privé liaison série rs485
 *
 * Copyright © 2011-2015 Pascal JEAN aka epsilonRT. All rights reserved.
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
#ifndef _AVRIO_IRQ_PRIVATE_H_
#define _AVRIO_IRQ_PRIVATE_H_
/* ========================================================================== */
#include "irq.h"

/* macros =================================================================== */
/* Sauvegarde les registres avant appel void handler(void*) */
#define IRQ_PROLOG() \
  __asm__( \
           "push r0 \n\t" \
           "in  r0, __SREG__ \n\t" \
           "push r0 \n\t" \
           "push r18 \n\t" \
           "push r19 \n\t" \
           "push r24 \n\t" \
           "push r25 \n\t" \
         )

/* Restaure les registres après appel void handler(void*) et sort de l'IRQ */
#define IRQ_EPILOG() \
  __asm__( \
           "pop r25 \n\t" \
           "pop r24 \n\t" \
           "pop r19 \n\t" \
           "pop r18 \n\t" \
           "pop r0 \n\t" \
           "out __SREG__, r0 \n\t" \
           "pop r0 \n\t" \
           "reti  \n\t" \
         )

/* Sauvegarde les registres avant exécution void handler(void*) */
#define HANDLER_PROLOG() \
  __asm__( \
           "push r1 \n\t" \
           "eor r1, r1 \n\t" \
           "push r20 \n\t" \
           "push r21 \n\t" \
           "push r22 \n\t" \
           "push r23 \n\t" \
           "push r26 \n\t" \
           "push r27 \n\t" \
           "push r30 \n\t" \
           "push r31 \n\t" \
         )

/* Restaure les registres après exécution void handler(void*) */
#define HANDLER_EPILOG() \
  __asm__( \
           "pop r31 \n\t" \
           "pop r30 \n\t" \
           "pop r27 \n\t" \
           "pop r26 \n\t" \
           "pop r23 \n\t" \
           "pop r22 \n\t" \
           "pop r21 \n\t" \
           "pop r20 \n\t" \
           "pop r1 \n\t" \
         )

/* ========================================================================== */
#endif /* _AVRIO_IRQ_PRIVATE_H_ */
