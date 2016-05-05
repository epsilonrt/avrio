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
#ifndef _AVRIO_SERIAL_RS485_PRIVATE_H_
#define _AVRIO_SERIAL_RS485_PRIVATE_H_
/* ========================================================================== */
#include "avrio-config.h"

#ifdef AVRIO_SERIAL_ENABLE
/* ========================================================================== */
#include <avrio/serial_irq_private.h>

#if AVRIO_SERIAL_FLAVOUR == SERIAL_FLAVOUR_RS485
// -----------------------------------------------------------------------------
#ifndef SERIAL_TXEN_ENABLE
#error SERIAL_TXEN_ENABLE must be defined for RS485 serial link !
#endif
// -----------------------------------------------------------------------------
#endif /* AVRIO_SERIAL_FLAVOUR == SERIAL_FLAVOUR_RS485 */

/* ========================================================================== */
#endif /* AVRIO_SERIAL_ENABLE defined */
#endif /* _AVRIO_SERIAL_RS485_PRIVATE_H_ */
