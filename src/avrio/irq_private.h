/**
 * @file serial_rs485_private.h
 * @brief Entête privé liaison série rs485
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
#ifndef _AVRIO_IRQ_PRIVATE_H_
#define _AVRIO_IRQ_PRIVATE_H_
#if ! defined(__DOXYGEN__)
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
#endif /* __DOXYGEN__ not defined */
#endif /* _AVRIO_IRQ_PRIVATE_H_ */
